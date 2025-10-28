"""
Authentication module for AniLex
Provides OAuth authentication with AniList API
"""

import http.server
import json
import logging
import os
import socketserver
import sys
import threading
import time
import webbrowser
from datetime import datetime
from logging.handlers import RotatingFileHandler
from typing import Optional, Dict, Any
from urllib.parse import urlparse, parse_qs

import keyring
import requests
from dateutil.relativedelta import relativedelta
from old.anilex_helper import get_cache_path


class AniListAuthenticator:
    """Handles AniList OAuth authentication flow"""
    
    def __init__(self, client_id: str = '28840', port: int = 8000):
        self.client_id = client_id
        self.port = port
        self.redirect_uri = f'http://localhost:{port}/callback'
        self.auth_url = f"https://anilist.co/api/v2/oauth/authorize?client_id={client_id}&response_type=token"
        self.api_url = "https://graphql.anilist.co"
        self.service_name = "UNOFICIAL-ANILEX"
        self.user_data_path = os.path.join(get_cache_path(), "user_data.json")
        self._received_token: Optional[str] = None


        # logger setup
        log_path = os.path.join(get_cache_path(), "logs", "anilist_auth.log")
        os.makedirs(os.path.dirname(log_path), exist_ok=True)
        logging.basicConfig(
            level=logging.DEBUG,
            format='%(asctime)s - %(levelname)s - %(message)s',
            handlers=[
                RotatingFileHandler(log_path, maxBytes=1_000_000, backupCount=5),
                logging.StreamHandler(sys.stdout)
            ]
        )
        self.logger = logging.getLogger(__name__)


    def is_already_authenticated(self) -> bool:
        """Check if user is already authenticated"""
        if not os.path.exists(self.user_data_path):
            return False
        
        try:
            with open(self.user_data_path, "r") as file:
                data = json.load(file)
            return data.get("auth", {}).get("user_token", "unsaved") == "saved"
        except (json.JSONDecodeError, IOError):
            return False
    
    def _create_handler_class(self):
        """Create HTTP request handler class with access to instance variables"""
        authenticator = self
        
        class AniListHandler(http.server.SimpleHTTPRequestHandler):
            def log_message(self, format, *args):
                """Suppress default logging"""
                pass
            
            def do_GET(self):
                if self.path.startswith("/callback"):
                    self._handle_callback()
                elif self.path.startswith("/token"):
                    self._handle_token()
                else:
                    self.send_error(404, "Not found")
            
            def _handle_callback(self):
                """Handle OAuth callback and serve HTML page"""
                self.send_response(200)
                self.send_header("Content-type", "text/html")
                self.end_headers()
                self.wfile.write(self._get_callback_html().encode())
            
            def _handle_token(self):
                """Extract and store access token"""
                query = urlparse(self.path).query
                access_token = parse_qs(query).get("access_token", [None])[0]
                
                if access_token:
                    authenticator._received_token = access_token
                    self.send_response(200)
                    self.send_header("Content-type", "text/plain")
                    self.end_headers()
                    self.wfile.write("Token received successfully".encode())
                else:
                    self.send_error(400, "No access token found")
            
            def _get_callback_html(self) -> str:
                """Return HTML for the callback page"""
                return """
                <html lang="en">
                    <head>
                        <meta charset="UTF-8" />
                        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
                        <title>AniList Authentication</title>
                        <link href="https://fonts.googleapis.com/css?family=Roboto:400,700&display=swap" rel="stylesheet" />
                        <style>
                            body {
                                margin: 0;
                                padding: 0;
                                font-family: 'Roboto', sans-serif;
                                background: #f4f6f8;
                                color: #333;
                                display: flex;
                                height: 100vh;
                                align-items: center;
                                justify-content: center;
                            }
                            .container {
                                background: #fff;
                                padding: 2rem;
                                border-radius: 12px;
                                box-shadow: 0 4px 20px rgba(0,0,0,0.1);
                                text-align: center;
                                max-width: 400px;
                                width: 90%;
                            }
                            h1 {
                                margin-bottom: 1rem;
                                font-size: 1.5rem;
                                font-weight: 700;
                            }
                            p {
                                margin: 0.5rem 0 1.5rem;
                                font-size: 1rem;
                            }
                            .success { color: #2a9d8f; }
                            .error { color: #e76f51; }
                        </style>
                    </head>
                    <body>
                        <div class="container">
                            <h1 id="status">Authentication in progress...</h1>
                            <p>Please wait while we process your authentication.</p>
                        </div>
                        
                        <script>
                            const hash = window.location.hash.substring(1);
                            const params = new URLSearchParams(hash);
                            const accessToken = params.get('access_token');
                            const statusEl = document.getElementById('status');
                            
                            if (accessToken) {
                                fetch('/token?access_token=' + accessToken)
                                    .then(res => {
                                        if (res.ok) {
                                            statusEl.textContent = 'Authentication successful!';
                                            statusEl.classList.add('success');
                                            document.querySelector('p').textContent = 'You can close this tab now.';
                                        } else {
                                            throw new Error('Server error');
                                        }
                                    })
                                    .catch(() => {
                                        statusEl.textContent = 'Error saving token!';
                                        statusEl.classList.add('error');
                                        document.querySelector('p').textContent = 'Please try again.';
                                    });
                            } else {
                                statusEl.textContent = 'No access token found!';
                                statusEl.classList.add('error');
                                document.querySelector('p').textContent = 'Authentication failed.';
                            }
                        </script>
                    </body>
                </html>
                """
        
        return AniListHandler
    
    def _start_local_server(self) -> None:
        """Start local server to handle OAuth callback"""
        handler_class = self._create_handler_class()
        
        def server_loop():
            with socketserver.TCPServer(("", self.port), handler_class) as httpd:
                self.logger.info(f"Local server started on http://localhost:{self.port}/callback")
                while not self._received_token:
                    httpd.handle_request()
        
        server_thread = threading.Thread(target=server_loop, daemon=True)
        server_thread.start()
    
    def _get_user_info(self, token: str) -> Dict[str, Any]:
        """Fetch user information from AniList API"""
        headers = {
            'Authorization': f'Bearer {token}',
            'Content-Type': 'application/json',
            'Accept': 'application/json',
        }
        
        query = """
        {
            Viewer {
                id
                name
            }
        }
        """
        
        response = requests.post(
            self.api_url, 
            json={'query': query}, 
            headers=headers,
            timeout=10
        )
        
        if response.status_code != 200:
            self.logger.error(f"API request failed {response.status_code}")
            raise Exception(f"API request failed with status ")
        
        data = response.json()
        if "errors" in data:
            self.logger.error(f"API returned errors: {data['errors']}")
            raise Exception(f"API returned errors: {data['errors']}")
        
        return data['data']['Viewer']
    
    def _save_token_to_keyring(self, username: str, user_id: int, token: str) -> None:
        """Save token to system keyring"""
        user_token_key = f"{username}-{user_id}-AniLex"
        keyring.set_password(self.service_name, user_token_key, token)
        self.logger.info(f"Token saved to keyring")
    
    def _save_user_data(self, username: str, user_id: int) -> None:
        """Save user data to JSON file"""
        user_token_key = f"{username}-{user_id}-AniLex"

        renew = datetime.now() + relativedelta(years=1)

        data = {
            username: {
                "auth": {
                    "user_id": user_id,
                    "renew_token": renew.isoformat(),
                    "user_token": "saved"
                },
                "auth_key": {
                    "service_name": self.service_name,
                    "user_token_key": user_token_key,
                    "user_data_path": self.user_data_path,
                    "keyring_backend": "Windows"
                }
            }
        }
        
        # Ensure directory exists
        os.makedirs(os.path.dirname(self.user_data_path), exist_ok=True)
        
        with open(self.user_data_path, "a") as file:
            json.dump(data, file, indent=4)

        self.logger.info(f"User data saved to {self.user_data_path}")
    
    def authenticate(self, force_reauth: bool = False) -> Dict[str, Any]:
        """
        Perform the authentication process
        :param force_reauth: Force re-authentication even if already authenticated
        :return: return Dict containing user info (username, user_id)
        :raises Exception: if authentication fails
        """
        # Check if already authenticated
        if not force_reauth and self.is_already_authenticated():
            self.logger.info("User already authenticated, loading existing data.")
            # Load existing user data
            with open(self.user_data_path, "r") as file:
                data = json.load(file)
            return {
                "username": data["auth"]["username"],
                "user_id": data["auth"]["user_id"]
            }

        self.logger.info(f"Authentication process started...")
        
        try:
            # Start local server
            self._start_local_server()
            
            # Open browser for authentication
            webbrowser.open(self.auth_url)
            self.logger.info("Browser opened. Please complete authentication in your browser.")
            
            # Wait for token
            timeout = 300  # 5 minutes timeout
            start_time = time.time()
            
            while not self._received_token:
                if time.time() - start_time > timeout:
                    self.logger.error("Authentication timeout. Please try again.")
                    return {}
                time.sleep(0.1)

            self.logger.info("Token received, verifying with AniList...")
            
            # Get user information
            user_info = self._get_user_info(self._received_token)
            username = user_info['name']
            user_id = user_info['id']
            
            # Save token and user data
            self._save_token_to_keyring(username, user_id, self._received_token)
            self._save_user_data(username, user_id)

            self.logger.info("Authentication successful!")
            
            return {
                "username": username,
                "user_id": user_id
            }
            
        except Exception as e:
            self.logger.error(f"Authentication failed: {e}")
            raise



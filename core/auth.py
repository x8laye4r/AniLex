'''
    Authentification for AniLex
'''

import http.server
import socketserver
import threading
import webbrowser
import requests
import time
import json
from urllib.parse import urlparse, parse_qs
import keyring
import sys
import os

'''
    Check if file exists and if the files are already saved
'''
if os.path.exists(r"C:\Users\eisel\PycharmProjects\UNOFICIAL-ANILEX\cache\user_data.json"):
    with open(r"C:\Users\eisel\PycharmProjects\UNOFICIAL-ANILEX\cache\user_data.json", "r") as file:
        data = json.load(file)

    if data["auth"]["user_token"] == "saved":
        print("Already saved!")
        sys.exit(0)



'''
    Data for local-server and AniList 
'''
PORT = 8000
CLIENT_ID = '28840'
REDIRECT_URI = f'http://localhost:{PORT}/callback'
url = "https://graphql.anilist.co"
auth_url = f"https://anilist.co/api/v2/oauth/authorize?client_id={CLIENT_ID}&response_type=token"

# Code for altering user data and getting user data
received_code = None

# Class for the server
class AniListHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        global received_code
        if self.path.startswith("/callback"):
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.end_headers()
            self.wfile.write("""
                <html lang="de">
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
                        .success {
                          color: #2a9d8f;
                        }
                        .error {
                          color: #e76f51;
                        }
                        .close-btn {
                          padding: 0.6rem 1.2rem;
                          font-size: 1rem;
                          font-weight: 700;
                          border: none;
                          border-radius: 6px;
                          background: #2a9d8f;
                          color: #fff;
                          cursor: pointer;
                          transition: transform 0.2s ease, background 0.2s ease;
                        }
                        .close-btn:hover {
                          transform: scale(1.05);
                          background: #21867a;
                        }
                      </style>
                    </head>
                    <body>
                      <div class="container">
                        <h1 id="status">Authentication running...</h1>
                        <p>Please wait a moment, you can't refresh the site manually.</p>
                        <button id="closeBtn" class="close-btn" style="display:none;" onclick="window.close();">
                          Close Tab
                        </button>
                      </div>
                    
                      <script>
                        const hash = window.location.hash.substring(1);
                        const params = new URLSearchParams(hash);
                        const accessToken = params.get('access_token');
                        const statusEl = document.getElementById('status');
                        const closeBtn = document.getElementById('closeBtn');
                    
                        function showCloseButton() {
                          closeBtn.style.display = 'inline-block';
                        }
                    
                        if (accessToken) {
                          fetch('/token?access_token=' + accessToken)
                            .then(res => {
                              if (res.ok) {
                                statusEl.textContent = 'Authentication successful';
                                statusEl.classList.add('success');
                                document.querySelector('p').textContent = 'You can close this tab now';
                                showCloseButton();
                              } else {
                                throw new Error('Fetch Error');
                              }
                            })
                            .catch(() => {
                              statusEl.textContent = 'Error while saving the Token!';
                              statusEl.classList.add('error');
                              document.querySelector('p').textContent = '';
                              showCloseButton();
                            });
                        } else {
                          statusEl.textContent = 'Error no token was found!';
                          statusEl.classList.add('error');
                          document.querySelector('p').textContent = '';
                          showCloseButton();
                        }
                        closeBtn.addEventListener("click", function() {
                            window.close()
                        });
                      </script>
                    </body>
                </html>
            """.encode())
        elif self.path.startswith("/token"):
            query = urlparse(self.path).query
            access_token = parse_qs(query).get("access_token", [None])[0]
            if access_token:
                received_code = access_token
                self.send_response(200)
                self.send_header("Content-type", "text/plain")
                self.end_headers()
                self.wfile.write("Token found".encode())
            else:
                self.send_error(400, "No token was found")
        else:
            self.send_error(404, "Nothing found")

def start_server_until_code():
    with socketserver.TCPServer(("", PORT), AniListHandler) as httpd:
        print(f"Local server started on http://localhost:{PORT}/callback")
        while not received_code:
            httpd.handle_request()

# Start the local server
server_thread = threading.Thread(target=start_server_until_code, daemon=True)
server_thread.start()

webbrowser.open(auth_url)
print("Browser opened. Login for AniList is ready")

while not received_code:
    time.sleep(0.1)

# Header and query for getting the code
headers = {
    'Authorization': f'Bearer {received_code}',
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

# Getting the code
response = requests.post(url, json={'query': query}, headers=headers)
if response.status_code != 200 or "errors" in response.json():
    print("Error getting the code")
    sys.exit(1)

viewer = response.json()['data']['Viewer']
username = viewer['name']
user_id = viewer['id']
user_token = received_code
del received_code

# Defining path
PATH = os.path.expanduser(r"~\\PycharmProjects\\UNOFICIAL-ANILEX\\cache\\user_data.json")
SERVICE_NAME = "UNOFICIAL-ANILEX"
USER_TOKEN_KEY = f"{username}-{user_id}-AniLex"

# Save token in keyring
def save_token(token):
    if token:
        keyring.set_password(SERVICE_NAME, USER_TOKEN_KEY, token)
        print("Token saved")


# Save user-data
data = {
    "auth": {
        "username": username,
        "user_id": user_id,
        "user_token": "saved"
    },
    "auth_key": {
        "service_name": SERVICE_NAME,
        "user_token_key": USER_TOKEN_KEY,
        "user_data_path": PATH,
        "keyring_backend": "Windows"
    }
}

os.makedirs(os.path.dirname(PATH), exist_ok=True)
with open(PATH, "w") as file:
    json.dump(data, file, indent=4)


save_token(user_token)

print("Authentification successful!")

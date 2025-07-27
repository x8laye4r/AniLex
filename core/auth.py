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
            <html>
            <body>
                <h1>Authentification is running</h1>
                <script>
                    const hash = window.location.hash.substring(1);
                    const params = new URLSearchParams(hash);
                    const accessToken = params.get('access_token');
                    if (accessToken) {
                        fetch('/token?access_token=' + accessToken)
                            .then(() => {
                                document.body.innerHTML = '<h1>Authentification successful!</h1><p>You can close the browser now</p>';
                            });
                    } else {
                        document.body.innerHTML = '<h1>Fehler: Kein Access Token gefunden!</h1>';
                    }
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

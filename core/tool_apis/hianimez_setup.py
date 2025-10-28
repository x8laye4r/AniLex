import json
import os
import shutil
import subprocess
import threading
import time

import git
from git import Repo
from github import Github
from old.anilex_helper import get_cache_path

if not os.path.exists(os.path.join(get_cache_path(), "api_data","aniwatchapi.json")):
    with open(os.path.join(get_cache_path(), "api_data", "aniwatchapi.json"), "w") as f:
        json.dump({}, f)

class HianimezSetup:
    '''
    Clones the repo https://github.com/ghoshRitesh12/aniwatch-api
    firstly it downloads the repo if not exists, then it starts a local server using npm and node
    '''

    def __init__(self, target_dir=None, branch=None):
        self.repo_url = "https://github.com/ghoshRitesh12/aniwatch-api.git"
        self.repo_owner = "ghoshRitesh12"
        self.repo_name = "aniwatch-api"
        self.target_dir = target_dir or os.path.join(get_cache_path(), "api_data", "aniwatch-api")
        self.branch = branch
        self.server_process = None

    def repo_valid(self):
        # Check if .git exists and package.json exists
        if not os.path.isdir(self.target_dir):
            print(f"Target directory {self.target_dir} does not exist.")
            return False
        if not os.path.isdir(os.path.join(self.target_dir, ".git")):
            print(f"No .git directory found in {self.target_dir}.")
            return False
        if not os.path.isfile(os.path.join(self.target_dir, "package.json")):
            print(f"No package.json found in {self.target_dir}.")
            return False
        return True

    def clone_repo(self):
        if not self.repo_valid():
            try:
                os.makedirs(self.target_dir, exist_ok=True)
                print("Cloning aniwatch-api repository...")
                Repo.clone_from(self.repo_url, self.target_dir, branch=self.branch)
                print("Repository cloned successfully.")
            except git.GitCommandError as e:
                print("Git command error: ", e)
            except Exception as e:
                print("Error cloning repository: ", e)
        else:
            print("Repository already exists and is valid. Skipping clone.")

    def delete_and_install(self):
        if os.path.exists(self.target_dir):
            shutil.rmtree(self.target_dir)
        self.clone_repo()

    @staticmethod
    def is_npm_installed():
        try:
            subprocess.run(["npm", "--version"], capture_output=True, text=True, check=True, shell=True)
            return True
        except (FileNotFoundError, subprocess.CalledProcessError):
            return False

    @staticmethod
    def read_api_data():
        api_data_path = os.path.join(get_cache_path(), "api_data", "aniwatchapi.json")
        if os.path.isfile(api_data_path):
            try:
                with open(api_data_path, "r") as file:
                    return json.load(file)
            except Exception as e:
                print("Error reading api data: ", e)
                return {}
        return {}

    @staticmethod
    def write_api_data(key, value):
        data = AniwatchSetup.read_api_data()
        data[key] = value
        with open(os.path.join(get_cache_path(), "api_data", "aniwatchapi.json"), "w") as file:
            json.dump(data, file)


    def downgrade_pino(self):
        if self.get_data_from_api("pino_downgrade"):
            print("Pino already downgraded. Skipping downgrade.")
            return True
        if not self.is_npm_installed():
            print("NPM is not installed. Please install Node.js and NPM first.")
            return False
        current_dir = os.getcwd()
        os.chdir(self.target_dir)
        try:
            result = subprocess.run(["npm", "install", "pino@7"], capture_output=True, text=True, shell=True)
            if result.returncode == 0:
                print("Pino downgraded successfully.")
                os.chdir(current_dir)
                self.write_api_data("pino_downgrade", True)
                return True
            else:
                print("Error downgrading pino: ", result.stderr)
                os.chdir(current_dir)
                return False
        except Exception as e:
            print("Exception during pino downgrade: ", e)
            os.chdir(current_dir)
            return False

    @staticmethod
    def get_data_from_api(key):
        data = AniwatchSetup.read_api_data()
        return data.get(key, False)

    def install_dependencies(self):
        if self.get_data_from_api("dependencies_installed"):
            print("Dependencies already installed. Skipping installation.")
            return True
        if not self.is_npm_installed():
            print("NPM is not installed. Please install Node.js and NPM first.")
            return False
        current_dir = os.getcwd()
        os.chdir(self.target_dir)
        print("Installing dependencies using npm...")
        try:
            result = subprocess.run(["npm", "install"], capture_output=True, text=True, shell=True)
            if result.returncode == 0:
                print("Dependencies installed successfully.")
                os.chdir(current_dir)
                self.write_api_data("dependencies_installed", True)
                return True
            else:
                print("Error installing dependencies: ", result.stderr)
                os.chdir(current_dir)
                return False
        except Exception as e:
            print("Exception during npm install: ", e)
            os.chdir(current_dir)
            return False

    def build_api(self):
        if self.get_data_from_api("api_built"):
            print("API already built. Skipping build step.")
            return True
        if not self.is_npm_installed():
            print("NPM is not installed.")
            return False
        current_dir = os.getcwd()
        os.chdir(self.target_dir)
        try:
            result = subprocess.run(["npm", "run", "build"], capture_output=True, text=True, shell=True)
            if result.returncode == 0:
                print("API built successfully.")
                # Check for expected build artifacts (example: dist folder or main file)
                dist_path = os.path.join(self.target_dir, "dist")
                if not os.path.isdir(dist_path):
                    print("Warning: Build completed but dist/ not found.")
                os.chdir(current_dir)
                self.write_api_data("api_built", True)
                return True
            else:
                print("Error building API: ", result.stderr)
                os.chdir(current_dir)
                return False
        except Exception as e:
            print("Exception during API build: ", e)
            os.chdir(current_dir)
            return False

    def start_server(self):
        if not self.is_npm_installed():
            print("NPM is not installed.")
            return False
        current_dir = os.getcwd()
        os.chdir(self.target_dir)
        try:
            self.server_process = subprocess.Popen(
                ["npm", "start"],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                shell=True,
                env=os.environ.copy(),
                creationflags=subprocess.CREATE_NEW_PROCESS_GROUP if os.name == "nt" else 0,
            )

            def monitor_process():
                while True:
                    output = self.server_process.stdout.readline()
                    if output == '' and self.server_process.poll() is not None:
                        break
                    if output:
                        print("Server Output: ", output.strip())
                err = self.server_process.stderr.read()
                if err:
                    print("Server Error: ", err.strip())

            threading.Thread(target=monitor_process, daemon=True).start()
            time.sleep(2)
            if self.server_process.poll() is None:
                print("Server started successfully.")
                os.chdir(current_dir)
                return True
            else:
                print("Server failed to start.")
                stdout, stderr = self.server_process.communicate()
                print("Output:", stdout)
                print("Error:", stderr)
                os.chdir(current_dir)
                return False
        except Exception as e:
            print("Exception during server start: ", e)
            os.chdir(current_dir)
            return False

    def server_running(self):
        return self.server_process and self.server_process.poll() is None

    def server_stop(self):
        if self.server_running():
            pid = self.server_process.pid
            if os.name == "nt":
                # Windows: kill process tree
                subprocess.call(["taskkill", "/F", "/T", "/PID", str(pid)])
            else:
                # Unix: terminate process group
                self.server_process.terminate()
                try:
                    self.server_process.wait(timeout=5)
                    print("Server stopped successfully.")
                except subprocess.TimeoutExpired:
                    self.server_process.kill()
                    print("Server forcefully killed.")
            print("Server stopped successfully.")
        else:
            print("Server is not running.")

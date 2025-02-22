from flask import Flask, request, abort
import requests
import json
import time

# Configure Flask to serve static files from the 'www' folder at the root URL.
app = Flask(__name__, static_folder='../www', static_url_path='')
# Function to check if the IP is from a hosting provider
def is_hosting_provider(ip):
    try:
        response = json.loads(requests.get(f"http://ip-api.com/json/{ip}?fields=16826371", timeout=5).text())
        return response["hosting"]
    except Exception as e:
        print(f"Error checking IP: {e}")
        return False  # Fail-safe: Allow access if API fails

@app.before_request
def log_user():
    if is_hosting_provider(request.remote_addr):
        abort(403)
        f = open("../log/useragents.log","a")
        f.write("HOSTING BLOCKED " + request.remote_addr + ": " + request.headers.get('User-Agent')+ " at " + str(time.time())+ " for " + request.path + '\n')
        f.close()
        return
    f = open("../log/useragents.log","a")
    f.write(request.remote_addr + ": " + request.headers.get('User-Agent')+ " at " + str(time.time())+ " for " + request.path + '\n')
    f.close()

# Explicitly serve index.html when accessing the root URL.
@app.route('/')
def index():
    return app.send_static_file('index.html')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80)
from flask import Flask, request, abort
import requests
import json
import time

# Configure Flask to serve static files from the 'www' folder at the root URL.
app = Flask(__name__, static_folder='../www', static_url_path='')
# Function to check if the IP is from a hosting provider
def is_blocked(ip):
    try:
        response = json.loads(requests.get(f"http://ip-api.com/json/{ip}?fields=66846719", timeout=5).text)
        # return response["hosting"]
        if response["hosting"] or response["country"] == "Russia" or response["country"] == "China":
            return True
        else:
            return False

    except Exception as e:
        print(f"Error checking IP: {e}")
        return True  # dissallow access if the api fails

@app.before_request
def log_user():
    if is_blocked(request.headers.get("CF-Connecting-IP", request.remote_addr)):
        f = open("../log/useragents.log","a")
        f.write("BLOCKED " + request.headers.get("CF-Connecting-IP", request.remote_addr) + ": " + request.headers.get('User-Agent')+ " at " + str(time.time())+ " for " + request.path + '\n')
        f.close()
        abort(403)
        return
    f = open("../log/useragents.log","a")
    f.write(request.headers.get("CF-Connecting-IP", request.remote_addr) + ": " + request.headers.get('User-Agent')+ " at " + str(time.time())+ " for " + request.path + '\n')
    f.close()

# Explicitly serve index.html when accessing the root URL.
@app.route('/')
def index():
    return app.send_static_file('index.html')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80)

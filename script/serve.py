from flask import Flask, request
import time

# Configure Flask to serve static files from the 'www' folder at the root URL.
app = Flask(__name__, static_folder='../www', static_url_path='')

# Explicitly serve index.html when accessing the root URL.
@app.route('/')
def index():
    f = open("../log/useragents.log","a")
    f.write(request.remote_addr + ": " + request.headers.get('User-Agent')+ " at " + str(time.time())+'\n')
    f.close()
    return app.send_static_file('index.html')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80)
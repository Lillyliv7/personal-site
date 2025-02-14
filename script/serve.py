from flask import Flask

# Configure Flask to serve static files from the 'www' folder at the root URL.
app = Flask(__name__, static_folder='../www', static_url_path='')

# Explicitly serve index.html when accessing the root URL.
@app.route('/')
def index():
    return app.send_static_file('index.html')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80)
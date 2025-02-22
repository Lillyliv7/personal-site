let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/tarball/nixos-24.05";
  pkgs = import nixpkgs { config = {}; overlays = []; };
in

pkgs.mkShellNoCC {
    buildInputs = [
        pkgs.nodejs
        pkgs.typescript
        pkgs.python3
        pkgs.python3Packages.flask
        pkgs.python3Packages.gunicorn
        pkgs.python3Packages.requests
        pkgs.yuicompressor
        pkgs.html-minifier
        pkgs.gcc
        pkgs.libxml2
        pkgs.pkg-config
        pkgs.libwebsockets
        pkgs.openssl.dev
    ];

    shellHook = ''
        mkdir build
        echo "compile c scripts"

        gcc c/injectcss.c -o build/injectcss $(pkg-config --cflags --libs libxml-2.0)
        ./build/injectcss ./html/ ./css/ ./www/

        echo "bash run.bash"
        cd script
        bash run.bash
        echo "Starting HTTP server"
        gunicorn -w 4 -b 0.0.0.0:80 serve:app
        echo "Exiting Nix environment"
        exit
    '';
}

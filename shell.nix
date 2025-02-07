let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/tarball/nixos-24.05";
  pkgs = import nixpkgs { config = {}; overlays = []; };
in

pkgs.mkShellNoCC {
    buildInputs = [
        pkgs.nodejs
        pkgs.typescript
        pkgs.python3
        pkgs.yuicompressor
    ];

    shellHook = ''
        mkdir www
        echo "Cleaning ./www/js"
        rm -rf ./www/js/*
        echo "Compiling Typescript files in ./typescript to ./www/js"
        tsc --project ./tsconfig.json
        echo "Uglifying Javascript files with Yuicompressor"
        find ./www/js -name "*.js" -size +0c -exec yuicompressor {} -o {} \;
        echo "Uglifying CSS files with Yuicompressor"
        find ./css -name "*.css" -exec sh -c 'yuicompressor "$0" -o "./www/css/$(basename "$0")"' {} \;
        echo "Copying HTML files"
        cp -r ./html/* ./www
        echo "Starting HTTP server"
        cd www
        python3 -m http.server 8080
        echo "Server exited"
        echo "closing nix shell"
        exit
    '';
}

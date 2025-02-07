let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/tarball/nixos-24.05";
  pkgs = import nixpkgs { config = {}; overlays = []; };
in

pkgs.mkShell {
    buildInputs = [
        pkgs.nodejs
        pkgs.typescript
        pkgs.python3
    ];

    shellHook = ''
        echo "Cleaning ./www/js"
        rm -rf ./www/js/*
        echo "Compiling typescript files in ./typescript to ./www/js"
        tsc --project ./tsconfig.json
        echo "Starting http server"
        cd www
        python3 -m http.server 8080
        echo "Server exited"
        echo "closing nix shell"
        exit
    '';
}

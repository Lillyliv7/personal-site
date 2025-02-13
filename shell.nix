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
        pkgs.html-minifier
        pkgs.gcc
        pkgs.libxml2
        pkgs.pkg-config
    ];

    shellHook = ''
        mkdir build
        echo "compile c scripts"
        gcc c/injectcss.c -o build/injectcss $(pkg-config --cflags --libs libxml-2.0)
        ./build/injectcss ./html/ ./css/ ./www/
        echo "bash run.bash"
        cd script
        bash run.bash
        echo "Exiting Nix environment"
        exit
    '';
}

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
    ];

    shellHook = ''
        echo "bash run.bash"
        cd script
        bash run.bash
        echo "Exiting Nix environment"
        exit
    '';
}

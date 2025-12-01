{
  description = "Development flake for my AOC 2025 solutions";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = {
    systems,
    nixpkgs,
    ...
  }: let
    inherit (nixpkgs) lib;
    forEachPkgs = f: lib.genAttrs (import systems) (system: f nixpkgs.legacyPackages.${system});
  in {
    devShells = forEachPkgs (pkgs: {
      default = pkgs.mkShell {
        # Will use inputsFrom when I can
        buildInputs = with pkgs; [
          gcc
        ];

        packages = with pkgs; [
          nixd
          alejandra
          bear
          just
          clang-tools
        ];
      };
    });
  };
}

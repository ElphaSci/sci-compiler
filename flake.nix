{
  description = "The Realm Online Server";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    devenv.url = "github:cachix/devenv";
    nix2container.url = "github:nlewo/nix2container";
    nix2container.inputs.nixpkgs.follows = "nixpkgs";
    mk-shell-bin.url = "github:rrbutani/nix-mk-shell-bin";
    flake-root.url = "github:srid/flake-root";
    treefmt-nix.url = "github:numtide/treefmt-nix";
  };

  nixConfig = {
    extra-trusted-public-keys = "devenv.cachix.org-1:w1cLUi8dv3hnoSPGAuibQv+f9TZLr6cv/Hm9XgU50cw=";
    extra-substituters = "https://devenv.cachix.org";
  };

  outputs = inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      imports = [
        inputs.devenv.flakeModule
        inputs.treefmt-nix.flakeModule
        inputs.flake-root.flakeModule
        inputs.flake-parts.flakeModules.easyOverlay
      ];
      systems = [ "x86_64-linux" ];

      perSystem = { config, pkgs, ... }:
        let

        in
        {


          # Per-system attributes can be defined here. The self' and inputs'
          # module parameters provide easy access to attributes of the same
          # system.
          treefmt.config = {
            inherit (config.flake-root) projectRootFile;
            package = pkgs.treefmt;
            programs.nixpkgs-fmt.enable = true;
            programs.prettier.enable = true;
            programs.beautysh = {
              enable = true;
              indent_size = 4;
            };
          };

          packages.default = pkgs.callPackage ./nix { };


          overlayAttrs = {
            inherit (config.packages) realm-server;
          };
          packages.realm-server = pkgs.callPackage ./nix { };

          devenv.shells.default = {
            name = "sci-compiler-dev";

            imports = [
              # This is just like the imports in devenv.nix.
              # See https://devenv.sh/guides/using-with-flake-parts/#import-a-devenv-module
              # ./devenv-foo.nix
            ];

            # https://devenv.sh/reference/options/
            packages = with pkgs; [
              /*build packages. Don't need all of them, but haven't removed all unnecessary yet */
              cmake
              pkg-config
              gcc
              libgcc
            ];

            scripts = {
              build = {
                exec = ''
                  rm bin/*
                  build_dir=cmake-build-''${2:-debug}
                  mkdir -p ''${build_dir}
                  cmake -B ''${build_dir}
                  cmake --build cmake-build-''${2:-debug} --target ''${1:-all} -- -j 22
                '';
              };
              rebuild = {
                exec = ''
                  rm CMakeCache.txt
                  build_type=''${1:-debug}
                  build_dir=cmake-build-''${build_type}
                  rm -rf ''${build_dir}
                  cmake -B ''${build_dir}
                  cmake -DCMAKE_BUILD_TYPE=''${build_type^} -B ''${build_dir}
                  build all ''${build_type}
                '';
              };
            };

            process.manager.implementation = "process-compose";
            processes = {
              build-release.exec = ''build all release'';
              build-debug = {
                exec = ''build all debug'';
                process-compose.disabled = true;
              };
              rebuild-release = {
                exec = ''rebuild release'';
                process-compose.disabled = true;
              };
              rebuild-debug = {
                exec = ''rebuild debug'';
                process-compose.disabled = true;
              };
            };
          };


        };
      flake = {
        # The usual flake attributes can be defined here, including system-
        # agnostic ones like nixosModule and system-enumerating ones, although
        # those are more easily expressed in perSystem.
      };
    };
}


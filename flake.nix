{
  description = "Writing an Operating System in C++ (inspired from DimOS)";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs {
          inherit system;

          config = {
            allowUnsupportedSystem = true;
          };
        };
        llvm = pkgs.llvmPackages_19;
      in
      with pkgs;
      {
        devShells.default = mkShell {
          nativeBuildInputs = [
            clang-tools
            llvm.clang
            ninja
            cmake
          ];

          buildInputs = [
            llvmPackages_19.libcxx
          ];

          CXX = "clang++";
          CC = "clang";
          CXXFLAGS = "-stdlib=libc++";
          LDFLAGS = "-stdlib=libc++";

          CLANGD_PATH = (lib.makeSearchPath "bin/clangd" [ clang-tools ]);
        };
      }
    );
}

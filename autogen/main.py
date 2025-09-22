import clang.cindex
from args import Args
from parser import Parse


def ConfigureClangLibrary(clangPath: str) -> None:
    clang.cindex.Config.set_library_file(clangPath)  # type: ignore


def main():
    args = Args()

    ConfigureClangLibrary(args.ClangPath)
    Parse(args.InputFile)


if __name__ == "__main__":
    main()

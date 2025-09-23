import clang.cindex
from args import Args
from generate import Generate


def ConfigureClangLibrary(clangPath: str) -> None:
    clang.cindex.Config.set_library_file(clangPath)  # type: ignore


def main():
    args = Args()

    ConfigureClangLibrary(args.ClangPath)
    output = Generate(args.InputFile, args.TemplateFile)

    with open(args.OutputFile, "w", encoding="utf-8") as f:
        f.write(output)


if __name__ == "__main__":
    main()

import argparse


class Args:
    def __init__(self):
        parser = argparse.ArgumentParser(
            description="Used for analyzing the c source files for determining their structure."
        )

        parser.add_argument(
            "-i",
            "--input",
            type=str,
            nargs="+",
            help="Path to the input C source files (absolute path).",
        )

        parser.add_argument(
            "--clang-path",
            type=str,
            required=True,
            help="Path to the clang binary.",
        )

        parser.add_argument(
            "-t",
            "--template",
            type=str,
            required=True,
            help="Path to the template file used for code generation (absolute path).",
        )

        parser.add_argument(
            "-o",
            "--output",
            type=str,
            required=True,
            help="Path to the output file where the generated code will be saved (absolute path).",
        )

        self._args = parser.parse_args()

    @property
    def InputFiles(self) -> list[str]:
        return self._args.input

    @property
    def ClangPath(self) -> str:
        return self._args.clang_path

    @property
    def TemplateFile(self) -> str:
        return self._args.template

    @property
    def OutputFile(self) -> str:
        return self._args.output

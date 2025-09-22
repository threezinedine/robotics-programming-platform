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
            required=True,
            help="Path to the input C source file.",
        )

        parser.add_argument(
            "--clang-path",
            type=str,
            required=True,
            help="Path to the clang binary.",
        )

        self._args = parser.parse_args()

    @property
    def InputFile(self) -> str:
        return self._args.input

    @property
    def ClangPath(self) -> str:
        return self._args.clang_path

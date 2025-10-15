from typing import * # type: ignore
from enum import * # type: ignore

FileHandle: TypeAlias = int
b8: TypeAlias = bool

u8: TypeAlias = int
u16: TypeAlias = int
u32: TypeAlias = int
u64: TypeAlias = int

i8: TypeAlias = int
i16: TypeAlias = int
i32: TypeAlias = int
i64: TypeAlias = int

f32: TypeAlias = float
f64: TypeAlias = float











































class FileSystem:












    @staticmethod
    def OpenFile(filePath: str, mode: u8) -> FileHandle:
        ...



    @staticmethod
    def IsFileOpen(file: FileHandle) -> b8:
        ...



    @staticmethod
    def Read(file: FileHandle) -> str:
        ...



    @staticmethod
    def Write(file: FileHandle, data: str) -> None:
        ...



    @staticmethod
    def CloseFile(file: FileHandle) -> None:
        ...













































class TestSystem:
















    @staticmethod
    def Yield() -> None:
        ...



    @staticmethod
    def Wait(milliseconds: float) -> None:
        ...



    @staticmethod
    def Log(message: str) -> None:
        ...








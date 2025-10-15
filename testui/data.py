import json
from dataclasses import dataclass, field, asdict
from dacite import from_dict


@dataclass
class Version:
    major: int = 0
    minor: int = 1
    patch: int = 0

    def __str__(self):
        return f"{self.major}.{self.minor}.{self.patch}"


@dataclass
class Application:
    version: Version = field(default_factory=Version)
    vscodePath: str = field(default="")

    def Save(self, filePath: str = "app.json") -> None:
        with open(filePath, "w", encoding="utf-8") as f:
            json.dump(asdict(self), f, indent=4)

    @staticmethod
    def From(filePath: str = "app.json") -> "Application":
        with open(filePath, "r", encoding="utf-8") as f:
            data = json.load(f)
            return from_dict(data_class=Application, data=data)

        raise FileNotFoundError(f"File {filePath} not found")

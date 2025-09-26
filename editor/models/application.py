from dataclasses import dataclass, field


@dataclass
class Version:
    major: int = field(default=1)
    minor: int = field(default=0)
    patch: int = field(default=0)


@dataclass
class Application:
    version: Version = field(default_factory=Version)

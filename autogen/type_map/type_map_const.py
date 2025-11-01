maps: list[tuple[list[str], str]] = [
    (["f32"], "float"),
    (["f64"], "double"),
    (["^std::string$", "^String$"], "str"),
    (["^void$"], "None"),
]

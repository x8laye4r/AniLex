import re
from pathlib import Path

# This file is for generating the C++ Qt Namespace enums

def parse_graphql_enums(graphql_content: str) -> dict[str, list[str]]:
    enum_pattern = re.compile(r"enum\s+(\w+)\s*\{([^}]*)\}", re.DOTALL)
    enums = {}

    for match in enum_pattern.finditer(graphql_content):
        enum_name = match.group(1)
        body = match.group(2)

        body_cleaned = re.sub(r'"[^"\\]*(?:\\.[^"\\]*)*"', "", body)
        body_cleaned = re.sub(
            r"@[a-zA-Z_]+\s*(\([^)]*\))?", "", body_cleaned
        )

        values = []
        for line in body_cleaned.splitlines():
            line = line.strip()
            if not line or line.startswith("#"):
                continue

            val_match = re.match(r"([A-Za-z_][A-Za-z0-9_]*)", line)
            if val_match:
                values.append(val_match.group(1))

        if values:
            enums[enum_name] = values

    return enums


def generate_qt_enum_header(
        enums: dict[str, list[str]],
        namespace: str = "anilist_enums",
        output_path: str = "include/generated/anilist_enums.h",
) -> None:
    lines = [
        "#pragma once",
        "#include <QObject>",
        "",
        "/*",
        " * Auto Generated File via a python script",
        " * Do not Edit manually",
        " */",
        "",
        f"namespace {namespace} {{",
        "    Q_NAMESPACE",
        "",
    ]

    for enum_name, values in enums.items():
        lines.append(f"    enum class {enum_name} {{")
        for i, val in enumerate(values):
            comma = "," if i < len(values) - 1 else ""
            lines.append(f"        {val}{comma}")
        lines.append("    };")
        lines.append(f"    Q_ENUM_NS({enum_name})")
        lines.append("")

    lines.append(f"}} // namespace {namespace}")

    out_file = Path(output_path)
    out_file.parent.mkdir(parents=True, exist_ok=True)
    out_file.write_text("\n".join(lines), encoding="utf-8")


def main():
    schema_file = Path("src/api/operations/schema.graphql")

    if not schema_file.exists():
        print("Schema not found")
        return

    content = schema_file.read_text(encoding="utf-8")
    enums = parse_graphql_enums(content)
    generate_qt_enum_header(
        enums,
        namespace="anilist_enums",
        output_path="include/generated/anilist_enums.h",
    )


if __name__ == "__main__":
    main()
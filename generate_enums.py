from pathlib import Path
from graphql import parse, EnumTypeDefinitionNode, EnumTypeExtensionNode

def parse_graphql_enums(graphql_content: str) -> dict[str, list[tuple[str, str | None]]]:
    ast = parse(graphql_content)
    enums: dict[str, list[tuple[str, str | None]]] = {}

    for definition in ast.definitions:
        if isinstance(definition, (EnumTypeDefinitionNode, EnumTypeExtensionNode)):
            enum_name = definition.name.value
            values = []

            for val in definition.values:
                val_name = val.name.value
                deprecation_reason = None

                if val.directives:
                    for directive in val.directives:
                        if directive.name.value == "deprecated":
                            deprecation_reason = "No longer supported"
                            if directive.arguments:
                                for arg in directive.arguments:
                                    if arg.name.value == "reason":
                                        deprecation_reason = arg.value.value
                                        break
                            break

                values.append((val_name, deprecation_reason))

            if enum_name in enums:
                enums[enum_name].extend(values)
            else:
                enums[enum_name] = values

    return enums

def generate_qt_enum_header(
        enums: dict[str, list[tuple[str, str | None]]],
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
        for i, (val, reason) in enumerate(values):
            comma = "," if i < len(values) - 1 else ""
            if reason:
                safe_reason = reason.replace('"', '\\"')
                lines.append(f"        {val} [[deprecated(\"{safe_reason}\")]]{comma}")
            else:
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
        namespace="graphql::enums",
        output_path="include/generated/GraphQLEnums.h",
    )

if __name__ == "__main__":
    main()
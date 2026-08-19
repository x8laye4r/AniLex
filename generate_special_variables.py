import os
import re
from pathlib import Path
from graphql import (
    build_ast_schema, parse, get_operation_ast
)
from graphql.language.ast import NonNullTypeNode, ListTypeNode
from graphql import type as gql_type
from collections import OrderedDict

SCHEMA_PATH = "src/api/special/schema.graphql"
SPECIAL_DIR = "src/api/special"
OUTPUT_DIR = "include/generated/operations/special"

def load_schema():
    with open(SCHEMA_PATH, "r", encoding="utf-8") as f:
        schema_ast = parse(f.read())
    return build_ast_schema(schema_ast)

schema = load_schema()

def resolve_type(type_node):
    is_non_null = False
    is_list = False
    inner_non_null = False

    if isinstance(type_node, NonNullTypeNode):
        is_non_null = True
        type_node = type_node.type

    if isinstance(type_node, ListTypeNode):
        is_list = True
        type_node = type_node.type
        if isinstance(type_node, NonNullTypeNode):
            inner_non_null = True
            type_node = type_node.type

    base_type_name = type_node.name.value
    return base_type_name, is_list, is_non_null, inner_non_null


def get_cpp_base_type(base_type_name):
    if base_type_name == "Int": return "int", "int"
    if base_type_name == "Float": return "double", "double"
    if base_type_name == "Boolean": return "bool", "bool"
    if base_type_name in ("String", "ID", "FuzzyDateInt"): return "QString", "string"

    t = schema.get_type(base_type_name)
    if isinstance(t, gql_type.GraphQLEnumType):
        return f"graphql::enums::{base_type_name}", "enum"
    if isinstance(t, gql_type.GraphQLScalarType):
        if base_type_name == "Json":
            return "QJsonObject", "json"
        return "QString", "string"

    return base_type_name, "object"


def wrap_cpp_type(cpp_type, is_list, is_non_null, inner_non_null):
    res = cpp_type
    if is_list:
        if not inner_non_null:
            res = f"std::optional<{res}>"
        res = f"std::vector<{res}>"
    if not is_non_null:
        res = f"std::optional<{res}>"
    return res


PLACEHOLDER_RE = re.compile(r"__replace_custom\w*__")

# replaces the placeholder with __typename so it still is a valid GraphQL query/mutation
def preprocess_graphql(content: str) -> str:
    return PLACEHOLDER_RE.sub("__typename", content)


def extract_variables(var_defs):
    variables = OrderedDict()
    if not var_defs:
        return variables

    for var in var_defs:
        var_name = var.variable.name.value
        base_type_name, is_list, is_non_null, inner_non_null = resolve_type(var.type)
        cpp_base, kind = get_cpp_base_type(base_type_name)
        cpp_full = wrap_cpp_type(cpp_base, is_list, is_non_null, inner_non_null)

        variables[var_name] = {
            "cpp_type": cpp_full,
            "cpp_base": cpp_base,
            "is_list": is_list,
            "is_non_null": is_non_null,
            "inner_non_null": inner_non_null,
            "kind": kind,
        }

    return variables

def generate_variables_header(op_name: str, variables: dict, resource_path: str) -> str:
    lines = [
        "#pragma once",
        "#include <QJsonObject>",
        "#include <QJsonArray>",
        "#include <QString>",
        "#include <optional>",
        "#include <vector>",
        '#include "generated/GraphQLEnums.h"',
        '#include "EnumConverter.hpp"',
        "",
        "namespace graphql::operations {",
        "",
        f"struct {op_name}_Variables {{",
    ]

    for vname, vinfo in variables.items():
        lines.append(f"    {vinfo['cpp_type']} {vname};")

    lines.append("")
    lines.append("    QJsonObject toJson() const {")
    lines.append("        QJsonObject json;")

    for vname, vinfo in variables.items():
        if vinfo["is_list"]:
            if not vinfo["is_non_null"]:
                lines.append(f"        if ({vname}.has_value()) {{")
                accessor_list = f"(*{vname})"
            else:
                lines.append("        {")
                accessor_list = vname

            lines.append("            QJsonArray arr;")
            lines.append(f"            for (const auto& item : {accessor_list}) {{")

            item_accessor = "item"
            if not vinfo["inner_non_null"]:
                lines.append(
                    "                if (!item.has_value()) { arr.append(QJsonValue::Null); continue; }"
                )
                item_accessor = "(*item)"

            if vinfo["kind"] == "enum":
                lines.append(
                    f"                arr.append(QString::fromStdString(enumToString({item_accessor})));"
                )
            else:
                lines.append(f"                arr.append({item_accessor});")

            lines.append("            }")
            lines.append(f'            json["{vname}"] = arr;')
            lines.append("        }")
        else:
            if not vinfo["is_non_null"]:
                lines.append(f"        if ({vname}.has_value()) {{")
                accessor = f"(*{vname})"
            else:
                lines.append("        {")
                accessor = vname

            if vinfo["kind"] == "enum":
                lines.append(
                    f'            json["{vname}"] = QString::fromStdString(enumToString({accessor}));'
                )
            else:
                lines.append(f'            json["{vname}"] = {accessor};')

            lines.append("        }")

    lines.append("        return json;")
    lines.append("    }")
    lines.append("};")
    lines.append("")
    lines.append("} // namespace graphql::operations")

    return "\n".join(lines)


def main():
    os.makedirs(OUTPUT_DIR, exist_ok=True)

    for root, dirs, files in os.walk(SPECIAL_DIR):
        for file in files:
            if not file.endswith(".graphql") or file == "schema.graphql":
                continue

            file_path = os.path.join(root, file)

            with open(file_path, "r", encoding="utf-8") as f:
                raw_content = f.read()

            content = preprocess_graphql(raw_content)

            try:
                doc = parse(content)
            except Exception as e:
                print(f"Skipping {file_path}: parse error – {e}")
                continue

            op = get_operation_ast(doc)
            if not op or not op.name:
                print(f"Skipping {file_path}: no named operation found.")
                continue

            op_name = op.name.value

            variables = extract_variables(op.variable_definitions)

            resource_path = os.path.relpath(file_path, SPECIAL_DIR)

            header_content = generate_variables_header(op_name, variables, resource_path)

            out_file = os.path.join(OUTPUT_DIR, f"{op_name}.hpp")
            with open(out_file, "w", encoding="utf-8") as f:
                f.write(header_content)

            print(f"Generated {out_file}")


if __name__ == "__main__":
    main()

import os
from pathlib import Path
from graphql import (
    build_ast_schema, parse, get_operation_ast, type as gql_type
)
from graphql.language.ast import NonNullTypeNode, ListTypeNode
from collections import OrderedDict

SCHEMA_PATH = "src/api/operations/schema.graphql"
OPERATIONS_DIR = "src/api/operations"
OUTPUT_DIR = "include/generated/operations"

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

class StructGenerator:
    def __init__(self, op_name, ast, fragments):
        self.op_name = op_name
        self.ast = ast
        self.fragments = fragments
        self.structs = OrderedDict()
        self.variables = OrderedDict()
        
    def _get_field_info(self, selection, parent_type_name):
        if selection.kind == 'field':
            name = selection.name.value
            alias = selection.alias.value if selection.alias else name
            
            parent_type = schema.get_type(parent_type_name)
            if isinstance(parent_type, (gql_type.GraphQLObjectType, gql_type.GraphQLInterfaceType, gql_type.GraphQLUnionType)):
                if isinstance(parent_type, gql_type.GraphQLUnionType):
                    if name == "__typename":
                        return name, alias, gql_type.GraphQLNonNull(gql_type.GraphQLString), None
                    return None, None, None, None
                field_def = parent_type.fields.get(name)
            else:
                field_def = None
                
            if field_def:
                return name, alias, field_def.type, selection.selection_set
            elif name == "__typename":
                return name, alias, gql_type.GraphQLNonNull(gql_type.GraphQLString), None
        return None, None, None, None

    def _resolve_schema_type(self, t):
        is_non_null = False
        is_list = False
        inner_non_null = False
        if isinstance(t, gql_type.GraphQLNonNull):
            is_non_null = True
            t = t.of_type
        if isinstance(t, gql_type.GraphQLList):
            is_list = True
            t = t.of_type
            if isinstance(t, gql_type.GraphQLNonNull):
                inner_non_null = True
                t = t.of_type
        return t.name, is_list, is_non_null, inner_non_null

    def gather_fields(self, selection_set, parent_type_name, out_fields):
        for sel in selection_set.selections:
            if sel.kind == 'field':
                name, alias, schema_type, sub_sel = self._get_field_info(sel, parent_type_name)
                if not name: continue
                
                base_type_name, is_list, is_non_null, inner_non_null = self._resolve_schema_type(schema_type)
                cpp_base, kind = get_cpp_base_type(base_type_name)
                
                if sub_sel:
                    struct_name = f"{parent_type_name}_{alias.capitalize()}"
                    self.generate_struct(struct_name, base_type_name, sub_sel)
                    cpp_base = struct_name
                    kind = "object"
                    
                cpp_full = wrap_cpp_type(cpp_base, is_list, is_non_null, inner_non_null)
                out_fields[alias] = {
                    "cpp_type": cpp_full,
                    "cpp_base": cpp_base,
                    "is_list": is_list,
                    "is_non_null": is_non_null,
                    "inner_non_null": inner_non_null,
                    "kind": kind
                }
            elif sel.kind == 'inline_fragment':
                type_cond = sel.type_condition.name.value
                self.gather_fields(sel.selection_set, type_cond, out_fields)
            elif sel.kind == 'fragment_spread':
                frag_name = sel.name.value
                frag = self.fragments[frag_name]
                self.gather_fields(frag.selection_set, frag.type_condition.name.value, out_fields)

    def generate_struct(self, struct_name, graphql_type_name, selection_set):
        if struct_name in self.structs:
            fields = self.structs[struct_name]
        else:
            fields = OrderedDict()
            self.structs[struct_name] = fields
            
        self.gather_fields(selection_set, graphql_type_name, fields)
        
    def generate_variables(self, var_defs):
        if not var_defs: return
        for var in var_defs:
            var_name = var.variable.name.value
            base_type_name, is_list, is_non_null, inner_non_null = resolve_type(var.type)
            cpp_base, kind = get_cpp_base_type(base_type_name)
            cpp_full = wrap_cpp_type(cpp_base, is_list, is_non_null, inner_non_null)
            
            self.variables[var_name] = {
                "cpp_type": cpp_full,
                "cpp_base": cpp_base,
                "is_list": is_list,
                "is_non_null": is_non_null,
                "inner_non_null": inner_non_null,
                "kind": kind
            }

def generate_header(op_name, gen: StructGenerator, resource_path):
    lines = [
        "#pragma once",
        "#include <QJsonObject>",
        "#include <QJsonArray>",
        "#include <QString>",
        "#include <expected>",
        "#include <optional>",
        "#include <vector>",
        '#include "api/GraphQLRequest.h"',
        '#include "generated/GraphQLEnums.h"',
        '#include "EnumConverter.hpp"',
        "",
        "namespace graphql::operations {",
        "",
        f"struct {op_name}_Variables {{"
    ]
    
    for vname, vinfo in gen.variables.items():
        lines.append(f"    {vinfo['cpp_type']} {vname};")
        
    lines.append("")
    lines.append("    QJsonObject toJson() const {")
    lines.append("        QJsonObject json;")
    for vname, vinfo in gen.variables.items():
        if vinfo['is_list']:
            if not vinfo['is_non_null']:
                lines.append(f"        if ({vname}.has_value()) {{")
                accessor_list = f"(*{vname})"
            else:
                lines.append("        {")
                accessor_list = vname
                
            lines.append("            QJsonArray arr;")
            lines.append(f"            for (const auto& item : {accessor_list}) {{")
            
            item_accessor = "item"
            if not vinfo['inner_non_null']:
                lines.append("                if (!item.has_value()) { arr.append(QJsonValue::Null); continue; }")
                item_accessor = "(*item)"
                
            if vinfo['kind'] == 'enum':
                lines.append(f"                arr.append(QString::fromStdString(enumToString({item_accessor})));")
            else:
                lines.append(f"                arr.append({item_accessor});")
            lines.append("            }")
            lines.append(f"            json[\"{vname}\"] = arr;")
            lines.append("        }")
        else:
            if not vinfo['is_non_null']:
                lines.append(f"        if ({vname}.has_value()) {{")
                accessor = f"(*{vname})"
            else:
                lines.append("        {")
                accessor = vname
                
            if vinfo['kind'] == 'enum':
                lines.append(f"            json[\"{vname}\"] = QString::fromStdString(enumToString({accessor}));")
            else:
                lines.append(f"            json[\"{vname}\"] = {accessor};")
            lines.append("        }")
    lines.append("        return json;")
    lines.append("    }")
    lines.append("};")
    lines.append("")
    
    lines.append(f"struct {op_name}_Result {{")
    
    ordered_structs = []
    visited = set()
    visiting = set()
    
    def visit_struct(sname):
        if sname in visited: return
        if sname in visiting: return
        visiting.add(sname)
        
        if sname in gen.structs:
            for fname, finfo in gen.structs[sname].items():
                if finfo['kind'] == 'object':
                    dep_name = finfo['cpp_base']
                    visit_struct(dep_name)
                    
        visiting.remove(sname)
        visited.add(sname)
        ordered_structs.append(sname)

    for sname in gen.structs:
        visit_struct(sname)
        
    for sname in ordered_structs:
        if sname == "Result": continue
        fields = gen.structs[sname]
        lines.append(f"    struct {sname} {{")
        for fname, finfo in fields.items():
            lines.append(f"        {finfo['cpp_type']} {fname};")
            
        lines.append("")
        lines.append(f"        static std::expected<{sname}, QString> fromJson(const QJsonObject& json) {{")
        lines.append(f"            {sname} res;")
        for fname, finfo in fields.items():
            lines.append(f"            if (json.contains(\"{fname}\") && !json[\"{fname}\"].isNull()) {{")
            if finfo['is_list']:
                inner_vec_type = finfo['cpp_base']
                if not finfo['inner_non_null']:
                    inner_vec_type = f"std::optional<{inner_vec_type}>"
                inner_vec_type = f"std::vector<{inner_vec_type}>"
                
                lines.append("                QJsonArray arr = json[\"{fname}\"].toArray();".format(fname=fname))
                lines.append(f"                {inner_vec_type} vec;")
                lines.append("                for (const auto& item : arr) {")
                if not finfo['inner_non_null']:
                    lines.append(f"                    if (item.isNull()) {{ vec.push_back(std::nullopt); continue; }}")
                
                if finfo['kind'] == 'object':
                    lines.append(f"                    auto parsed = {finfo['cpp_base']}::fromJson(item.toObject());")
                    lines.append(f"                    if (parsed) vec.push_back(*parsed);")
                elif finfo['kind'] == 'enum':
                    lines.append(f"                    vec.push_back(stringToEnum<{finfo['cpp_base']}>(item.toString().toStdString()));")
                elif finfo['kind'] == 'int':
                    lines.append(f"                    vec.push_back(item.toInt());")
                elif finfo['kind'] == 'double':
                    lines.append(f"                    vec.push_back(item.toDouble());")
                elif finfo['kind'] == 'bool':
                    lines.append(f"                    vec.push_back(item.toBool());")
                else:
                    lines.append(f"                    vec.push_back(item.toString());")
                lines.append("                }")
                lines.append(f"                res.{fname} = vec;")
            else:
                if finfo['kind'] == 'object':
                    lines.append(f"                auto parsed = {finfo['cpp_base']}::fromJson(json[\"{fname}\"].toObject());")
                    lines.append(f"                if (parsed) res.{fname} = *parsed;")
                elif finfo['kind'] == 'enum':
                    lines.append(f"                res.{fname} = stringToEnum<{finfo['cpp_base']}>(json[\"{fname}\"].toString().toStdString());")
                elif finfo['kind'] == 'int':
                    lines.append(f"                res.{fname} = json[\"{fname}\"].toInt();")
                elif finfo['kind'] == 'double':
                    lines.append(f"                res.{fname} = json[\"{fname}\"].toDouble();")
                elif finfo['kind'] == 'bool':
                    lines.append(f"                res.{fname} = json[\"{fname}\"].toBool();")
                elif finfo['kind'] == 'json':
                    lines.append(f"                res.{fname} = json[\"{fname}\"].toObject();")
                else:
                    lines.append(f"                res.{fname} = json[\"{fname}\"].toString();")
            lines.append("            }")
        lines.append("            return res;")
        lines.append("        }")
        lines.append("    };")
        lines.append("")
        
    result_fields = gen.structs.get("Result", {})
    for fname, finfo in result_fields.items():
        lines.append(f"    {finfo['cpp_type']} {fname};")
        
    lines.append("")
    lines.append(f"    static std::expected<{op_name}_Result, QString> fromJson(const QJsonObject& json) {{")
    lines.append(f"        {op_name}_Result res;")
    for fname, finfo in result_fields.items():
        lines.append(f"        if (json.contains(\"{fname}\") && !json[\"{fname}\"].isNull()) {{")
        if finfo['is_list']:
            inner_vec_type = finfo['cpp_base']
            if not finfo['inner_non_null']:
                inner_vec_type = f"std::optional<{inner_vec_type}>"
            inner_vec_type = f"std::vector<{inner_vec_type}>"
            
            lines.append("            QJsonArray arr = json[\"{fname}\"].toArray();".format(fname=fname))
            lines.append(f"            {inner_vec_type} vec;")
            lines.append("            for (const auto& item : arr) {")
            if not finfo['inner_non_null']:
                lines.append(f"                if (item.isNull()) {{ vec.push_back(std::nullopt); continue; }}")
            
            if finfo['kind'] == 'object':
                lines.append(f"                auto parsed = {finfo['cpp_base']}::fromJson(item.toObject());")
                lines.append(f"                if (parsed) vec.push_back(*parsed);")
            elif finfo['kind'] == 'enum':
                lines.append(f"                vec.push_back(stringToEnum<{finfo['cpp_base']}>(item.toString().toStdString()));")
            elif finfo['kind'] == 'int':
                lines.append(f"                vec.push_back(item.toInt());")
            elif finfo['kind'] == 'double':
                lines.append(f"                vec.push_back(item.toDouble());")
            elif finfo['kind'] == 'bool':
                lines.append(f"                vec.push_back(item.toBool());")
            else:
                lines.append(f"                vec.push_back(item.toString());")
            lines.append("            }")
            lines.append(f"            res.{fname} = vec;")
        else:
            if finfo['kind'] == 'object':
                lines.append(f"            auto parsed = {finfo['cpp_base']}::fromJson(json[\"{fname}\"].toObject());")
                lines.append(f"            if (parsed) res.{fname} = *parsed;")
            elif finfo['kind'] == 'enum':
                lines.append(f"            res.{fname} = stringToEnum<{finfo['cpp_base']}>(json[\"{fname}\"].toString().toStdString());")
            elif finfo['kind'] == 'int':
                lines.append(f"            res.{fname} = json[\"{fname}\"].toInt();")
            elif finfo['kind'] == 'double':
                lines.append(f"            res.{fname} = json[\"{fname}\"].toDouble();")
            elif finfo['kind'] == 'bool':
                lines.append(f"            res.{fname} = json[\"{fname}\"].toBool();")
            elif finfo['kind'] == 'json':
                lines.append(f"            res.{fname} = json[\"{fname}\"].toObject();")
            else:
                lines.append(f"            res.{fname} = json[\"{fname}\"].toString();")
        lines.append("        }")
    lines.append("        return res;")
    lines.append("    }")
    lines.append("};")
    lines.append("")
    
    lines.append(f"using {op_name} = GraphQLOperation<")
    lines.append(f"    {op_name}_Variables,")
    lines.append(f"    {op_name}_Result,")
    formatted_path = ":/graphql/" + resource_path.replace("\\", "/")
    lines.append(f"    \"{formatted_path}\"")
    lines.append(">;")
    lines.append("")
    lines.append("} // namespace graphql::operations")
    
    return "\n".join(lines)

def main():
    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR, exist_ok=True)
        
    for root, dirs, files in os.walk(OPERATIONS_DIR):
        for file in files:
            if file.endswith(".graphql") and file != "schema.graphql":
                file_path = os.path.join(root, file)
                resource_path = os.path.relpath(file_path, OPERATIONS_DIR)
                
                with open(file_path, "r", encoding="utf-8") as f:
                    content = f.read()
                    
                doc = parse(content)
                op = get_operation_ast(doc)
                if not op or not op.name:
                    print(f"Skipping {file_path}: No named operation found.")
                    continue
                    
                op_name = op.name.value
                fragments = {d.name.value: d for d in doc.definitions if d.kind == 'fragment_definition'}
                
                root_type = "Mutation" if op.operation.name == 'MUTATION' else "Query"
                gen = StructGenerator(op_name, op, fragments)
                gen.generate_variables(op.variable_definitions)
                gen.generate_struct("Result", root_type, op.selection_set)
                
                header_content = generate_header(op_name, gen, resource_path)
                
                out_file = os.path.join(OUTPUT_DIR, f"{op_name}.hpp")
                with open(out_file, "w", encoding="utf-8") as f:
                    f.write(header_content)

if __name__ == "__main__":
    main()

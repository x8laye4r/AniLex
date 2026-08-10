#pragma once
#include <QJsonObject>
#include <QString>
#include <expected>
#include <string_view>

namespace GraphQLOperation {

  template <typename T>
  concept JsonParsable = requires(const QJsonObject& json) {
      { T::fromJson(json) } -> std::same_as<std::expected<T, QString>>;
  };

  template <typename T>
  concept JsonSerializable = requires(const T& vars) {
    { vars.toJson() } -> std::same_as<QJsonObject>;
  };

  template <JsonSerializable Vars, JsonParsable Result, std::string_view Path>
  struct GraphQLOperation {
    using Variables = Vars;
    using ResultType = Result;
    static constexpr std::string_view resourcePath = Path;

    static std::expected<Result, QString> parse(const QJsonObject& data) {
      return Result::fromJson(data);
    }
  };

} // namespace GraphQLOperations
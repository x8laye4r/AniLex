#pragma once
#include <QMetaEnum>
#include <QString>
#include <QVariant>
#include <QDebug>

/**
 * @class EnumConverter
 * @brief this class is used for converting Q_ENUMS into strings and converting them back into enums
 *
 * This class is using the QMetaEnum class to convert any enum which is defined with the Q_ENUM or Q_ENUM_NS macro to
 * strings or back to the original.
 * It can either convert any enum to an uppercase string or
 * convert it back to the original enum structure.
 *
 * @note returns an uppercase QString and only works if the string is the same as the enum is written
 */
class EnumConverter {
public:
    /**
     * @brief Converts any given Qt-Enum to an uppercase QString using the QMetaEnum class
     * @tparam T any enum which is defined with the Qt-Enums macros
     * @param value the enum which should be converted to a string
     * @return uppercase QString
     * @warning won't work with C++ enums
     */
    template<typename T>
    static QString toString(const T &value) {
        QMetaEnum metaEnum = QMetaEnum::fromType<T>();
        if (!metaEnum.isValid()) {
            qWarning() << "EnumConverter::toString() - Not a valid Q_ENUM";
            return QString();
        }
        return QString::fromUtf8(metaEnum.valueToKey(static_cast<int>(value))).toUpper();
    }

    /**
     * @brief Converts any given string to the given enum
     * @tparam T any Qt-Enum which the string should be converted to
     * @param name the string which should be converted to a Qt-Enum
     * @param ok optional parameter to know if the conversion was successfully
     * @return @tparam T
     * @note the given string must be written the same as the defined enum
     * @warning won't work with normal C++ enums
     */
    template<typename T>
    static T convertTo(const QString &name, bool *ok = nullptr) {
        QMetaEnum metaEnum = QMetaEnum::fromType<T>();
        if (!metaEnum.isValid()) {
            qWarning() << "EnumConverter::convertTo() - Not a valid QMetaEnum";
            return static_cast<T>(0);
        }
        int value = metaEnum.keyToValue(name.toUtf8().constData(), ok);
        return static_cast<T>(value == -1 ? 0 : value);
    }
};

#pragma once
#include <QString>

/**
 * Metadata for the tab.
 * @note @ref icon can be uninitialized
 */
typedef struct TabMeta {
    QString name;
    QString icon;

    bool operator==(const TabMeta &other) const {
        return name == other.name and icon == other.icon;
    }
}TabMeta;
// Rule evaluation interface.
#pragma once

#include <QVariant>

namespace RuleEngine {
QVariantMap evaluate(const QVariantList& rules,
                     const QString& formId,
                     const QVariantMap& formData,
                     const QVariantMap& resultMap);
}

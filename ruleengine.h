// Rule evaluation interface.
#pragma once

#include <QVariant>

#include "rule.h"

namespace RuleEngine {
QVariantMap evaluate(const QList<Rule*>& rules,
                     const QString& formId,
                     const QVariantMap& formData,
                     const QVariantMap& resultMap);
}

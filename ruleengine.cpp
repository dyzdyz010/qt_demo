// Rule evaluation helpers and core engine.
#include "ruleengine.h"

#include <QString>
#include <QMetaType>

namespace {
QVariant toNumber(const QVariant& value)
{
    if (!value.isValid() || value.isNull()) {
        return {};
    }
    if (value.typeId() == QMetaType::Double || value.typeId() == QMetaType::Int || value.typeId() == QMetaType::LongLong) {
        return value.toDouble();
    }
    const auto text = value.toString().trimmed();
    if (text.isEmpty()) {
        return {};
    }
    bool ok = false;
    const auto num = text.toDouble(&ok);
    return ok ? QVariant(num) : QVariant();
}

QString toText(const QVariant& value)
{
    if (!value.isValid() || value.isNull()) {
        return {};
    }
    return value.toString();
}

QVariantList asArray(const QVariant& value)
{
    if (!value.isValid() || value.isNull()) {
        return {};
    }
    if (value.typeId() == QMetaType::QVariantList) {
        return value.toList();
    }
    if (value.typeId() == QMetaType::QString) {
        const auto trimmed = value.toString().trimmed();
        if (trimmed.isEmpty()) {
            return {};
        }
        if (trimmed.contains(",")) {
            QVariantList out;
            const auto parts = trimmed.split(",");
            for (const auto& part : parts) {
                const auto item = part.trimmed();
                if (!item.isEmpty()) {
                    out.append(item);
                }
            }
            return out;
        }
        return { trimmed };
    }
    return { value };
}

bool isEmptyValue(const QVariant& value)
{
    if (!value.isValid() || value.isNull()) {
        return false;
    }
    if (value.typeId() == QMetaType::QString) {
        return value.toString().isEmpty();
    }
    if (value.canConvert<QString>()) {
        return value.toString().isEmpty();
    }
    return false;
}

bool equalsValue(const QVariant& left, const QVariant& right)
{
    const auto leftNum = toNumber(left);
    const auto rightNum = toNumber(right);
    if (leftNum.isValid() && rightNum.isValid()) {
        return leftNum.toDouble() == rightNum.toDouble();
    }
    return toText(left) == toText(right);
}

bool compareNumber(const QVariant& left, const QVariant& right, const QString& op)
{
    const auto leftNum = toNumber(left);
    const auto rightNum = toNumber(right);
    if (!leftNum.isValid() || !rightNum.isValid()) {
        return false;
    }
    if (op == "$gt") {
        return leftNum.toDouble() > rightNum.toDouble();
    }
    if (op == "$lt") {
        return leftNum.toDouble() < rightNum.toDouble();
    }
    return false;
}

bool compareLength(const QVariant& value, const QVariant& expected, const QString& op)
{
    const auto lengthValue = toText(value).length();
    const auto expectedNum = toNumber(expected);
    if (!expectedNum.isValid()) {
        return false;
    }
    const auto expectedLength = expectedNum.toDouble();
    if (op == "$len_gt") {
        return lengthValue > expectedLength;
    }
    if (op == "$len_lt") {
        return lengthValue < expectedLength;
    }
    if (op == "$len_eq") {
        return lengthValue == expectedLength;
    }
    if (op == "$len_ne") {
        return lengthValue != expectedLength;
    }
    return false;
}

bool matchesCondition(const QVariantMap& formData, const QVariantMap& condition)
{
    const auto key = condition.value("key").toString();
    if (!formData.contains(key)) {
        return false;
    }
    const auto fieldValue = formData.value(key);
    const auto expected = condition.value("value");
    const auto op = condition.value("op").toString();

    if (op == "$eq") {
        return equalsValue(fieldValue, expected);
    }
    if (op == "$ne") {
        return !equalsValue(fieldValue, expected);
    }
    if (op == "$in") {
        const auto list = asArray(expected);
        for (const auto& item : list) {
            if (equalsValue(fieldValue, item)) {
                return true;
            }
        }
        return false;
    }
    if (op == "$nin") {
        const auto list = asArray(expected);
        for (const auto& item : list) {
            if (equalsValue(fieldValue, item)) {
                return false;
            }
        }
        return true;
    }
    if (op == "$gt" || op == "$lt") {
        return compareNumber(fieldValue, expected, op);
    }
    if (op == "$contain") {
        return toText(fieldValue).contains(toText(expected));
    }
    if (op == "$not_contain") {
        return !toText(fieldValue).contains(toText(expected));
    }
    if (op == "$empty") {
        return isEmptyValue(fieldValue);
    }
    if (op == "$len_gt" || op == "$len_lt" || op == "$len_eq" || op == "$len_ne") {
        return compareLength(fieldValue, expected, op);
    }
    if (op == "$start_with") {
        return toText(fieldValue).startsWith(toText(expected));
    }
    return false;
}

bool matchesRule(const QVariantMap& rule, const QVariantMap& formData)
{
    const auto enabledValue = rule.value("$enabled");
    const bool enabled = enabledValue.isValid() ? enabledValue.toBool() : rule.value("enabled").toBool();
    if (!enabled) {
        return false;
    }
    const auto conditionsValue = rule.value("$and");
    const auto conditions = conditionsValue.isValid() ? conditionsValue.toList() : rule.value("and").toList();
    if (conditions.isEmpty()) {
        return false;
    }
    for (const auto& item : conditions) {
        if (!matchesCondition(formData, item.toMap())) {
            return false;
        }
    }
    return true;
}
} // namespace

namespace RuleEngine {
QVariantMap evaluate(const QVariantList& rules,
                     const QString& formId,
                     const QVariantMap& formData,
                     const QVariantMap& resultMap)
{
    QVariantList matched;
    for (const auto& item : rules) {
        const auto rule = item.toMap();
        // Accept $-prefixed and legacy keys for compatibility.
        auto ruleFormId = rule.value("$form_id").toString();
        if (ruleFormId.isEmpty()) {
            ruleFormId = rule.value("formId").toString();
        }
        if (ruleFormId != formId) {
            continue;
        }
        if (matchesRule(rule, formData)) {
            matched.append(rule);
        }
    }

    QVariantList codes;
    QVariantList labels;
    for (const auto& item : matched) {
        const auto rule = item.toMap();
        auto code = rule.value("$result").toString();
        if (code.isEmpty()) {
            code = rule.value("result").toString();
        }
        codes.append(code);
        const auto label = resultMap.value(code, QStringLiteral("未知结果")).toString();
        labels.append(code.isEmpty() ? label : code + " - " + label);
    }

    return QVariantMap{
        { "matchedRules", matched },
        { "resultCodes", codes },
        { "resultLabels", labels }
    };
}
} // namespace RuleEngine

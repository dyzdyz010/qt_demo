// RuleController implementation.
#include "rulecontroller.h"

#include "formmodel.h"
#include "ruleengine.h"
#include "rulemodel.h"

RuleController::RuleController(QObject* parent)
    : QObject(parent)
{
}

RuleModel* RuleController::model() const
{
    return m_model;
}

void RuleController::setModel(RuleModel* model)
{
    if (m_model == model) {
        return;
    }
    m_model = model;
    emit modelChanged();
}

FormModel* RuleController::formModel() const
{
    return m_formModel;
}

void RuleController::setFormModel(FormModel* model)
{
    if (m_formModel == model) {
        return;
    }
    m_formModel = model;
    emit formModelChanged();
}

QVariantMap RuleController::evaluate(const QString& formId, const QVariantMap& formData) const
{
    if (!m_model) {
        return {};
    }
    QList<Rule*> rulesCopy;
    const QList<Rule*>& modelRules = m_model->rules();
    rulesCopy.reserve(modelRules.size());
    for (const auto& rule : modelRules) {
        rulesCopy.append(rule);
    }
    return RuleEngine::evaluate(rulesCopy, formId, formData, m_model->resultMap());
}

bool RuleController::isListOp(const QString& op) const
{
    return op == "$in" || op == "$nin";
}

QString RuleController::formatValue(const QVariant& value) const
{
    if (value.typeId() == QMetaType::QVariantList) {
        const auto list = value.toList();
        QStringList parts;
        parts.reserve(list.size());
        for (const auto& item : list) {
            parts.append(item.toString());
        }
        return parts.join(", ");
    }
    if (!value.isValid() || value.isNull()) {
        return {};
    }
    return value.toString();
}

QVariant RuleController::parseValue(const QString& op, const QString& text) const
{
    if (op == "$empty") {
        return QString();
    }
    if (isListOp(op)) {
        const auto parts = splitListText(text);
        QVariantList out;
        out.reserve(parts.size());
        for (const auto& part : parts) {
            out.append(part);
        }
        return out;
    }
    const auto trimmed = text.trimmed();
    if (op == "$gt" || op == "$lt" || op.startsWith("$len_")) {
        bool ok = false;
        const auto num = trimmed.toDouble(&ok);
        return ok ? QVariant(num) : QVariant(trimmed);
    }
    return trimmed;
}

QStringList RuleController::splitListText(const QString& text) const
{
    if (text.trimmed().isEmpty()) {
        return {};
    }
    QStringList out;
    const auto parts = text.split(",");
    for (const auto& part : parts) {
        const auto item = part.trimmed();
        if (!item.isEmpty()) {
            out.append(item);
        }
    }
    return out;
}

QString RuleController::resultLabel(const QString& code) const
{
    if (!m_model) {
        return QStringLiteral("未知结果");
    }
    const auto label = m_model->resultMap().value(code).toString();
    return label.isEmpty() ? QStringLiteral("未知结果") : label;
}

QString RuleController::ruleSummary(const QVariantMap& rule) const
{
    const auto id = rule.value("id").toString();
    auto formId = rule.value("$form_id").toString();
    if (formId.isEmpty()) {
        formId = rule.value("formId").toString();
    }
    auto result = rule.value("$result").toString();
    if (result.isEmpty()) {
        result = rule.value("result").toString();
    }
    const auto formTitle = m_formModel ? m_formModel->formTitle(formId) : QStringLiteral("未知表单");
    return QStringLiteral("编号：") + id + QStringLiteral(" | 表单：") + formTitle + QStringLiteral(" | 结果：") + result +
           QStringLiteral(" - ") + resultLabel(result);
}

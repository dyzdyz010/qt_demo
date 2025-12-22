// FormController implementation.
#include "formcontroller.h"

#include "rulecontroller.h"

FormController::FormController(QObject* parent)
    : QObject(parent)
{
    resetForm();
}

QString FormController::formId() const
{
    return m_formId;
}

void FormController::setFormId(const QString& formId)
{
    if (m_formId == formId) {
        return;
    }
    m_formId = formId;
    emit formIdChanged();
    resetForm();
}

QVariantList FormController::fields() const
{
    return m_fields;
}

void FormController::setFields(const QVariantList& fields)
{
    if (m_fields == fields) {
        return;
    }
    m_fields = fields;
    emit fieldsChanged();
    resetForm();
}

QVariantMap FormController::formData() const
{
    return m_formData;
}

QVariantMap FormController::evaluation() const
{
    return m_evaluation;
}

RuleController* FormController::ruleController() const
{
    return m_ruleController;
}

void FormController::setRuleController(RuleController* controller)
{
    if (m_ruleController == controller) {
        return;
    }
    m_ruleController = controller;
    emit ruleControllerChanged();
}

void FormController::setFieldValue(const QString& key, const QVariant& value)
{
    m_formData.insert(key, value);
    emit formDataChanged();
}

void FormController::setFieldText(const QString& key, const QString& text)
{
    const auto field = fieldForKey(key);
    const auto type = field.value("type").toString();
    if (type == "number") {
        const auto trimmed = text.trimmed();
        if (trimmed.isEmpty()) {
            setFieldValue(key, "");
            return;
        }
        bool ok = false;
        const auto num = trimmed.toDouble(&ok);
        setFieldValue(key, ok ? QVariant(num) : QVariant(trimmed));
        return;
    }
    setFieldValue(key, text);
}

int FormController::optionIndex(const QVariantList& options, const QVariant& value) const
{
    if (options.isEmpty()) {
        return -1;
    }
    const auto target = value.toString();
    for (int i = 0; i < options.size(); ++i) {
        if (options.at(i).toString() == target) {
            return i;
        }
    }
    return 0;
}

void FormController::resetForm()
{
    QVariantMap next;
    for (const auto& fieldValue : m_fields) {
        const auto field = fieldValue.toMap();
        const auto key = field.value("key").toString();
        const auto type = field.value("type").toString();
        if (type == "select") {
            const auto options = field.value("options").toList();
            next.insert(key, options.isEmpty() ? QVariant("") : options.first());
        } else if (type == "bool") {
            next.insert(key, false);
        } else if (field.contains("default")) {
            next.insert(key, field.value("default"));
        } else {
            next.insert(key, "");
        }
    }
    m_formData = next;
    emit formDataChanged();
    m_evaluation = QVariantMap {
        { "matchedRules", QVariantList {} },
        { "resultCodes", QVariantList {} },
        { "resultLabels", QVariantList {} }
    };
    emit evaluationChanged();
}

void FormController::validate()
{
    if (!m_ruleController) {
        return;
    }
    m_evaluation = m_ruleController->evaluate(m_formId, m_formData);
    emit evaluationChanged();
}

QVariantMap FormController::fieldForKey(const QString& key) const
{
    for (const auto& fieldValue : m_fields) {
        const auto field = fieldValue.toMap();
        if (field.value("key").toString() == key) {
            return field;
        }
    }
    return {};
}

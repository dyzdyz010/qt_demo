// RuleEditorController implementation.
#include "ruleeditorcontroller.h"

#include "formmodel.h"
#include "rulecontroller.h"
#include "rulemodel.h"

#include <QJsonDocument>
#include <QJsonObject>

RuleEditorController::RuleEditorController(QObject* parent)
    : QObject(parent)
{
}

RuleModel* RuleEditorController::model() const
{
    return m_model;
}

void RuleEditorController::setModel(RuleModel* model)
{
    if (m_model == model) {
        return;
    }
    if (m_model && m_rulesChangedConnection) {
        QObject::disconnect(m_rulesChangedConnection);
    }
    m_model = model;
    if (m_model) {
        m_rulesChangedConnection = connect(m_model, &RuleModel::rulesChanged, this, &RuleEditorController::ensureSelection);
    }
    emit modelChanged();
    ensureSelection();
}

FormModel* RuleEditorController::formModel() const
{
    return m_formModel;
}

void RuleEditorController::setFormModel(FormModel* model)
{
    if (m_formModel == model) {
        return;
    }
    m_formModel = model;
    emit formModelChanged();
    normalizeConditionsForForm(m_ruleFormId);
}

RuleController* RuleEditorController::controller() const
{
    return m_controller;
}

void RuleEditorController::setController(RuleController* controller)
{
    if (m_controller == controller) {
        return;
    }
    m_controller = controller;
    emit controllerChanged();
}

int RuleEditorController::selectedIndex() const
{
    return m_selectedIndex;
}

void RuleEditorController::setSelectedIndex(int index)
{
    if (m_selectedIndex == index) {
        return;
    }
    m_selectedIndex = index;
    emit selectedIndexChanged();
    loadRule(index);
}

QString RuleEditorController::ruleId() const
{
    return m_ruleId;
}

QString RuleEditorController::ruleFormId() const
{
    return m_ruleFormId;
}

void RuleEditorController::setRuleFormId(const QString& formId)
{
    if (m_ruleFormId == formId) {
        return;
    }
    m_ruleFormId = formId;
    emit ruleFormIdChanged();
    normalizeConditionsForForm(m_ruleFormId);
}

QString RuleEditorController::ruleResult() const
{
    return m_ruleResult;
}

void RuleEditorController::setRuleResult(const QString& result)
{
    if (m_ruleResult == result) {
        return;
    }
    m_ruleResult = result;
    emit ruleResultChanged();
}

bool RuleEditorController::ruleEnabled() const
{
    return m_ruleEnabled;
}

void RuleEditorController::setRuleEnabled(bool enabled)
{
    if (m_ruleEnabled == enabled) {
        return;
    }
    m_ruleEnabled = enabled;
    emit ruleEnabledChanged();
}

ConditionModel* RuleEditorController::conditionsModel()
{
    return &m_conditions;
}

void RuleEditorController::addRule()
{
    if (!m_model || !m_formModel) {
        return;
    }
    const auto forms = m_formModel->forms();
    QString formId;
    if (!forms.isEmpty()) {
        formId = forms.first().toMap().value("id").toString();
    }
    const auto fields = m_formModel->fieldsForForm(formId);
    QString firstKey;
    if (!fields.isEmpty()) {
        firstKey = fields.first().toMap().value("key").toString();
    }
    const auto rule = m_model->createDefaultRule(formId, firstKey);
    m_model->addRule(rule);
    m_selectedIndex = m_model->rules().size() - 1;
    emit selectedIndexChanged();
    loadRule(m_selectedIndex);
}

void RuleEditorController::removeRule()
{
    if (!m_model || m_selectedIndex < 0 || m_selectedIndex >= m_model->rules().size()) {
        return;
    }
    m_model->removeRule(m_selectedIndex);
    ensureSelection();
}

void RuleEditorController::addCondition()
{
    const auto fieldKeys = fieldKeysForForm(m_ruleFormId);
    const auto fieldKey = fieldKeys.isEmpty() ? QString() : fieldKeys.first();
    m_conditions.addCondition(fieldKey, "$eq", "");
}

void RuleEditorController::removeCondition(int row)
{
    m_conditions.removeCondition(row);
}

void RuleEditorController::saveRule()
{
    if (!m_model || m_selectedIndex < 0 || m_selectedIndex >= m_model->rules().size()) {
        return;
    }
    m_model->updateRule(m_selectedIndex, buildRule());
}

QString RuleEditorController::previewJson() const
{
    const auto rule = buildRule();
    if (rule.isEmpty()) {
        return QStringLiteral("未选择规则");
    }
    const auto doc = QJsonDocument::fromVariant(rule);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Indented));
}

void RuleEditorController::loadRule(int index)
{
    if (!m_model || index < 0 || index >= m_model->rules().size()) {
        m_ruleId.clear();
        m_ruleFormId.clear();
        m_ruleResult = QStringLiteral("01");
        m_ruleEnabled = true;
        emit ruleIdChanged();
        emit ruleFormIdChanged();
        emit ruleResultChanged();
        emit ruleEnabledChanged();
        m_conditions.setItems({});
        return;
    }

    const auto rule = m_model->rules().at(index).toMap();
    m_ruleId = rule.value("id").toString();
    m_ruleFormId = rule.value("$form_id").toString();
    if (m_ruleFormId.isEmpty()) {
        m_ruleFormId = rule.value("formId").toString();
    }
    m_ruleResult = rule.value("$result").toString();
    if (m_ruleResult.isEmpty()) {
        m_ruleResult = rule.value("result").toString();
    }
    const auto enabledValue = rule.value("$enabled");
    m_ruleEnabled = enabledValue.isValid() ? enabledValue.toBool() : rule.value("enabled").toBool();
    emit ruleIdChanged();
    emit ruleFormIdChanged();
    emit ruleResultChanged();
    emit ruleEnabledChanged();

    QVector<ConditionModel::Item> items;
    const auto conditionsValue = rule.value("$and");
    const auto conditions = conditionsValue.isValid() ? conditionsValue.toList() : rule.value("and").toList();
    items.reserve(conditions.size());
    for (const auto& item : conditions) {
        const auto map = item.toMap();
        const auto valueText = m_controller ? m_controller->formatValue(map.value("value")) : map.value("value").toString();
        items.push_back({ map.value("key").toString(), map.value("op").toString(), valueText });
    }
    m_conditions.setItems(items);
    normalizeConditionsForForm(m_ruleFormId);
}

QVariantMap RuleEditorController::buildRule() const
{
    if (m_selectedIndex < 0) {
        return {};
    }
    QVariantList conditionList;
    const auto items = m_conditions.items();
    conditionList.reserve(items.size());
    for (const auto& item : items) {
        const auto value = m_controller ? m_controller->parseValue(item.op, item.valueText) : QVariant(item.valueText);
        conditionList.append(QVariantMap{
            { "key", item.key },
            { "op", item.op },
            { "value", value }
        });
    }
    return QVariantMap{
        { "id", m_ruleId },
        { "$form_id", m_ruleFormId },
        { "$and", conditionList },
        { "$result", m_ruleResult },
        { "$enabled", m_ruleEnabled }
    };
}

QStringList RuleEditorController::fieldKeysForForm(const QString& formId) const
{
    if (!m_formModel) {
        return {};
    }
    const auto fields = m_formModel->fieldsForForm(formId);
    QStringList keys;
    keys.reserve(fields.size());
    for (const auto& fieldValue : fields) {
        const auto key = fieldValue.toMap().value("key").toString();
        if (!key.isEmpty()) {
            keys.append(key);
        }
    }
    return keys;
}

void RuleEditorController::normalizeConditionsForForm(const QString& formId)
{
    const auto keys = fieldKeysForForm(formId);
    if (keys.isEmpty()) {
        return;
    }
    auto items = m_conditions.items();
    if (items.isEmpty()) {
        return;
    }
    const auto defaultKey = keys.first();
    bool changed = false;
    for (auto& item : items) {
        if (!keys.contains(item.key)) {
            item.key = defaultKey;
            changed = true;
        }
    }
    if (changed) {
        m_conditions.setItems(items);
    }
}

void RuleEditorController::ensureSelection()
{
    if (!m_model) {
        m_selectedIndex = -1;
        emit selectedIndexChanged();
        loadRule(-1);
        return;
    }
    const auto count = m_model->rules().size();
    if (count == 0) {
        m_selectedIndex = -1;
        emit selectedIndexChanged();
        loadRule(-1);
        return;
    }
    if (m_selectedIndex < 0) {
        m_selectedIndex = 0;
        emit selectedIndexChanged();
    } else if (m_selectedIndex >= count) {
        m_selectedIndex = count - 1;
        emit selectedIndexChanged();
    }
    loadRule(m_selectedIndex);
}

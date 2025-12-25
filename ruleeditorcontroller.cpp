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

    m_conditionModel = new ConditionModel(this);
    connect(m_conditionModel, &ConditionModel::conditionsChanged, this,
        [this]() { m_currentRule->setConditions(m_conditionModel->conditions()); });
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
    normalizeConditionsForForm(m_currentRule->formId());
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

Rule* RuleEditorController::currentRule() const
{
    return m_currentRule;
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
    const auto fieldKeys = fieldKeysForForm(m_currentRule->formId());
    const auto fieldKey = fieldKeys.isEmpty() ? QString() : fieldKeys.first();
    m_currentRule->addCondition(fieldKey, "$eq", QVariant());
}

void RuleEditorController::removeCondition(int row)
{
    m_currentRule->removeCondition(row);
}

void RuleEditorController::saveRule()
{
    if (!m_model || m_selectedIndex < 0 || m_selectedIndex >= m_model->rules().size()) {
        return;
    }
    updateCurrentRule();
    m_model->updateRule(m_selectedIndex, m_currentRule);
}

QString RuleEditorController::previewJson() const
{
    const auto rule = currentRule();
    if (rule == nullptr) {
        return QStringLiteral("未选择规则");
    }
    const auto doc = QJsonDocument::fromVariant(rule->toJSON());
    return QString::fromUtf8(doc.toJson(QJsonDocument::Indented));
}

void RuleEditorController::loadRule(int index)
{
    if (!m_model || index < 0 || index >= m_model->rules().size()) {
        m_currentRule = nullptr;
        emit currentRuleChanged();
        updateCurrentRule();
        return;
    }

    const auto& existingRule = m_model->rules().at(index);
    const auto rule = new Rule(existingRule->id(), existingRule->formId(), existingRule->conditions(), existingRule->result(), existingRule->enabled());
    m_currentRule = rule;
    emit currentRuleChanged();
    
    // Keep `currentRule` up to date when the condition list changes.
    connect(m_currentRule, &Rule::conditionsChanged, this,
        [this]() { updateCurrentRule(); });
    connect(m_currentRule, &Rule::conditionsChanged, this,
        [this]() { m_conditionModel->setConditions(m_currentRule->conditions()); });

    m_currentRule->setConditions(existingRule->conditions());
    normalizeConditionsForForm(m_currentRule->formId());
    updateCurrentRule();
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
    auto conditions = m_currentRule->conditions();
    for (int i = 0; i < conditions.size(); ++i) {
        const auto& condition = conditions[i];
        if (!keys.contains(condition->key())) {
            m_currentRule->setKey(i, keys.first());
        } else {
            m_currentRule->setKey(i, condition->key());
        }
    }
    m_currentRule->setConditions(conditions);

    // m_conditionModel->setConditions(conditions);
}

void RuleEditorController::updateCurrentRule()
{
    const auto rule = currentRule();
    if (m_currentRule == rule) {
        return;
    }
    m_currentRule = rule;
    emit currentRuleChanged();
    m_conditionModel->setConditions(m_currentRule->conditions());
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

ConditionModel* RuleEditorController::conditionModel() const
{
    return m_conditionModel;
}

void RuleEditorController::setConditionModel(ConditionModel* model)
{
    if (m_conditionModel == model) {
        return;
    }
    m_conditionModel = model;
    emit conditionModelChanged();
}
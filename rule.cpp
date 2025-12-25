#include "rule.h"
#include <QVariantMap>

Rule::Rule(const QString& id, const QString& formId, const QList<Condition*>& conditions, const QString& result, bool enabled, QObject* parent)
    : m_id { id }
    , m_formId { formId }
    , m_conditions(conditions)
    , m_result { result }
    , m_enabled { enabled }
    , QObject { parent }
{
}

QString Rule::id() const
{
    return m_id;
}

void Rule::setId(const QString& id)
{
    if (m_id == id) {
        return;
    }
    m_id = id;
    emit idChanged();
}

QString Rule::formId() const
{
    return m_formId;
}

void Rule::setFormId(const QString& formId)
{
    m_formId = formId;
    emit formIdChanged();
}

QList<Condition*> Rule::conditions() const
{
    return m_conditions;
}

void Rule::setConditions(const QList<Condition*>& conditions)
{
    m_conditions = conditions;
    emit conditionsChanged();
}

QString Rule::result() const
{
    return m_result;
}

void Rule::setResult(const QString& result)
{
    m_result = result;
    emit resultChanged();
}

bool Rule::enabled() const
{
    return m_enabled;
}

void Rule::setEnabled(bool enabled)
{
    m_enabled = enabled;
    emit enabledChanged();
}

Rule* Rule::fromJSON(const QVariantMap& json)
{
    QList<Condition*> conditions;
    for (const auto& item : json.value("$and").toList()) {
        conditions.append(Condition::fromJSON(item.toMap()));
    }
    return new Rule(json.value("id").toString(), json.value("$form_id").toString(), conditions, json.value("$result").toString(), json.value("$enabled").toBool());
}

QVariantMap Rule::toJSON() const
{
    QVariantList conditions_json;
    for (const auto& condition : m_conditions) {
        conditions_json.append(condition->toJSON());
    }
    QVariantMap json;
    json["id"] = m_id;
    json["$form_id"] = m_formId;
    json["$and"] = conditions_json;
    json["$result"] = m_result;
    json["$enabled"] = m_enabled;

    return json;
}

void Rule::addCondition(const QString& key, const QString& op, const QVariant& value)
{
    m_conditions.append(new Condition(key, op, value));
    emit conditionsChanged();
}

void Rule::removeCondition(int row)
{
    if (row < 0 || row >= m_conditions.size()) {
        return;
    }
    m_conditions.removeAt(row);
    emit conditionsChanged();
}

void Rule::setKey(int row, const QString& key)
{
    if (row < 0 || row >= m_conditions.size()) {
        return;
    }
    m_conditions[row]->setKey(key);
    emit conditionsChanged();
}

void Rule::setOp(int row, const QString& op)
{
    if (row < 0 || row >= m_conditions.size()) {
        return;
    }
    m_conditions[row]->setOp(op);
    emit conditionsChanged();
}

void Rule::setValue(int row, const QVariant& value)
{
    if (row < 0 || row >= m_conditions.size()) {
        return;
    }
    m_conditions[row]->setValue(value);
    emit conditionsChanged();
}

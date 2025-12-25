// In-memory rule store and option lists.
#pragma once

#include <QObject>
#include <QVariant>
#include <qcontainerfwd.h>

#include "rule.h"

class OpOptionItem {
public:
    QString label;
    QString value;
};

class RuleModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<Rule*> rules READ rules NOTIFY rulesChanged)
    Q_PROPERTY(QVariantList opOptions READ opOptions CONSTANT)
    Q_PROPERTY(QVariantList resultOptions READ resultOptions CONSTANT)
    Q_PROPERTY(QVariantMap resultMap READ resultMap CONSTANT)
public:
    explicit RuleModel(QObject* parent = nullptr);

    const QList<Rule*>& rules() const;
    QVariantList opOptions() const;
    QVariantList resultOptions() const;
    QVariantMap resultMap() const;

    Q_INVOKABLE Rule* createDefaultRule(const QString& formId, const QString& fieldKey);
    Q_INVOKABLE void addRule(Rule* rule);
    Q_INVOKABLE void updateRule(int index, Rule* rule);
    Q_INVOKABLE void removeRule(int index);

signals:
    void rulesChanged();

private:
    QString generateId() const;

    QList<Rule*> m_rules;
    QVariantList m_opOptions;
    QVariantList m_resultOptions;
    QVariantMap m_resultMap;
};

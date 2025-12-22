// In-memory rule store and option lists.
#pragma once

#include <QObject>
#include <QVariant>

class RuleModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList rules READ rules NOTIFY rulesChanged)
    Q_PROPERTY(QVariantList opOptions READ opOptions CONSTANT)
    Q_PROPERTY(QVariantList resultOptions READ resultOptions CONSTANT)
    Q_PROPERTY(QVariantMap resultMap READ resultMap CONSTANT)
public:
    explicit RuleModel(QObject* parent = nullptr);

    QVariantList rules() const;
    QVariantList opOptions() const;
    QVariantList resultOptions() const;
    QVariantMap resultMap() const;

    Q_INVOKABLE QVariantMap createDefaultRule(const QString& formId, const QString& fieldKey);
    Q_INVOKABLE void addRule(const QVariantMap& rule);
    Q_INVOKABLE void updateRule(int index, const QVariantMap& rule);
    Q_INVOKABLE void removeRule(int index);

signals:
    void rulesChanged();

private:
    QString generateId() const;

    QVariantList m_rules;
    QVariantList m_opOptions;
    QVariantList m_resultOptions;
    QVariantMap m_resultMap;
};

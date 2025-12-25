#ifndef RULE_H
#define RULE_H

#include "condition.h"
#include <QObject>

class Rule : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(QString formId READ formId WRITE setFormId NOTIFY formIdChanged FINAL)
    Q_PROPERTY(QList<Condition*> conditions READ conditions WRITE setConditions NOTIFY conditionsChanged FINAL)
    Q_PROPERTY(QString result READ result WRITE setResult NOTIFY resultChanged FINAL)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)
public:
    explicit Rule(const QString& id, const QString& formId, const QList<Condition*>& conditions, const QString& result, bool enabled, QObject* parent = nullptr);
    QString id() const;
    void setId(const QString& id);
    QString formId() const;
    void setFormId(const QString& formId);
    QList<Condition*> conditions() const;
    void setConditions(const QList<Condition*>& conditions);
    QString result() const;
    void setResult(const QString& result);
    bool enabled() const;
    void setEnabled(bool enabled);
    Q_INVOKABLE void addCondition(const QString& key, const QString& op, const QVariant& value);
    Q_INVOKABLE void removeCondition(int row);
    Q_INVOKABLE void setKey(int row, const QString& key);
    Q_INVOKABLE void setOp(int row, const QString& op);
    Q_INVOKABLE void setValue(int row, const QVariant& value);

    static Rule* fromJSON(const QVariantMap& json);
    QVariantMap toJSON() const;

private:
    QString m_id;
    QString m_formId;
    QList<Condition*> m_conditions;
    QString m_result;
    bool m_enabled;

signals:
    void idChanged();
    void formIdChanged();
    void conditionsChanged();
    void resultChanged();
    void enabledChanged();
};

#endif // RULE_H

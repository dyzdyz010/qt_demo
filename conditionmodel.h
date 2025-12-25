// QAbstractListModel for editing rule conditions in QML.
#pragma once

#include "condition.h"
#include <QAbstractListModel>

class ConditionModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QList<Condition*> conditions READ conditions WRITE setConditions NOTIFY conditionsChanged)
    
signals:
    void conditionsChanged();

public:
    enum ConditionRoles {
        KeyRole = Qt::UserRole + 1,
        OpRole,
        ValueTextRole
    };

    explicit ConditionModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QHash<int, QByteArray> roleNames() const override;

    QList<Condition*> conditions() const;
    void setConditions(const QList<Condition*>& conditions);

    Q_INVOKABLE void addCondition(const QString& key, const QString& op, const QString& valueText);
    Q_INVOKABLE void removeCondition(int row);
    Q_INVOKABLE void setKey(int row, const QString& key);
    Q_INVOKABLE void setOp(int row, const QString& op);
    Q_INVOKABLE void setValueText(int row, const QString& valueText);

private:
    QList<Condition*> m_conditions;
};

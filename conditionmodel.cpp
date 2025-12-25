// ConditionModel implementation.
#include "conditionmodel.h"

#include <QDebug>

ConditionModel::ConditionModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int ConditionModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_conditions.size();
}

QVariant ConditionModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_conditions.size()) {
        return {};
    }
    const auto& condition = m_conditions.at(index.row());
    switch (role) {
    case KeyRole:
        return condition->key();
    case OpRole:
        return condition->op();
    case ValueTextRole:
        return condition->value().toString();
    default:
        return {};
    }
}

bool ConditionModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_conditions.size()) {
        return false;
    }
    auto& condition = m_conditions[index.row()];
    switch (role) {
    case KeyRole:
        condition->setKey(value.toString());
        break;
    case OpRole:
        condition->setOp(value.toString());
        break;
    case ValueTextRole:
        condition->setValue(value);
        break;
    default:
        return false;
    }
    emit dataChanged(index, index, { role });
    return true;
}

Qt::ItemFlags ConditionModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QHash<int, QByteArray> ConditionModel::roleNames() const
{
    return {
        { KeyRole, "key" },
        { OpRole, "op" },
        { ValueTextRole, "valueText" }
    };
}

void ConditionModel::setConditions(const QList<Condition*>& conditions)
{
    beginResetModel();
    m_conditions = conditions;
    endResetModel();
}

QList<Condition*> ConditionModel::conditions() const
{
    return m_conditions;
}

void ConditionModel::addCondition(const QString& key, const QString& op, const QString& valueText)
{
    const int row = m_conditions.size();
    beginInsertRows(QModelIndex(), row, row);
    m_conditions.append(new Condition(key, op, valueText));
    endInsertRows();
}

void ConditionModel::removeCondition(int row)
{
    qDebug() << "removeCondition" << row;
    qDebug() << "m_conditions.size()" << m_conditions.size();
    if (row < 0 || row >= m_conditions.size()) {
        return;
    }
    beginRemoveRows(QModelIndex(), row, row);
    delete m_conditions.takeAt(row);
    endRemoveRows();
}

void ConditionModel::setKey(int row, const QString& key)
{
    if (row < 0 || row >= m_conditions.size()) {
        return;
    }
    m_conditions[row]->setKey(key);
    emit dataChanged(index(row), index(row), { KeyRole });
}

void ConditionModel::setOp(int row, const QString& op)
{
    setData(index(row), op, OpRole);
}

void ConditionModel::setValueText(int row, const QString& valueText)
{
    setData(index(row), valueText, ValueTextRole);
}

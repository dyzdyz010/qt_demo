// ConditionModel implementation.
#include "conditionmodel.h"

ConditionModel::ConditionModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int ConditionModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_items.size();
}

QVariant ConditionModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size()) {
        return {};
    }
    const auto& item = m_items.at(index.row());
    switch (role) {
    case KeyRole:
        return item.key;
    case OpRole:
        return item.op;
    case ValueTextRole:
        return item.valueText;
    default:
        return {};
    }
}

bool ConditionModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size()) {
        return false;
    }
    auto& item = m_items[index.row()];
    switch (role) {
    case KeyRole:
        item.key = value.toString();
        break;
    case OpRole:
        item.op = value.toString();
        break;
    case ValueTextRole:
        item.valueText = value.toString();
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

void ConditionModel::setItems(const QVector<Item>& items)
{
    beginResetModel();
    m_items = items;
    endResetModel();
}

QVector<ConditionModel::Item> ConditionModel::items() const
{
    return m_items;
}

void ConditionModel::addCondition(const QString& key, const QString& op, const QString& valueText)
{
    const int row = m_items.size();
    beginInsertRows(QModelIndex(), row, row);
    m_items.push_back({ key, op, valueText });
    endInsertRows();
}

void ConditionModel::removeCondition(int row)
{
    if (row < 0 || row >= m_items.size()) {
        return;
    }
    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
}

void ConditionModel::setKey(int row, const QString& key)
{
    setData(index(row), key, KeyRole);
}

void ConditionModel::setOp(int row, const QString& op)
{
    setData(index(row), op, OpRole);
}

void ConditionModel::setValueText(int row, const QString& valueText)
{
    setData(index(row), valueText, ValueTextRole);
}

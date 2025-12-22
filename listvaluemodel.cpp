// ListValueModel implementation.
#include "listvaluemodel.h"

ListValueModel::ListValueModel(QObject* parent)
    : QAbstractListModel(parent)
{
    m_items.append(QString());
}

QString ListValueModel::valueText() const
{
    return m_valueText;
}

void ListValueModel::setValueText(const QString& valueText)
{
    if (valueText == m_valueText && !m_items.isEmpty()) {
        return;
    }

    const auto parsed = parseListText(valueText);
    const auto normalized = parsed.join(", ");

    beginResetModel();
    m_items = parsed;
    if (m_items.isEmpty()) {
        m_items.append(QString());
    }
    endResetModel();

    if (m_valueText == normalized) {
        return;
    }
    m_valueText = normalized;
    emit valueTextChanged();
}

int ListValueModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_items.size();
}

QVariant ListValueModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size()) {
        return {};
    }

    if (role == ItemTextRole) {
        return m_items.at(index.row());
    }
    return {};
}

bool ListValueModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size()) {
        return false;
    }
    if (role != ItemTextRole) {
        return false;
    }

    const auto next = value.toString();
    if (m_items.at(index.row()) == next) {
        return false;
    }

    m_items[index.row()] = next;
    emit dataChanged(index, index, { role });
    updateValueText();
    return true;
}

Qt::ItemFlags ListValueModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QHash<int, QByteArray> ListValueModel::roleNames() const
{
    return {
        { ItemTextRole, "itemText" }
    };
}

void ListValueModel::appendItem(const QString& text)
{
    const int row = m_items.size();
    beginInsertRows(QModelIndex(), row, row);
    m_items.append(text);
    endInsertRows();
    updateValueText();
}

void ListValueModel::removeItem(int row)
{
    if (row < 0 || row >= m_items.size()) {
        return;
    }
    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
    ensureNotEmpty();
    updateValueText();
}

void ListValueModel::setItemText(int row, const QString& text)
{
    setData(index(row), text, ItemTextRole);
}

QStringList ListValueModel::parseListText(const QString& text)
{
    QStringList out;
    const auto trimmed = text.trimmed();
    if (trimmed.isEmpty()) {
        return out;
    }
    const auto parts = trimmed.split(",");
    for (const auto& part : parts) {
        const auto item = part.trimmed();
        if (!item.isEmpty()) {
            out.append(item);
        }
    }
    return out;
}

void ListValueModel::ensureNotEmpty()
{
    if (!m_items.isEmpty()) {
        return;
    }
    beginInsertRows(QModelIndex(), 0, 0);
    m_items.append(QString());
    endInsertRows();
}

void ListValueModel::updateValueText()
{
    QStringList parts;
    parts.reserve(m_items.size());
    for (const auto& item : m_items) {
        const auto trimmed = item.trimmed();
        if (!trimmed.isEmpty()) {
            parts.append(trimmed);
        }
    }
    const auto normalized = parts.join(", ");
    if (normalized == m_valueText) {
        return;
    }
    m_valueText = normalized;
    emit valueTextChanged();
}

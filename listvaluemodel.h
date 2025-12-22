// Model helper for list-valued condition inputs.
#pragma once

#include <QAbstractListModel>
#include <QStringList>

class ListValueModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString valueText READ valueText WRITE setValueText NOTIFY valueTextChanged)
public:
    enum ListValueRoles {
        ItemTextRole = Qt::UserRole + 1
    };

    explicit ListValueModel(QObject* parent = nullptr);

    QString valueText() const;
    void setValueText(const QString& valueText);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void appendItem(const QString& text);
    Q_INVOKABLE void removeItem(int row);
    Q_INVOKABLE void setItemText(int row, const QString& text);

signals:
    void valueTextChanged();

private:
    static QStringList parseListText(const QString& text);
    void ensureNotEmpty();
    void updateValueText();

    QStringList m_items;
    QString m_valueText;
};

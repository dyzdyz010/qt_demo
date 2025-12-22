// Holds static form definitions and navigation metadata.
#pragma once

#include <QObject>
#include <QVariant>

class FormModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList forms READ forms NOTIFY formsChanged)
    Q_PROPERTY(QVariantList navigationItems READ navigationItems NOTIFY navigationItemsChanged)
public:
    explicit FormModel(QObject* parent = nullptr);

    QVariantList forms() const;
    QVariantList navigationItems() const;

    Q_INVOKABLE QVariantMap formById(const QString& formId) const;
    Q_INVOKABLE QVariantList fieldsForForm(const QString& formId) const;
    Q_INVOKABLE QString formTitle(const QString& formId) const;

signals:
    void formsChanged();
    void navigationItemsChanged();

private:
    void rebuildNavigationItems();

    QVariantList m_forms;
    QVariantList m_navigationItems;
};

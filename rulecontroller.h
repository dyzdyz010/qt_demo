// Facade for rule evaluation and formatting.
#pragma once

#include <QObject>
#include <QVariant>

#include "formmodel.h"
#include "rulemodel.h"

class RuleController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(RuleModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(FormModel* formModel READ formModel WRITE setFormModel NOTIFY formModelChanged)
public:
    explicit RuleController(QObject* parent = nullptr);

    RuleModel* model() const;
    void setModel(RuleModel* model);

    FormModel* formModel() const;
    void setFormModel(FormModel* model);

    Q_INVOKABLE QVariantMap evaluate(const QString& formId, const QVariantMap& formData) const;
    Q_INVOKABLE bool isListOp(const QString& op) const;
    Q_INVOKABLE QString formatValue(const QVariant& value) const;
    Q_INVOKABLE QVariant parseValue(const QString& op, const QString& text) const;
    Q_INVOKABLE QStringList splitListText(const QString& text) const;
    Q_INVOKABLE QString resultLabel(const QString& code) const;
    Q_INVOKABLE QString ruleSummary(const QVariantMap& rule) const;

signals:
    void modelChanged();
    void formModelChanged();

private:
    RuleModel* m_model = nullptr;
    FormModel* m_formModel = nullptr;
};

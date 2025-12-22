// Bridges form state to rule evaluation for QML.
#pragma once

#include <QObject>
#include <QVariant>

#include "rulecontroller.h"

class FormController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString formId READ formId WRITE setFormId NOTIFY formIdChanged)
    Q_PROPERTY(QVariantList fields READ fields WRITE setFields NOTIFY fieldsChanged)
    Q_PROPERTY(QVariantMap formData READ formData NOTIFY formDataChanged)
    Q_PROPERTY(QVariantMap evaluation READ evaluation NOTIFY evaluationChanged)
    Q_PROPERTY(RuleController* ruleController READ ruleController WRITE setRuleController NOTIFY ruleControllerChanged)
public:
    explicit FormController(QObject* parent = nullptr);

    QString formId() const;
    void setFormId(const QString& formId);

    QVariantList fields() const;
    void setFields(const QVariantList& fields);

    QVariantMap formData() const;
    QVariantMap evaluation() const;

    RuleController* ruleController() const;
    void setRuleController(RuleController* controller);

    Q_INVOKABLE void setFieldValue(const QString& key, const QVariant& value);
    Q_INVOKABLE void setFieldText(const QString& key, const QString& text);
    Q_INVOKABLE int optionIndex(const QVariantList& options, const QVariant& value) const;
    Q_INVOKABLE void resetForm();
    Q_INVOKABLE void validate();

signals:
    void formIdChanged();
    void fieldsChanged();
    void formDataChanged();
    void evaluationChanged();
    void ruleControllerChanged();

private:
    QVariantMap fieldForKey(const QString& key) const;

    QString m_formId;
    QVariantList m_fields;
    QVariantMap m_formData;
    QVariantMap m_evaluation;
    RuleController* m_ruleController = nullptr;
};

// Manages rule editing state for the config UI.
#pragma once

#include <QObject>
#include <QStringList>

#include "conditionmodel.h"
#include "formmodel.h"
#include "rulecontroller.h"
#include "rulemodel.h"

class RuleEditorController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(RuleModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(FormModel* formModel READ formModel WRITE setFormModel NOTIFY formModelChanged)
    Q_PROPERTY(RuleController* controller READ controller WRITE setController NOTIFY controllerChanged)
    Q_PROPERTY(int selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY selectedIndexChanged)
    Q_PROPERTY(QString ruleId READ ruleId NOTIFY ruleIdChanged)
    Q_PROPERTY(QString ruleFormId READ ruleFormId WRITE setRuleFormId NOTIFY ruleFormIdChanged)
    Q_PROPERTY(QString ruleResult READ ruleResult WRITE setRuleResult NOTIFY ruleResultChanged)
    Q_PROPERTY(bool ruleEnabled READ ruleEnabled WRITE setRuleEnabled NOTIFY ruleEnabledChanged)
    Q_PROPERTY(ConditionModel* conditionsModel READ conditionsModel CONSTANT)
public:
    explicit RuleEditorController(QObject* parent = nullptr);

    RuleModel* model() const;
    void setModel(RuleModel* model);

    FormModel* formModel() const;
    void setFormModel(FormModel* model);

    RuleController* controller() const;
    void setController(RuleController* controller);

    int selectedIndex() const;
    void setSelectedIndex(int index);

    QString ruleId() const;
    QString ruleFormId() const;
    void setRuleFormId(const QString& formId);

    QString ruleResult() const;
    void setRuleResult(const QString& result);

    bool ruleEnabled() const;
    void setRuleEnabled(bool enabled);

    ConditionModel* conditionsModel();

    Q_INVOKABLE void addRule();
    Q_INVOKABLE void removeRule();
    Q_INVOKABLE void addCondition();
    Q_INVOKABLE void removeCondition(int row);
    Q_INVOKABLE void saveRule();
    Q_INVOKABLE QString previewJson() const;

signals:
    void modelChanged();
    void formModelChanged();
    void controllerChanged();
    void selectedIndexChanged();
    void ruleIdChanged();
    void ruleFormIdChanged();
    void ruleResultChanged();
    void ruleEnabledChanged();

private:
    void loadRule(int index);
    QVariantMap buildRule() const;
    void ensureSelection();
    QStringList fieldKeysForForm(const QString& formId) const;
    void normalizeConditionsForForm(const QString& formId);

    QMetaObject::Connection m_rulesChangedConnection;

    RuleModel* m_model = nullptr;
    FormModel* m_formModel = nullptr;
    RuleController* m_controller = nullptr;
    int m_selectedIndex = -1;

    QString m_ruleId;
    QString m_ruleFormId;
    QString m_ruleResult;
    bool m_ruleEnabled = true;

    ConditionModel m_conditions;
};

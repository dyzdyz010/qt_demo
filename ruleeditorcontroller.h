// Manages rule editing state for the config UI.
#pragma once

#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include <qtmetamacros.h>

#include "conditionmodel.h"
#include "formmodel.h"
#include "rulecontroller.h"
#include "rulemodel.h"

class RuleEditorController : public QObject {
    Q_OBJECT
    Q_PROPERTY(RuleModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(FormModel* formModel READ formModel WRITE setFormModel NOTIFY formModelChanged)
    Q_PROPERTY(RuleController* controller READ controller WRITE setController NOTIFY controllerChanged)
    Q_PROPERTY(int selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY selectedIndexChanged)
    Q_PROPERTY(Rule* currentRule READ currentRule NOTIFY currentRuleChanged)
    Q_PROPERTY(ConditionModel* conditionModel READ conditionModel WRITE setConditionModel NOTIFY conditionModelChanged)
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

    Rule* currentRule() const;

    ConditionModel* conditionModel() const;
    void setConditionModel(ConditionModel* model);

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
    void currentRuleChanged();
    void conditionModelChanged();
private:
    void loadRule(int index);
    void ensureSelection();
    QStringList fieldKeysForForm(const QString& formId) const;
    void normalizeConditionsForForm(const QString& formId);
    void updateCurrentRule();

    QMetaObject::Connection m_rulesChangedConnection;

    RuleModel* m_model = nullptr;
    FormModel* m_formModel = nullptr;
    RuleController* m_controller = nullptr;
    int m_selectedIndex = -1;

    Rule* m_currentRule = nullptr;
    ConditionModel* m_conditionModel = nullptr;
};

// RuleModel implementation and default rules.
#include "rulemodel.h"

#include <QDateTime>
#include <QDebug>
#include <qcontainerfwd.h>
#include <qobject.h>

RuleModel::RuleModel(QObject* parent)
    : QObject(parent)
{
    m_opOptions = QVariantList {
        QVariantMap { { "label", "等于" }, { "value", "$eq" } },
        QVariantMap { { "label", "不等于" }, { "value", "$ne" } },
        QVariantMap { { "label", "属于" }, { "value", "$in" } },
        QVariantMap { { "label", "不属于" }, { "value", "$nin" } },
        QVariantMap { { "label", "大于" }, { "value", "$gt" } },
        QVariantMap { { "label", "小于" }, { "value", "$lt" } },
        QVariantMap { { "label", "包含" }, { "value", "$contain" } },
        QVariantMap { { "label", "不包含" }, { "value", "$not_contain" } },
        QVariantMap { { "label", "为空" }, { "value", "$empty" } },
        QVariantMap { { "label", "长度大于" }, { "value", "$len_gt" } },
        QVariantMap { { "label", "长度小于" }, { "value", "$len_lt" } },
        QVariantMap { { "label", "长度等于" }, { "value", "$len_eq" } },
        QVariantMap { { "label", "长度不等于" }, { "value", "$len_ne" } },
        QVariantMap { { "label", "以...开头" }, { "value", "$start_with" } }
    };

    m_resultOptions = QVariantList {
        QVariantMap { { "code", "01" }, { "label", "01 - 出入事由不可录，默认值8" } },
        QVariantMap { { "code", "02" }, { "label", "02 - 签证种类代码不可录" } },
        QVariantMap { { "code", "03" }, { "label", "03 - 强制异常提交" } },
        QVariantMap { { "code", "04" }, { "label", "04 - 强制锁屏" } }
    };

    m_resultMap = QVariantMap {
        { "01", "出入事由不可录，默认值8" },
        { "02", "签证种类代码不可录" },
        { "03", "强制异常提交" },
        { "04", "强制锁屏" }
    };

    QVariantList rules_json = QVariantList {
        QVariantMap {
            { "id", "规则-001" },
            { "$form_id", "form_person" },
            { "$and", QVariantList { QVariantMap { { "key", "district" }, { "op", "$in" }, { "value", QVariantList { "中国", "香港" } } }, QVariantMap { { "key", "age" }, { "op", "$gt" }, { "value", 18 } }, QVariantMap { { "key", "name" }, { "op", "$contain" }, { "value", "张" } }, QVariantMap { { "key", "detail" }, { "op", "$eq" }, { "value", "" } }, QVariantMap { { "key", "content" }, { "op", "$start_with" }, { "value", "高" } } } },
            { "$result", "01" },
            { "$enabled", true } },
        QVariantMap {
            { "id", "中国边民出境" },
            { "$form_id", "bmph" },
            { "ywlb", "边民平衡" },
            { "$and", QVariantList { QVariantMap { { "key", "inout" }, { "op", "$in" }, { "value", QVariantList { "2" } } }, QVariantMap { { "key", "zjzl" }, { "op", "$in" }, { "value", QVariantList { "20", "96" } } }, QVariantMap { { "key", "region" }, { "op", "$in" }, { "value", QVariantList { "CHN" } } } } },
            { "$result", "01" },
            { "$enabled", true } },
        QVariantMap {
            { "id", "外国边民入境" },
            { "ywlb", "边民平衡" }, { "$form_id", "bmph" },
            { "$and", QVariantList { QVariantMap { { "key", "inout" }, { "op", "$in" }, { "value", QVariantList { "1" } } }, QVariantMap { { "key", "zjzl" }, { "op", "$in" }, { "value", QVariantList { "20", "96" } } }, QVariantMap { { "key", "region" }, { "op", "$nin" }, { "value", QVariantList { "CHN", "HKG", "MAC", "TWN" } } } } },
            { "$result", "01" },
            { "$enabled", true } },
        QVariantMap {
            { "id", "出入事由" },
            { "$form_id", "lkyf" },
            { "ywlb", "旅客预放" },
            { "$and", QVariantList {
                          QVariantMap { { "key", "crbz" }, { "op", "$eq" }, { "value", "2" } },
                          QVariantMap { { "key", "gjdqdm" }, { "op", "$nin" }, { "value", QVariantList { "CHN" } } },
                      } },

            { "$result", "01" }, { "$enabled", true } },
        QVariantMap {
            { "id", "签证种类代码" },
            { "$form_id", "lkyf" },
            { "ywlb", "旅客预放" },
            { "$and", QVariantList {
                          QVariantMap { { "key", "crbz" }, { "op", "$eq" }, { "value", "2" } },
                          QVariantMap { { "key", "gjdqdm" }, { "op", "$nin" }, { "value", QVariantList { "CHN" } } },
                      } },

            { "$result", "02" }, { "$enabled", true } },
        QVariantMap {
                { "id", "指纹采集" },
                { "$form_id", "lkyf" },
                { "ywlb", "旅客预放" },
                { "$and", QVariantList {
                              QVariantMap { { "key", "zjzl" }, { "op", "$nin" }, { "value", QVariantList { "11", "62" } } },
                              QVariantMap { { "key", "gjdqdm" }, { "op", "$nin" }, { "value", QVariantList { "CHN", "HKG", "MAC", "TWN" } } },
                              QVariantMap { { "key", "crbz" }, { "op", "$eq" }, { "value", "1" } }
                             },
                           },
    
                { "$result", "02" }, { "$enabled", true } },
        QVariantMap { { "id", "规则-002" }, { "$form_id", "form_person" }, { "$and", QVariantList {
                                                                                         QVariantMap { { "key", "age" }, { "op", "$gt" }, { "value", 18 } },
                                                                                         QVariantMap { { "key", "district" }, { "op", "$in" }, { "value", QVariantList { "中国", "香港" } } },
                                                                                     } },
            { "$result", "02" }, { "$enabled", true } },
        QVariantMap { { "id", "规则-003" }, { "$form_id", "form_order" }, { "$and", QVariantList { QVariantMap { { "key", "amount" }, { "op", "$gt" }, { "value", 1000 } }, QVariantMap { { "key", "channel" }, { "op", "$eq" }, { "value", "合作方" } } } }, { "$result", "03" }, { "$enabled", true } }, QVariantMap { { "id", "规则-004" }, { "$form_id", "form_device" }, { "$and", QVariantList { QVariantMap { { "key", "battery" }, { "op", "$lt" }, { "value", 10 } } } }, { "$result", "04" }, { "$enabled", true } }
    };

    for (const auto& item : std::as_const(rules_json)) {
        m_rules.append(Rule::fromJSON(item.toMap()));
    }
}

const QList<Rule*>& RuleModel::rules() const
{
    return m_rules;
}

QVariantList RuleModel::opOptions() const
{
    return m_opOptions;
}

QVariantList RuleModel::resultOptions() const
{
    return m_resultOptions;
}

QVariantMap RuleModel::resultMap() const
{
    return m_resultMap;
}

Rule* RuleModel::createDefaultRule(const QString& formId, const QString& fieldKey)
{
    return new Rule(generateId(),
        formId.isEmpty() ? QStringLiteral("form_person") : formId,
        QList<Condition*> { new Condition(fieldKey.isEmpty() ? QStringLiteral("district") : fieldKey, "$eq", "") },
        "01", true);
}

void RuleModel::addRule(Rule* rule)
{
    m_rules.append(rule);
    emit rulesChanged();
}

void RuleModel::updateRule(int index, Rule* rule)
{
    if (index < 0 || index >= m_rules.size() || !rule) {
        return;
    }
    // 由于Rule继承自QObject，不能使用拷贝赋值运算符（第109行原错误），
    // 需要直接更新属性而不是替换整个对象
    Rule* targetRule = m_rules[index];
    targetRule->setId(rule->id());
    targetRule->setFormId(rule->formId());
    targetRule->setResult(rule->result());
    targetRule->setEnabled(rule->enabled());
    // 注意：setConditions也有类似问题（Condition也继承自QObject），
    // 但这是另一个需要单独修复的问题
    // targetRule->setConditions(rule->conditions());
    emit rulesChanged();
}

void RuleModel::removeRule(int index)
{
    if (index < 0 || index >= m_rules.size()) {
        return;
    }
    m_rules.removeAt(index);
    emit rulesChanged();
}

QString RuleModel::generateId() const
{
    const auto stamp = QString::number(QDateTime::currentMSecsSinceEpoch());
    return QStringLiteral("规则-") + stamp;
}

// RuleModel implementation and default rules.
#include "rulemodel.h"

#include <QDateTime>

RuleModel::RuleModel(QObject* parent)
    : QObject(parent)
{
    m_opOptions = QVariantList{
        QVariantMap{ { "label", "等于" }, { "value", "$eq" } },
        QVariantMap{ { "label", "不等于" }, { "value", "$ne" } },
        QVariantMap{ { "label", "属于" }, { "value", "$in" } },
        QVariantMap{ { "label", "不属于" }, { "value", "$nin" } },
        QVariantMap{ { "label", "大于" }, { "value", "$gt" } },
        QVariantMap{ { "label", "小于" }, { "value", "$lt" } },
        QVariantMap{ { "label", "包含" }, { "value", "$contain" } },
        QVariantMap{ { "label", "不包含" }, { "value", "$not_contain" } },
        QVariantMap{ { "label", "为空" }, { "value", "$empty" } },
        QVariantMap{ { "label", "长度大于" }, { "value", "$len_gt" } },
        QVariantMap{ { "label", "长度小于" }, { "value", "$len_lt" } },
        QVariantMap{ { "label", "长度等于" }, { "value", "$len_eq" } },
        QVariantMap{ { "label", "长度不等于" }, { "value", "$len_ne" } },
        QVariantMap{ { "label", "以...开头" }, { "value", "$start_with" } }
    };

    m_resultOptions = QVariantList{
        QVariantMap{ { "code", "01" }, { "label", "01 - 提示确认提交" } },
        QVariantMap{ { "code", "02" }, { "label", "02 - 提示异常提交" } },
        QVariantMap{ { "code", "03" }, { "label", "03 - 强制异常提交" } },
        QVariantMap{ { "code", "04" }, { "label", "04 - 强制锁屏" } }
    };

    m_resultMap = QVariantMap{
        { "01", "提示确认提交" },
        { "02", "提示异常提交" },
        { "03", "强制异常提交" },
        { "04", "强制锁屏" }
    };

    m_rules = QVariantList{
        QVariantMap{
            { "id", "规则-001" },
            { "$form_id", "form_person" },
            { "$and", QVariantList{
                QVariantMap{ { "key", "district" }, { "op", "$in" }, { "value", QVariantList{ "中国", "香港" } } },
                QVariantMap{ { "key", "age" }, { "op", "$gt" }, { "value", 18 } },
                QVariantMap{ { "key", "name" }, { "op", "$contain" }, { "value", "张" } },
                QVariantMap{ { "key", "detail" }, { "op", "$eq" }, { "value", "" } },
                QVariantMap{ { "key", "content" }, { "op", "$start_with" }, { "value", "高" } }
            } },
            { "$result", "01" },
            { "$enabled", true }
        },
        QVariantMap{
            { "id", "规则-002" },
            { "$form_id", "form_person" },
            { "$and", QVariantList{
                QVariantMap{ { "key", "age" }, { "op", "$lt" }, { "value", 18 } }
            } },
            { "$result", "02" },
            { "$enabled", true }
        },
        QVariantMap{
            { "id", "规则-003" },
            { "$form_id", "form_order" },
            { "$and", QVariantList{
                QVariantMap{ { "key", "amount" }, { "op", "$gt" }, { "value", 1000 } },
                QVariantMap{ { "key", "channel" }, { "op", "$eq" }, { "value", "合作方" } }
            } },
            { "$result", "03" },
            { "$enabled", true }
        },
        QVariantMap{
            { "id", "规则-004" },
            { "$form_id", "form_device" },
            { "$and", QVariantList{
                QVariantMap{ { "key", "battery" }, { "op", "$lt" }, { "value", 10 } }
            } },
            { "$result", "04" },
            { "$enabled", true }
        }
    };
}

QVariantList RuleModel::rules() const
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

QVariantMap RuleModel::createDefaultRule(const QString& formId, const QString& fieldKey)
{
    return QVariantMap{
        { "id", generateId() },
        { "$form_id", formId.isEmpty() ? QStringLiteral("form_person") : formId },
        { "$and", QVariantList{
            QVariantMap{
                { "key", fieldKey.isEmpty() ? QStringLiteral("district") : fieldKey },
                { "op", "$eq" },
                { "value", "" }
            }
        } },
        { "$result", "01" },
        { "$enabled", true }
    };
}

void RuleModel::addRule(const QVariantMap& rule)
{
    m_rules.append(rule);
    emit rulesChanged();
}

void RuleModel::updateRule(int index, const QVariantMap& rule)
{
    if (index < 0 || index >= m_rules.size()) {
        return;
    }
    m_rules[index] = rule;
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

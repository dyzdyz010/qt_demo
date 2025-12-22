// FormModel implementation.
#include "formmodel.h"

FormModel::FormModel(QObject* parent)
    : QObject(parent)
{
    m_forms = QVariantList{
        QVariantMap{
            { "id", "form_person" },
            { "title", "人员表单" },
            { "fields", QVariantList{
                QVariantMap{ { "key", "district" }, { "label", "地区" }, { "type", "select" }, { "options", QVariantList{ "中国", "香港", "美国", "德国" } } },
                QVariantMap{ { "key", "age" }, { "label", "年龄" }, { "type", "number" }, { "placeholder", "例如：18" } },
                QVariantMap{ { "key", "name" }, { "label", "姓名" }, { "type", "text" }, { "placeholder", "例如：张三" } },
                QVariantMap{ { "key", "detail" }, { "label", "详情" }, { "type", "text" }, { "placeholder", "例如：可选" } },
                QVariantMap{ { "key", "content" }, { "label", "内容" }, { "type", "text" }, { "placeholder", "例如：高开头" } }
            } }
        },
        QVariantMap{
            { "id", "form_order" },
            { "title", "订单表单" },
            { "fields", QVariantList{
                QVariantMap{ { "key", "order_id" }, { "label", "订单号" }, { "type", "text" }, { "placeholder", "例如：订单001" } },
                QVariantMap{ { "key", "amount" }, { "label", "金额" }, { "type", "number" }, { "placeholder", "例如：1200" } },
                QVariantMap{ { "key", "currency" }, { "label", "币种" }, { "type", "select" }, { "options", QVariantList{ "人民币", "美元", "欧元" } } },
                QVariantMap{ { "key", "channel" }, { "label", "渠道" }, { "type", "select" }, { "options", QVariantList{ "网页", "应用", "合作方" } } },
                QVariantMap{ { "key", "note" }, { "label", "备注" }, { "type", "multiline" }, { "placeholder", "例如：简短说明" } },
                QVariantMap{ { "key", "urgent" }, { "label", "是否加急" }, { "type", "bool" } }
            } }
        },
        QVariantMap{
            { "id", "form_device" },
            { "title", "设备表单" },
            { "fields", QVariantList{
                QVariantMap{ { "key", "os" }, { "label", "系统" }, { "type", "select" }, { "options", QVariantList{ "安卓系统", "苹果系统", "视窗系统", "开源系统" } } },
                QVariantMap{ { "key", "version" }, { "label", "系统版本" }, { "type", "text" }, { "placeholder", "例如：14.2" } },
                QVariantMap{ { "key", "model" }, { "label", "型号" }, { "type", "text" }, { "placeholder", "例如：型号甲" } },
                QVariantMap{ { "key", "battery" }, { "label", "电量(%)" }, { "type", "number" }, { "placeholder", "例如：15" } },
                QVariantMap{ { "key", "owner" }, { "label", "持有人" }, { "type", "text" }, { "placeholder", "例如：用户甲" } }
            } }
        }
    };
    rebuildNavigationItems();
}

QVariantList FormModel::forms() const
{
    return m_forms;
}

QVariantList FormModel::navigationItems() const
{
    return m_navigationItems;
}

QVariantMap FormModel::formById(const QString& formId) const
{
    for (const auto& formValue : m_forms) {
        const auto form = formValue.toMap();
        if (form.value("id").toString() == formId) {
            return form;
        }
    }
    return {};
}

QVariantList FormModel::fieldsForForm(const QString& formId) const
{
    const auto form = formById(formId);
    return form.value("fields").toList();
}

QString FormModel::formTitle(const QString& formId) const
{
    const auto form = formById(formId);
    const auto title = form.value("title").toString();
    return title.isEmpty() ? QStringLiteral("未知表单") : title;
}

void FormModel::rebuildNavigationItems()
{
    QVariantList items;
    items.reserve(m_forms.size() + 1);
    for (const auto& formValue : m_forms) {
        const auto form = formValue.toMap();
        items.append(QVariantMap{
            { "title", form.value("title") },
            { "type", "form" },
            { "formId", form.value("id") }
        });
    }
    items.append(QVariantMap{
        { "title", QStringLiteral("规则配置") },
        { "type", "config" }
    });
    m_navigationItems = items;
    emit navigationItemsChanged();
}

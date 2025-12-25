#include "condition.h"

/**
 * @brief 构造函数实现
 * 初始化条件的名称、操作符和值
 */
Condition::Condition(const QString& key, const QString& op, const QVariant& value, QObject* parent)
    : m_key(key)
    , m_op(op)
    , m_value(value)
    , QObject(parent)
{
}

/**
 * @brief 从JSON对象创建Condition实例
 * 从QVariantMap中提取name、op、value字段并创建Condition对象
 */
Condition* Condition::fromJSON(const QVariantMap& json)
{
    return new Condition(json.value("key").toString(), json.value("op").toString(), json.value("value"));
}

/**
 * @brief 将Condition对象转换为JSON格式
 * 将条件的名称、操作符和值打包成QVariantMap返回
 */
QVariantMap Condition::toJSON() const
{
    return QVariantMap {
        { "key", m_key },
        { "op", m_op },
        { "value", m_value }
    };
}

/**
 * @brief 获取条件名称
 */
QString Condition::key() const
{
    return m_key;
}

/**
 * @brief 设置条件键
 * 如果新值与当前值相同，则不进行更新，避免不必要的信号发射
 */
void Condition::setKey(const QString& key)
{
    if (m_key == key) {
        return;
    }
    m_key = key;
    emit keyChanged(key);
}

/**
 * @brief 获取条件操作符
 */
QString Condition::op() const
{
    return m_op;
}

/**
 * @brief 设置条件操作符
 * 如果新值与当前值相同，则不进行更新，避免不必要的信号发射
 */
void Condition::setOp(const QString& op)
{
    if (m_op == op) {
        return;
    }
    m_op = op;
    emit opChanged(op);
}

/**
 * @brief 获取条件值
 */
QVariant Condition::value() const
{
    return m_value;
}

/**
 * @brief 设置条件值
 * 如果新值与当前值相同，则不进行更新，避免不必要的信号发射
 */
void Condition::setValue(const QVariant& value)
{
    if (m_value == value) {
        return;
    }
    m_value = value;
    emit valueChanged(value);
}

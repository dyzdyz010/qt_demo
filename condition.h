#ifndef CONDITION_H
#define CONDITION_H

#include <QObject>
#include <QVariant>

/**
 * @brief Condition类用于表示一个条件对象
 *
 * 该类封装了条件的基本信息，包括条件名称、操作符和值。
 * 支持JSON序列化和反序列化，并提供了Qt属性系统支持，
 * 可以在QML中使用。

 * 举例：
 * {
 *     "key": "条件名称",
 *     "op": "操作符",
 *     "value": "条件值"
 * }
 */
class Condition : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString key READ key WRITE setKey NOTIFY keyChanged FINAL)
    Q_PROPERTY(QString op READ op WRITE setOp NOTIFY opChanged FINAL)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged FINAL)
public:
    /**
     * @brief 构造函数
     * @param key 条件名称
     * @param op 条件操作符
     * @param value 条件值
     * @param parent 父对象指针
     */
    explicit Condition(const QString& key, const QString& op, const QVariant& value, QObject* parent = nullptr);

    /**
     * @brief 从JSON对象创建Condition实例
     * @param json 包含key、op、value字段的QVariantMap对象
     * @return 创建的Condition对象
     */
    static Condition* fromJSON(const QVariantMap& json);

    /**
     * @brief 将Condition对象转换为JSON格式
     * @return 包含key、op、value字段的QVariantMap对象
     */
    QVariantMap toJSON() const;

    /**
     * @brief 获取条件名称
     * @return 条件名称
     */
    QString key() const;

    /**
     * @brief 设置条件名称
     * @param key 新的条件名称
     */
    void setKey(const QString& key);

    /**
     * @brief 获取条件操作符
     * @return 条件操作符
     */
    QString op() const;

    /**
     * @brief 设置条件操作符
     * @param type 新的条件类型
     */
    void setOp(const QString& op);

    /**
     * @brief 获取条件值
     * @return 条件值
     */
    QVariant value() const;

    /**
     * @brief 设置条件值
     * @param value 新的条件值
     */
    void setValue(const QVariant& value);

signals:
    /**
     * @brief 当条件键改变时发出的信号
     * @param key 新的条件键
     */
    void keyChanged(const QString& key);

    /**
     * @brief 当条件操作符改变时发出的信号
     * @param op 新的条件操作符
     */
    void opChanged(const QString& op);

    /**
     * @brief 当条件值改变时发出的信号
     * @param value 新的条件值
     */
    void valueChanged(const QVariant& value);

private:
    QString m_key; ///< 条件键
    QString m_op; ///< 条件操作符
    QVariant m_value; ///< 条件值
};

#endif // CONDITION_H

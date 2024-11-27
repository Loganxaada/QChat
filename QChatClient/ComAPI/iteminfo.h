#ifndef ITEMINFO_H
#define ITEMINFO_H

#include <QtCore/qglobal.h>
#include <QPixmap>
#include <QRectF>
#include "MyUnit.h"

class ItemInfo
{
public:
    ItemInfo();
    ItemInfo(const QString& strName, const QString& datetime, const QString &pixmap,const QString &text,
             const QString& strSize, const quint8 &orientation = Right, const quint8 &msgType = TEXT);

    ~ItemInfo();

public:
    // public interfaces

    void setName(const QString &text);
    QString getName() const;

    void setDatetime(const QString &text);
    QString getDatetime() const;

    void setHeadPixmap(const QString &pixmap);
    QString getStrPixmap() const;

    void setText(const QString& text);
    QString getText() const;

    void setFileSizeString(const QString& strSize);
    QString getFileSizeString() const;

    void setOrientation(quint8 orientation);
    quint8 getOrientation() const;

    void setMsgType(const quint8 &msgType);
    quint8 getMsgType() const;

    void setItemHeight(qreal itemHeight);
    qreal getItemHeight() const;

    void setBobbleRect(const QRectF &bobbleRect);
    QRectF getBobbleRect() const;

private:
    // 消息的标题，比如名字和时间
    QString     m_strName;
    // 消息日期
    QString     m_strDatetime;
    // 头像图片
    QString     m_strPixmap;
    // 消息内容
    QString     m_strText;
    // 如果是文件，会显示文件大小
    QString     m_strSize;

    // 气泡方向
    quint8      m_orientation;
    // 消息内容
    quint8      m_msgType;
    // 暂时没有用
    qreal       m_itemHeight;
    // 窗口的坐标，在点击查询时需要访问
    QRectF      m_bobbleRect;
    // 消息状态
    quint8      m_nStatus;
};


#endif // ITEMINFO_H

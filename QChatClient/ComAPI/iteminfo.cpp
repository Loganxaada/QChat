#include "iteminfo.h"

ItemInfo::ItemInfo() :
    m_strName(""),
    m_strDatetime(""),
    m_strPixmap(""),
    m_strText(""),
    m_strSize(""),
    m_orientation(Left),
    m_msgType(TEXT),
    m_itemHeight(ITEM_HEIGHT)
{

}


ItemInfo::ItemInfo(const QString &strName, const QString &datetime, const QString &pixmap, const QString &text, const QString &strSize, const quint8 &orientation, const quint8 &msgType) :
    m_strName(strName),
    m_strDatetime(datetime),
    m_strPixmap(pixmap),
    m_strText(text),
    m_strSize(strSize),
    m_orientation(orientation),
    m_msgType(msgType),
    m_itemHeight(ITEM_HEIGHT)
{

}

ItemInfo::~ItemInfo() {

}

void ItemInfo::setName(const QString &text)
{
        m_strName = text;
}

QString ItemInfo::getName() const
{
    return m_strName;
}

void ItemInfo::setDatetime(const QString &text)
{
    m_strDatetime = text;
}

QString ItemInfo::getDatetime() const
{
    return m_strDatetime;
}

void ItemInfo::setHeadPixmap(const QString &pixmap)
{
    m_strPixmap = pixmap;
}

QString ItemInfo::getStrPixmap() const {
    return m_strPixmap;
}


void ItemInfo::setText(const QString &text)
{
    m_strText = text;
}

QString ItemInfo::getText() const
{
    return m_strText;
}

void ItemInfo::setFileSizeString(const QString &strSize)
{
    m_strSize = strSize;
}

QString ItemInfo::getFileSizeString() const
{
    return m_strSize;
}

quint8 ItemInfo::getOrientation() const
{
    return m_orientation;
}

void ItemInfo::setOrientation(quint8 orientation)
{
    m_orientation = orientation;
}

quint8 ItemInfo::getMsgType() const
{
    return m_msgType;
}

void ItemInfo::setMsgType(const quint8 &msgType)
{
    m_msgType = msgType;
}

qreal ItemInfo::getItemHeight() const
{
    return m_itemHeight;
}

void ItemInfo::setItemHeight(qreal itemHeight)
{
    m_itemHeight = itemHeight;
}

QRectF ItemInfo::getBobbleRect() const
{
    return m_bobbleRect;
}

void ItemInfo::setBobbleRect(const QRectF &bobbleRect)
{
    m_bobbleRect = bobbleRect;
}

#ifndef QQCELL_H
#define QQCELL_H

#include <QString>
#include <QList>

typedef enum tagEnQQCellType
{
    QQCELLTYPE_GROUP = 0,
    QQCELLTYPE_CHILD = 1,
    QQCELLTYPE_GROUPEX = 2,
} EnQQCellType;

class QQCell
{
public:
    QQCell();

    void SetSubtitle(const QString &text);
    void SetIconPath(const QString &path);
    void SetStatus(const quint8 &val);
public:
    QString m_iconPath;
    QString name;
    QString m_signature;
    QString m_subTitle;

    QString ipaddr;
    QString groupName;      //组名
    int     id;             //QQ号，作为标识
    int     status;

    //
    EnQQCellType type;

    //下面几项都是用于组(QQCELLTYPE_GROUP)状态, 对于子(QQCELLTYPE_CHILD)状态无用
    //是否是打开状态
    bool m_bOpen;
    //子项
    QList<QQCell *> childs;
};

#endif // QQCELL_H

#include "qqcell.h"

QQCell::QQCell()
{
    m_bOpen = false;
}

void QQCell::SetSubtitle(const QString &text)
{
    if (text.isEmpty()) return;
    m_subTitle = text;
}

void QQCell::SetIconPath(const QString &path)
{
    if (path.isEmpty()) return;
    m_iconPath = path;
}

void QQCell::SetStatus(const quint8 &val)
{
    status = val;
}

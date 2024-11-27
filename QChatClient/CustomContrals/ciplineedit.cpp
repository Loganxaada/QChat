#include "ciplineedit.h"
#include<QHBoxLayout>
#include<QLabel>
#include<QRegExpValidator>
#include<QKeyEvent>
#include<QMessageBox>

CIPLineEdit::CIPLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    QRegExp rx("(25[0-5]|2[0-4][0-9]|1?[0-9]{1,2})");
    QHBoxLayout *pHBox = new QHBoxLayout(this);
    pHBox->setSpacing(5);  // 增加间距
    pHBox->setContentsMargins(5, 5, 5, 5);
    QLabel *labelDot[3];
    for (int i = 0; i < 4; i++)
    {
        m_lineEdit[i] = new QLineEdit(this);
        m_lineEdit[i]->setProperty("ip", true);
        m_lineEdit[i]->setFrame(false);
        m_lineEdit[i]->setMaxLength(3);
        m_lineEdit[i]->setAlignment(Qt::AlignCenter);
        m_lineEdit[i]->installEventFilter(this);
        m_lineEdit[i]->setValidator(new QRegExpValidator(rx, this));
        m_lineEdit[i]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        pHBox->addWidget(m_lineEdit[i]);
        if (i < 3) {
            labelDot[i] = new QLabel(this);
            labelDot[i]->setText(".");
            labelDot[i]->setFixedWidth(10);  // 调整宽度
            labelDot[i]->setAlignment(Qt::AlignCenter);  // 居中对齐
            pHBox->addWidget(labelDot[i]);
        }
    }
}

void CIPLineEdit::setText(const QString &strIP)
{
    // 是否是IP地址
    if (!isTextValid(strIP))
    {
        QMessageBox::warning(this, "Attention","Your IP Address is Invalid!", QMessageBox::StandardButton::Ok);
        return;
    }
    else
    {
        int i = 0;
        QStringList ipList = strIP.split(".");

        foreach (QString ip ,ipList)
        {
            m_lineEdit[i]->setText(ip);
            i++;
        }
    }
}

QString CIPLineEdit::text() const
{
    QString strIP;
    for (int i = 0; i < 4; i++) {
        strIP.append(m_lineEdit[i]->text());
        if (3 != i) {
            strIP.append(".");
        }
    }
    return strIP;
}

bool CIPLineEdit::eventFilter(QObject *obj, QEvent *ev)
{
    if (children().contains(obj) && QEvent::KeyPress == ev->type())
    {
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(ev);
        QLineEdit *pCurrentEdit = qobject_cast<QLineEdit *>(obj);
        switch (keyEvent->key())
        {
        case Qt::Key_0:
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
        {
            QString strText = pCurrentEdit->text();
            if (pCurrentEdit->selectedText().length())
            {
                pCurrentEdit->text().replace(pCurrentEdit->selectedText(), QChar(keyEvent->key()));
            }
            else if (strText.length() <=3 &&
                     strText.toInt() * 10 > 255)
            {
                int index = getIndex(pCurrentEdit);
                if (index != -1 && index != 3)
                {
                    m_lineEdit[index + 1]->setFocus();
                    m_lineEdit[index + 1]->selectAll();
                }
            }
            else if (strText.length() == 2 && strText.toInt() * 10 < 255)
            {
                if (Qt::Key_0 == keyEvent->key() && strText.toInt())
                {
                    pCurrentEdit->setText(strText.insert(pCurrentEdit->cursorPosition(),
                                                         QChar(Qt::Key_0)));
                }
            }
            return QLineEdit::eventFilter(obj, ev);
        }
            break;
        case Qt::Key_Backspace:
        {
            QString strText = pCurrentEdit->text();
            if (strText.isEmpty())
            {
                int index = getIndex(pCurrentEdit);
                if (index != -1 && index != 0)
                {
                    m_lineEdit[index - 1]->setFocus();
                    int length = m_lineEdit[index - 1]->text().length();
                    m_lineEdit[index - 1]->setCursorPosition(length ? length : 0);
                }
            }
            return QLineEdit::eventFilter(obj, ev);
        }
        case Qt::Key_Left:
        {
            if (!pCurrentEdit->cursorPosition())
            {
                int index = getIndex(pCurrentEdit);
                if (index != -1 && index != 0)
                {
                    m_lineEdit[index - 1]->setFocus();
                    int length = m_lineEdit[index - 1]->text().length();
                    m_lineEdit[index - 1]->setCursorPosition(length ? length : 0);
                }
            }
            return QLineEdit::eventFilter(obj, ev);
        }
        case Qt::Key_Right:
        {
            if (pCurrentEdit->cursorPosition() == pCurrentEdit->text().length())
            {
                int index = getIndex(pCurrentEdit);
                if (index != -1 && index != 3)
                {
                    m_lineEdit[index + 1]->setFocus();
                    m_lineEdit[index + 1]->setCursorPosition(0);
                }
            }
            return QLineEdit::eventFilter(obj, ev);
        }
            // 小键盘的“.”号
        case Qt::Key_Period:
        {
            int index = getIndex(pCurrentEdit);
            if (index != -1 && index != 3)
            {
                m_lineEdit[index + 1]->setFocus();
                m_lineEdit[index + 1]->setCursorPosition(0);
            }
            return QLineEdit::eventFilter(obj, ev);
        }
            break;
        default:
            break;
        }
    }
    return false;
}

int CIPLineEdit::getIndex(QLineEdit *pEdit)
{
    int index = -1;
    for (int i = 0; i < 4; i++)
    {
        if (pEdit == m_lineEdit[i])
            index = i;
    }
    return index;
}

bool CIPLineEdit::isTextValid(const QString &strIP)
{
    QRegExp rx2("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if (!rx2.exactMatch(strIP))
        return false;
    return true;
}

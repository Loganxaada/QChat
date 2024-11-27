#ifndef CIPLINEEDIT_H
#define CIPLINEEDIT_H

#include<QLineEdit>

class CIPLineEdit:public QLineEdit
{
    Q_OBJECT
public:
    explicit CIPLineEdit(QWidget *parent=nullptr);

    void setText(const QString &strIP);
    QString text() const;
protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

    int getIndex(QLineEdit *pEdit);
    bool isTextValid(const QString &strIP);
private:
    QLineEdit *m_lineEdit[4];
};

#endif // CIPLINEEDIT_H

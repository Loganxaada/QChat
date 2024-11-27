#ifndef CCHATWINDOW_H
#define CCHATWINDOW_H

#include <QWidget>
#include<QStandardItemModel>
#include<QTime>
#include<QTableView>
#include<QTextEdit>
#include"ComAPI/json.hpp"
#include"customwidget.h"
#include"CustomContrals/CustomList/qqcell.h"
#include"CustomContrals/cchatbubble.h"
#include"ComAPI/group.h"
using json = nlohmann::json;

class CChatWindow : public CustomMoveWidget
{
    Q_OBJECT

public:
    explicit CChatWindow(QWidget *parent = nullptr);
    ~CChatWindow();

    void setCell(QQCell *cell, const quint8 &type = 0);

    void setLabelWinTitle(QString title);

    int getUserId() const;

    // add msg to this
    void addMessage(const json js);

     void getGroupusers(vector<GroupUser> groupusers);
signals:
    void sig_sendMessage(const json &js);

    void sig_close();

public slots:
    void on_btnSendMsg_clicked();

    void on_closeBtn_clicked();
public:
    QVBoxLayout *m_mainLayout=nullptr;

    QWidget *m_titleBar=nullptr;



    BubbleList *m_widgetBubble=nullptr;


private:
    // 用户相关信息保存
    QQCell *m_cell;
    QStandardItemModel *m_model;
    QLabel *m_labelWinTitle=nullptr;
    QTableView *m_tableViewGroups=nullptr ;

    QWidget *m_widgetFileInfo=nullptr;
    QWidget *m_widgetFileBoard=nullptr;

    QPushButton *m_btnSendMsg=nullptr;

    QWidget *m_widgetMsg=nullptr;
    QTextEdit *m_textEditMsg=nullptr;
    // 文件传输
    //ClientFileSocket *m_tcpFileSocket;
    QString          m_strFileName;
    QTime            m_updateTime;

    quint8          m_nFileType;

    quint8          m_nChatType;        // 聊天类型，群组聊天或私人聊天    

    vector<GroupUser> m_groupusers;
    int m_rowCount=0;
};

#endif // CCHATWINDOW_H

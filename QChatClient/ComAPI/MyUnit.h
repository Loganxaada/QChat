#ifndef MYUNIT_H
#define MYUNIT_H

/* 聊天气泡相关设置 */
#define ITEM_START_ANGLE 270
#define ITEM_D_ANGLE 2
#define ITEM_D_ZOOMING_FACTOR 0.05
#define UPDATE_TIMER_INTERVAL 10 // ms
#define ITEM_COUNTER_TIMER_INTERVAL 200
#define SIZE_HINT QSize(300,300)

#define ITEM_HEIGHT 40
#define BG_START_COLOR QColor(179,179,183)
#define BG_END_COLOR QColor(187,193,207)

#define ITEM_START_COLOR ITEM_END_COLOR.lighter()
#define ITEM_END_COLOR QColor(62,135,166)

#define ITEM_START_COLOR_A ITEM_END_COLOR_A.lighter()
#define ITEM_END_COLOR_A QColor(221,221,221)

#define HOVER_OPACITY 0.2
#define HOVER_START_COLOR HOVER_END_COLOR.lighter()
#define HOVER_END_COLOR QColor(98,139,21)

#define HOVER_TEXT_COLOR Qt::red
#define SELECT_TEXT_COLOR Qt::white
#define NORMAL_TEXT_COLOR Qt::black

#define RECT_RADIUS 4

#define ITEM_SPACE          20
#define ITEM_TITLE_HEIGHT   20

// 气泡方向
typedef enum
{
    None,
    Left,
    Right
} Orientation;

typedef enum
{
    TEXT,    // 普通文字消息
    AUDIO,   // 语音消息
    PICTURE, // 图片消息
    FILES,   // 文件传输
} MessageType;

enum EnMsgType
{
    LOGIN_MSG = 1, // 登录消息
    LOGIN_MSG_ACK, // 登录响应消息
    LOGINOUT_MSG, // 注销消息
    REG_MSG, // 注册消息
    REG_MSG_ACK, // 注册响应消息
    ONE_CHAT_MSG, // 聊天消息
    ADD_FRIEND_MSG, // 添加好友消息
    ADD_FRIEND_ACK,
    CREATE_GROUP_MSG, // 创建群组
    CREATE_GROUP_ACK,
    ADD_GROUP_MSG, // 加入群组
    ADD_GROUP_ACK,
    GROUP_CHAT_MSG, // 群聊天

    DEL_FRIEND_MSG,// delete friend
    DEL_FRIEND_ACK,// delete friend ack

    FRIEND_STATE_CHANGE,
    UPDATE_SIGNATURE,
};

typedef enum {
    ConnectedHost = 0x01,
    DisConnectedHost,

    LoginSuccess,       // 登录成功
    LoginPasswdError,   // 密码错误

    ONLINE,
    OFFLINE,

    RegisterOk,
    RegisterFailed,

    AddFriendOk,
    AddFriendFailed,
} EnSTATUS;

#endif // MYUNIT_H

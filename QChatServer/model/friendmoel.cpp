#include "friendmodel.hpp"
#include "db/db.h"
#include<QDebug>

// 添加好友关系
bool FriendModel::insert(int userid, int friendid)
{
    // 1.组装SQL语句
       char sql[1024] = {0};
       sprintf(sql, "insert into Friend values(%d, %d)", userid, friendid);

       MySQL mysql;
       if (mysql.connect())
       {
           // 执行SQL语句并判断结果
           return mysql.update(sql);
       }

       // 连接数据库失败，返回false
       return false;
}

// 返回用户好友列表
vector<User> FriendModel::query(int userid)
{
    // 1.组装sql语句
    char sql[1024] = {0};

//    sprintf(sql, "select a.id,a.name,a.state,a.signature from User a inner join Friend b on b.friendid = a.id where b.userid=%d", userid);
    sprintf(sql, "select a.id, a.name, a.state, a.signature from User a inner join Friend b on (b.friendid = a.id or b.userid = a.id) where (b.userid = %d or b.friendid = %d) and a.id != %d", userid, userid, userid);

    vector<User> vec;
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            // 把userid用户的所有离线消息放入vec中返回
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr)
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);

                user.setSignature(row[3]);
                vec.push_back(user);
            }
            mysql_free_result(res);
            return vec;
        }
    }
    return vec;
}

bool FriendModel::delFriend(int userid, int friendid)
{
    // 1.组装sql语句
    char sql[1024] = {0};

    // 删除用户和好友之间的关系（删除两条记录）
    sprintf(sql, "delete from Friend where (userid = %d and friendid = %d) or (userid = %d and friendid = %d)", userid, friendid, friendid, userid);

    qDebug()<<"del friend sql"<<sql;

    // 执行 SQL 语句
    MySQL mysql;
    if (mysql.connect())
    {
        return mysql.update(sql);
    }else{
        return false;
    }
}

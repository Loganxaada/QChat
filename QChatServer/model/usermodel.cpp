#include "usermodel.hpp"
#include "db/db.h"
#include <iostream>
using namespace std;

// User表的增加方法
bool UserModel::insert(User &user)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into User(name, password, state) values('%s', '%s', '%s')",
            user.getName().c_str(), user.getPwd().c_str(), user.getState().c_str());
    
    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            // 获取插入成功的用户数据生成的主键id
            user.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    
    return false;
}

// 根据用户号码查询用户信息
User UserModel::query(int id)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select * from User where id = %d", id);
    
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row != nullptr)
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);
                user.setSignature(row[4]);
                mysql_free_result(res);
                return user;
            }
        }
    }
    
    return User();
}

vector<User> UserModel::queryAll()
{
    std::vector<User> userList;
    
    // 1. 组装 SQL 语句
    std::string sql = "select * from User";
    
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);
                
                userList.push_back(user);
            }
            mysql_free_result(res);
        }
    }
    
    return userList;
}

int UserModel::count()
{
    int total = 0;
    std::string sql = "select count(*) from User";

    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row != nullptr)
            {
                total = atoi(row[0]);
            }
            mysql_free_result(res);
        }
    }

    return total;
}

vector<User> UserModel::queryPage(int offset, int limit)
{
    std::vector<User> userList;
    char sql[1024] = {0};
    sprintf(sql, "select * from User limit %d offset %d", limit, offset);

    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);
                user.setSignature(row[4]);

                userList.push_back(user);
            }
            mysql_free_result(res);
        }
    }

    return userList;
}

vector<User> UserModel::queryNewRecords(int lastId)
{
    vector<User> userList;
    char sql[1024] = {0};
    sprintf(sql, "select * from User where id > %d order by id asc", lastId);

    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);

                userList.push_back(user);
            }
            mysql_free_result(res);
        }
    }

    return userList;
}

// 更新用户的状态信息
bool UserModel::updateState(User user)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "update User set state = '%s' where id = %d", user.getState().c_str(), user.getId());
    
    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}

bool UserModel::updateSignature(int userid,string signature)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "update User set signature = '%s' where id = %d", signature.c_str(), userid);

    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}

// 重置用户的状态信息
void UserModel::resetState()
{
    // 1.组装sql语句
    char sql[1024] = "update User set state = 'offline' where state = 'online'";
    
    MySQL mysql;
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}

#ifndef USERMODEL_H
#define USERMODEL_H

#include "user.hpp"
#include<vector>

// User表的数据操作类
class UserModel {
public:
    // User表的增加方法
    bool insert(User &user);

    // 根据用户号码查询用户信息
    User query(int id);
    
    vector<User> queryAll();

    int count();

    vector<User> queryPage(int offset, int limit);

    vector<User> queryNewRecords(int lastId);

    // 更新用户的状态信息
    bool updateState(User user);

    //更改userid的signature
    bool updateSignature(int userid,string signature);

    // 重置用户的状态信息
    void resetState();
};

#endif

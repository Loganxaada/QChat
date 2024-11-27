#ifndef MANAGERMODEL_H
#define MANAGERMODEL_H

#include"manager.h"

class ManagerModel
{
public:

    bool insert(Manager &manager);

    // 根据用户号码查询用户信息
    Manager query(int id);

    // 根据用户号码查询用户信息
    Manager query(string name);
};

#endif // MANAGERMODEL_H

#include "managermodel.h"
#include"db/db.h"

bool ManagerModel::insert(Manager &manager)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into ServerManager(name, pwd) values('%s','%s')", manager.getName().c_str(),manager.getPwd().c_str());

    MySQL mysql;
    if(mysql.connect())
    {
        if (mysql.update(sql))
        {
            // 获取插入成功的用户数据生成的主键id
            manager.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}

Manager ManagerModel::query(int id)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select * from ServerManager where id = %d", id);

    MySQL mysql;
    if(mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if(res!=nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row != nullptr)
            {
                Manager manager;
                manager.setId(atoi(row[0]));
                manager.setName(row[1]);
                manager.setPwd(row[2]);

                mysql_free_result(res);

                return manager;
            }
        }
    }
    return Manager();
}

Manager ManagerModel::query(string name)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select * from ServerManager where name = '%s'", name.c_str());

    MySQL mysql;
    if(mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if(res!=nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row != nullptr)
            {
                Manager manager;
                manager.setId(atoi(row[0]));
                manager.setName(row[1]);
                manager.setPwd(row[2]);

                mysql_free_result(res);

                return manager;
            }
        }
    }
    return Manager();
}

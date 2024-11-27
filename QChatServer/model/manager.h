#ifndef MANAGER_H
#define MANAGER_H

#include<string>
using namespace std;

class Manager
{
public:
    Manager(int id=-1,string name="",string pwd="");

    void setId(int id);
    void setName(string name);
    void setPwd(string pwd);

    int getId();
    string getName();
    string getPwd();


private:
    int id;
    string name;
    string password;
};

#endif // MANAGER_H

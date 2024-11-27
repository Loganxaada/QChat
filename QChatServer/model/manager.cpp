#include "manager.h"


Manager::Manager(int id, string name, string pwd)
    :id(id)
    ,name(name)
    ,password(pwd)
{

}

void Manager::setId(int id)
{
    this->id = id;
}

void Manager::setName(string name)
{
    this->name = name;
}

void Manager::setPwd(string pwd)
{
    this->password = pwd;
}

int Manager::getId()
{
    return this->id;
}

string Manager::getName()
{
    return this->name;
}

string Manager::getPwd()
{
    return this->password;
}


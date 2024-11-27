#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

#include <string>
using namespace std;

// User表的ORM类
class User
{
public:
    User(int id = -1, string name = "", string pwd = "", string state = "offline",string signature="")
    {
        this->id = id;
        this->name = name;
        this->password = pwd;
        this->state = state;
        this->signature=signature;
    }

    void setId(int id) { this->id = id; }
    void setName(string name) { this->name = name; }
    void setPwd(string pwd) { this->password = pwd; }
    void setState(string state) { this->state = state; }
    void setSignature(string signature){this->signature=signature;}

    int getId() const { return this->id; }
    string getName() const { return this->name; }
    string getPwd() const { return this->password; }
    string getState() const { return this->state; }
    string getSignture() const { return this->signature; }

protected:
    int id;
    string name;
    string password;
    string state;
    string signature;
};


#endif

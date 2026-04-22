#ifndef USER_H
#define USER_H

#include <string>

struct User {
    std::string username;
    std::string password;
    std::string name;
    std::string mailAddr;
    int privilege;

    User() : privilege(0) {}
    User(const std::string& u, const std::string& p, const std::string& n,
         const std::string& m, int g)
        : username(u), password(p), name(n), mailAddr(m), privilege(g) {}
};

class UserManager {
private:
    static const int MAX_USERS = 10000;
    static const int HASH_SIZE = 20011;

    User* users;
    int userCount;
    int* userHash;
    int* userNext;

    bool* loggedInUsers;

    int hash(const std::string& username);
    int findUser(const std::string& username);

public:
    UserManager();
    ~UserManager();

    int addUser(const std::string& curUser, const std::string& username,
                const std::string& password, const std::string& name,
                const std::string& mailAddr, int privilege);

    int login(const std::string& username, const std::string& password);
    int logout(const std::string& username);

    std::string queryProfile(const std::string& curUser, const std::string& targetUser);
    std::string modifyProfile(const std::string& curUser, const std::string& targetUser,
                             const std::string& password, const std::string& name,
                             const std::string& mailAddr, int privilege);

    bool isLoggedIn(const std::string& username);
    int getUserPrivilege(const std::string& username);
};

#endif
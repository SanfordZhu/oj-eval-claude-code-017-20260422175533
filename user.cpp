#include "user.h"
#include <iostream>

UserManager::UserManager() : userCount(0) {
    users = new User[MAX_USERS];
    userHash = new int[HASH_SIZE];
    userNext = new int[MAX_USERS];
    loggedInUsers = new bool[MAX_USERS];

    for (int i = 0; i < HASH_SIZE; i++) {
        userHash[i] = -1;
    }
    for (int i = 0; i < MAX_USERS; i++) {
        userNext[i] = -1;
        loggedInUsers[i] = false;
    }
}

UserManager::~UserManager() {
    delete[] users;
    delete[] userHash;
    delete[] userNext;
    delete[] loggedInUsers;
}

int UserManager::hash(const std::string& username) {
    unsigned long long hash = 0;
    for (char c : username) {
        hash = hash * 131 + c;
    }
    return hash % HASH_SIZE;
}

int UserManager::findUser(const std::string& username) {
    int h = hash(username);
    int index = userHash[h];

    while (index != -1) {
        if (users[index].username == username) {
            return index;
        }
        index = userNext[index];
    }
    return -1;
}

int UserManager::addUser(const std::string& curUser, const std::string& username,
                        const std::string& password, const std::string& name,
                        const std::string& mailAddr, int privilege) {
    if (userCount == 0) {
        users[userCount] = User(username, password, name, mailAddr, 10);
        int h = hash(username);
        userNext[userCount] = userHash[h];
        userHash[h] = userCount;
        userCount++;
        return 0;
    }

    if (findUser(username) != -1) return -1;

    int curIndex = findUser(curUser);
    if (curIndex == -1 || !loggedInUsers[curIndex]) return -1;
    if (users[curIndex].privilege <= privilege) return -1;

    users[userCount] = User(username, password, name, mailAddr, privilege);
    int h = hash(username);
    userNext[userCount] = userHash[h];
    userHash[h] = userCount;
    userCount++;
    return 0;
}

int UserManager::login(const std::string& username, const std::string& password) {
    int index = findUser(username);
    if (index == -1) return -1;
    if (users[index].password != password) return -1;
    if (loggedInUsers[index]) return -1;

    loggedInUsers[index] = true;
    return 0;
}

int UserManager::logout(const std::string& username) {
    int index = findUser(username);
    if (index == -1) return -1;
    if (!loggedInUsers[index]) return -1;

    loggedInUsers[index] = false;
    return 0;
}

std::string UserManager::queryProfile(const std::string& curUser, const std::string& targetUser) {
    int curIndex = findUser(curUser);
    if (curIndex == -1 || !loggedInUsers[curIndex]) return "-1";

    int targetIndex = findUser(targetUser);
    if (targetIndex == -1) return "-1";

    if (users[curIndex].privilege <= users[targetIndex].privilege && curUser != targetUser) {
        return "-1";
    }

    return users[targetIndex].username + " " + users[targetIndex].name + " " +
           users[targetIndex].mailAddr + " " + std::to_string(users[targetIndex].privilege);
}

std::string UserManager::modifyProfile(const std::string& curUser, const std::string& targetUser,
                                      const std::string& password, const std::string& name,
                                      const std::string& mailAddr, int privilege) {
    int curIndex = findUser(curUser);
    if (curIndex == -1 || !loggedInUsers[curIndex]) return "-1";

    int targetIndex = findUser(targetUser);
    if (targetIndex == -1) return "-1";

    if (users[curIndex].privilege <= users[targetIndex].privilege && curUser != targetUser) {
        return "-1";
    }
    if (privilege != -1 && users[curIndex].privilege <= privilege) {
        return "-1";
    }

    if (!password.empty()) users[targetIndex].password = password;
    if (!name.empty()) users[targetIndex].name = name;
    if (!mailAddr.empty()) users[targetIndex].mailAddr = mailAddr;
    if (privilege != -1) users[targetIndex].privilege = privilege;

    return users[targetIndex].username + " " + users[targetIndex].name + " " +
           users[targetIndex].mailAddr + " " + std::to_string(users[targetIndex].privilege);
}

bool UserManager::isLoggedIn(const std::string& username) {
    int index = findUser(username);
    return index != -1 && loggedInUsers[index];
}

int UserManager::getUserPrivilege(const std::string& username) {
    int index = findUser(username);
    return index != -1 ? users[index].privilege : -1;
}
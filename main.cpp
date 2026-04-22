#include <iostream>
#include <string>
#include <cstring>
#include "user.h"
#include "train.h"
#include "ticket.h"
#include "utility.h"

UserManager* userManager = nullptr;
TrainManager* trainManager = nullptr;
TicketManager* ticketManager = nullptr;

void initializeManagers() {
    static UserManager um;
    static TrainManager tm;
    static TicketManager tkm;

    userManager = &um;
    trainManager = &tm;
    ticketManager = &tkm;
}

void handleAddUser(const std::string& params) {
    std::string parts[10];
    int count = StringUtil::split(params, " ", parts, 10);

    std::string curUser, username, password, name, mailAddr;
    int privilege = 0;

    for (int i = 0; i < count; i += 2) {
        if (parts[i] == "-c") curUser = parts[i + 1];
        else if (parts[i] == "-u") username = parts[i + 1];
        else if (parts[i] == "-p") password = parts[i + 1];
        else if (parts[i] == "-n") name = parts[i + 1];
        else if (parts[i] == "-m") mailAddr = parts[i + 1];
        else if (parts[i] == "-g") privilege = StringUtil::parseInt(parts[i + 1]);
    }

    int result = userManager->addUser(curUser, username, password, name, mailAddr, privilege);
    std::cout << result << std::endl;
}

void handleLogin(const std::string& params) {
    std::string parts[4];
    int count = StringUtil::split(params, " ", parts, 4);

    std::string username, password;
    for (int i = 0; i < count; i += 2) {
        if (parts[i] == "-u") username = parts[i + 1];
        else if (parts[i] == "-p") password = parts[i + 1];
    }

    int result = userManager->login(username, password);
    std::cout << result << std::endl;
}

void handleLogout(const std::string& params) {
    std::string parts[2];
    int count = StringUtil::split(params, " ", parts, 2);

    std::string username;
    for (int i = 0; i < count; i += 2) {
        if (parts[i] == "-u") username = parts[i + 1];
    }

    int result = userManager->logout(username);
    std::cout << result << std::endl;
}

void handleQueryProfile(const std::string& params) {
    std::string parts[4];
    int count = StringUtil::split(params, " ", parts, 4);

    std::string curUser, targetUser;
    for (int i = 0; i < count; i += 2) {
        if (parts[i] == "-c") curUser = parts[i + 1];
        else if (parts[i] == "-u") targetUser = parts[i + 1];
    }

    std::string result = userManager->queryProfile(curUser, targetUser);
    std::cout << result << std::endl;
}

void handleModifyProfile(const std::string& params) {
    std::string parts[12];
    int count = StringUtil::split(params, " ", parts, 12);

    std::string curUser, targetUser, password, name, mailAddr;
    int privilege = -1;
    bool hasPassword = false, hasName = false, hasMail = false, hasPrivilege = false;

    for (int i = 0; i < count; i += 2) {
        if (parts[i] == "-c") curUser = parts[i + 1];
        else if (parts[i] == "-u") targetUser = parts[i + 1];
        else if (parts[i] == "-p") { password = parts[i + 1]; hasPassword = true; }
        else if (parts[i] == "-n") { name = parts[i + 1]; hasName = true; }
        else if (parts[i] == "-m") { mailAddr = parts[i + 1]; hasMail = true; }
        else if (parts[i] == "-g") { privilege = StringUtil::parseInt(parts[i + 1]); hasPrivilege = true; }
    }

    if (!hasPassword) password = "";
    if (!hasName) name = "";
    if (!hasMail) mailAddr = "";
    if (!hasPrivilege) privilege = -1;

    std::string result = userManager->modifyProfile(curUser, targetUser, password, name, mailAddr, privilege);
    std::cout << result << std::endl;
}

void handleAddTrain(const std::string& params) {
    std::string parts[22];
    int count = StringUtil::split(params, " ", parts, 22);

    std::string trainID, stations, prices, startTime, travelTimes, stopoverTimes, saleDate, type;
    int stationNum = 0, seatNum = 0;

    for (int i = 0; i < count; i += 2) {
        if (parts[i] == "-i") trainID = parts[i + 1];
        else if (parts[i] == "-n") stationNum = StringUtil::parseInt(parts[i + 1]);
        else if (parts[i] == "-m") seatNum = StringUtil::parseInt(parts[i + 1]);
        else if (parts[i] == "-s") stations = parts[i + 1];
        else if (parts[i] == "-p") prices = parts[i + 1];
        else if (parts[i] == "-x") startTime = parts[i + 1];
        else if (parts[i] == "-t") travelTimes = parts[i + 1];
        else if (parts[i] == "-o") stopoverTimes = parts[i + 1];
        else if (parts[i] == "-d") saleDate = parts[i + 1];
        else if (parts[i] == "-y") type = parts[i + 1];
    }

    std::string stationList[100];
    int priceList[99];
    int travelList[99];
    int stopoverList[98];

    std::string stationParts[100];
    int stationCount = StringUtil::split(stations, "|", stationParts, 100);
    for (int i = 0; i < stationCount; i++) {
        stationList[i] = stationParts[i];
    }

    std::string priceParts[99];
    int priceCount = StringUtil::split(prices, "|", priceParts, 99);
    for (int i = 0; i < priceCount; i++) {
        priceList[i] = StringUtil::parseInt(priceParts[i]);
    }

    std::string timeParts[99];
    int timeCount = StringUtil::split(travelTimes, "|", timeParts, 99);
    for (int i = 0; i < timeCount; i++) {
        travelList[i] = StringUtil::parseInt(timeParts[i]);
    }

    std::string stopParts[98];
    int stopCount = StringUtil::split(stopoverTimes, "|", stopParts, 98);
    for (int i = 0; i < stopCount; i++) {
        stopoverList[i] = StringUtil::parseInt(stopParts[i]);
    }

    std::string saleParts[2];
    StringUtil::split(saleDate, "|", saleParts, 2);

    int startHour = StringUtil::parseInt(startTime.substr(0, 2));
    int startMin = StringUtil::parseInt(startTime.substr(3, 2));

    int result = trainManager->addTrain(trainID, stationNum, seatNum, stationList, priceList,
                                      startHour, startMin, travelList, stopoverList,
                                      saleParts[0], saleParts[1], type[0]);
    std::cout << result << std::endl;
}

void handleReleaseTrain(const std::string& params) {
    std::string parts[2];
    int count = StringUtil::split(params, " ", parts, 2);

    std::string trainID;
    for (int i = 0; i < count; i += 2) {
        if (parts[i] == "-i") trainID = parts[i + 1];
    }

    int result = trainManager->releaseTrain(trainID);
    std::cout << result << std::endl;
}

void handleQueryTrain(const std::string& params) {
    std::string parts[4];
    int count = StringUtil::split(params, " ", parts, 4);

    std::string trainID, date;
    for (int i = 0; i < count; i += 2) {
        if (parts[i] == "-i") trainID = parts[i + 1];
        else if (parts[i] == "-d") date = parts[i + 1];
    }

    std::string result = trainManager->queryTrain(trainID, date);
    std::cout << result << std::endl;
}

void handleDeleteTrain(const std::string& params) {
    std::string parts[2];
    int count = StringUtil::split(params, " ", parts, 2);

    std::string trainID;
    for (int i = 0; i < count; i += 2) {
        if (parts[i] == "-i") trainID = parts[i + 1];
    }

    int result = trainManager->deleteTrain(trainID);
    std::cout << result << std::endl;
}

void handleQueryTicket(const std::string& params) {
    std::string parts[10];
    int count = StringUtil::split(params, " ", parts, 10);

    std::string fromStation, toStation, date, sortBy = "time";
    for (int i = 0; i < count; i += 2) {
        if (parts[i] == "-s") fromStation = parts[i + 1];
        else if (parts[i] == "-t") toStation = parts[i + 1];
        else if (parts[i] == "-d") date = parts[i + 1];
        else if (parts[i] == "-p") sortBy = parts[i + 1];
    }

    TicketQuery results[1000];
    int countResult = ticketManager->queryTicket(fromStation, toStation, date, sortBy, results, 1000);

    std::cout << countResult << std::endl;
    for (int i = 0; i < countResult; i++) {
        std::cout << results[i].trainID << " " << results[i].fromStation << " "
                  << results[i].departureTime << " -> " << results[i].toStation << " "
                  << results[i].arrivalTime << " " << results[i].price << " "
                  << results[i].availableSeats << std::endl;
    }
}

void handleQueryTransfer(const std::string& params) {
    std::string parts[10];
    int count = StringUtil::split(params, " ", parts, 10);

    std::string fromStation, toStation, date, sortBy = "time";
    for (int i = 0; i < count; i += 2) {
        if (parts[i] == "-s") fromStation = parts[i + 1];
        else if (parts[i] == "-t") toStation = parts[i + 1];
        else if (parts[i] == "-d") date = parts[i + 1];
        else if (parts[i] == "-p") sortBy = parts[i + 1];
    }

    TicketQuery result1, result2;
    int result = ticketManager->queryTransfer(fromStation, toStation, date, sortBy, result1, result2);

    if (result == 0) {
        std::cout << "0" << std::endl;
    } else {
        std::cout << result1.trainID << " " << result1.fromStation << " "
                  << result1.departureTime << " -> " << result1.toStation << " "
                  << result1.arrivalTime << " " << result1.price << " "
                  << result1.availableSeats << std::endl;
        std::cout << result2.trainID << " " << result2.fromStation << " "
                  << result2.departureTime << " -> " << result2.toStation << " "
                  << result2.arrivalTime << " " << result2.price << " "
                  << result2.availableSeats << std::endl;
    }
}

void handleBuyTicket(const std::string& params) {
    std::string parts[14];
    int count = StringUtil::split(params, " ", parts, 14);

    std::string username, trainID, date, fromStation, toStation;
    int numTickets = 0;
    bool queueIfUnavailable = false;

    for (int i = 0; i < count; i += 2) {
        if (parts[i] == "-u") username = parts[i + 1];
        else if (parts[i] == "-i") trainID = parts[i + 1];
        else if (parts[i] == "-d") date = parts[i + 1];
        else if (parts[i] == "-n") numTickets = StringUtil::parseInt(parts[i + 1]);
        else if (parts[i] == "-f") fromStation = parts[i + 1];
        else if (parts[i] == "-t") toStation = parts[i + 1];
        else if (parts[i] == "-q") queueIfUnavailable = (parts[i + 1] == "true");
    }

    std::string result = ticketManager->buyTicket(username, trainID, date, numTickets,
                                                fromStation, toStation, queueIfUnavailable);
    std::cout << result << std::endl;
}

void handleQueryOrder(const std::string& params) {
    std::string parts[2];
    int count = StringUtil::split(params, " ", parts, 2);

    std::string username;
    for (int i = 0; i < count; i += 2) {
        if (parts[i] == "-u") username = parts[i + 1];
    }

    Order results[1000];
    int countResult = ticketManager->queryOrder(username, results, 1000);

    std::cout << countResult << std::endl;
    for (int i = 0; i < countResult; i++) {
        std::cout << "[" << results[i].status << "] " << results[i].trainID << " "
                  << results[i].fromStation << " " << results[i].date << " "
                  << results[i].toStation << " " << results[i].date << " "
                  << results[i].totalPrice << " " << results[i].numTickets << std::endl;
    }
}

void handleRefundTicket(const std::string& params) {
    std::string parts[4];
    int count = StringUtil::split(params, " ", parts, 4);

    std::string username;
    int orderIndex = 1;
    for (int i = 0; i < count; i += 2) {
        if (parts[i] == "-u") username = parts[i + 1];
        else if (parts[i] == "-n") orderIndex = StringUtil::parseInt(parts[i + 1]);
    }

    int result = ticketManager->refundTicket(username, orderIndex);
    std::cout << result << std::endl;
}

void handleClean() {
    // Reinitialize managers
    initializeManagers();
    std::cout << "0" << std::endl;
}

void handleExit() {
    std::cout << "bye" << std::endl;
    exit(0);
}

int main() {
    initializeManagers();
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        size_t spacePos = line.find(' ');
        std::string command = line.substr(0, spacePos);
        std::string params = (spacePos != std::string::npos) ? line.substr(spacePos + 1) : "";

        if (command == "add_user") {
            handleAddUser(params);
        } else if (command == "login") {
            handleLogin(params);
        } else if (command == "logout") {
            handleLogout(params);
        } else if (command == "query_profile") {
            handleQueryProfile(params);
        } else if (command == "modify_profile") {
            handleModifyProfile(params);
        } else if (command == "add_train") {
            handleAddTrain(params);
        } else if (command == "release_train") {
            handleReleaseTrain(params);
        } else if (command == "query_train") {
            handleQueryTrain(params);
        } else if (command == "delete_train") {
            handleDeleteTrain(params);
        } else if (command == "query_ticket") {
            handleQueryTicket(params);
        } else if (command == "query_transfer") {
            handleQueryTransfer(params);
        } else if (command == "buy_ticket") {
            handleBuyTicket(params);
        } else if (command == "query_order") {
            handleQueryOrder(params);
        } else if (command == "refund_ticket") {
            handleRefundTicket(params);
        } else if (command == "clean") {
            handleClean();
        } else if (command == "exit") {
            handleExit();
        }
    }

    return 0;
}
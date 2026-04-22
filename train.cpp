#include "train.h"
#include "utility.h"
#include <iostream>
#include <iomanip>

std::string StationTime::toString() const {
    std::string result;
    if (month == 0 && day == 0) {
        result = "xx-xx xx:xx";
    } else {
        result = std::to_string(month) + "-" + std::to_string(day) + " ";
        if (hour < 10) result += "0";
        result += std::to_string(hour) + ":";
        if (min < 10) result += "0";
        result += std::to_string(min);
    }
    return result;
}

TrainManager::TrainManager() : trainCount(0) {
    trains = new Train[MAX_TRAINS];
    trainHash = new int[HASH_SIZE];
    trainNext = new int[MAX_TRAINS];

    for (int i = 0; i < HASH_SIZE; i++) {
        trainHash[i] = -1;
    }
    for (int i = 0; i < MAX_TRAINS; i++) {
        trainNext[i] = -1;
    }
}

TrainManager::~TrainManager() {
    delete[] trains;
    delete[] trainHash;
    delete[] trainNext;
}

int TrainManager::hash(const std::string& trainID) {
    unsigned long long hash = 0;
    for (char c : trainID) {
        hash = hash * 131 + c;
    }
    return hash % HASH_SIZE;
}

int TrainManager::findTrain(const std::string& trainID) {
    int h = hash(trainID);
    int index = trainHash[h];

    while (index != -1) {
        if (trains[index].trainID == trainID) {
            return index;
        }
        index = trainNext[index];
    }
    return -1;
}

int TrainManager::addTrain(const std::string& trainID, int stationNum, int seatNum,
                          const std::string* stations, const int* prices,
                          int startTimeHour, int startTimeMin,
                          const int* travelTimes, const int* stopoverTimes,
                          const std::string& saleDateStart, const std::string& saleDateEnd,
                          char type) {
    if (findTrain(trainID) != -1) return -1;
    if (stationNum < 2 || stationNum > 100) return -1;
    if (seatNum <= 0 || seatNum > 100000) return -1;

    trains[trainCount].trainID = trainID;
    trains[trainCount].stationNum = stationNum;
    trains[trainCount].seatNum = seatNum;
    trains[trainCount].startTimeHour = startTimeHour;
    trains[trainCount].startTimeMin = startTimeMin;
    trains[trainCount].saleDateStart = saleDateStart;
    trains[trainCount].saleDateEnd = saleDateEnd;
    trains[trainCount].type = type;
    trains[trainCount].isReleased = false;

    for (int i = 0; i < stationNum; i++) {
        trains[trainCount].stations[i] = stations[i];
    }

    for (int i = 0; i < stationNum - 1; i++) {
        trains[trainCount].prices[i] = prices[i];
    }

    for (int i = 0; i < stationNum - 1; i++) {
        trains[trainCount].travelTimes[i] = travelTimes[i];
    }

    for (int i = 0; i < stationNum - 2; i++) {
        trains[trainCount].stopoverTimes[i] = stopoverTimes[i];
    }

    int h = hash(trainID);
    trainNext[trainCount] = trainHash[h];
    trainHash[h] = trainCount;
    trainCount++;

    return 0;
}

int TrainManager::releaseTrain(const std::string& trainID) {
    int index = findTrain(trainID);
    if (index == -1) return -1;
    if (trains[index].isReleased) return -1;

    trains[index].isReleased = true;
    return 0;
}

std::string TrainManager::queryTrain(const std::string& trainID, const std::string& date) {
    int index = findTrain(trainID);
    if (index == -1) return "-1";

    Train& train = trains[index];
    std::string result = trainID + " " + train.type + "\n";

    int queryMonth = StringUtil::parseInt(date.substr(0, 2));
    int queryDay = StringUtil::parseInt(date.substr(3, 2));

    int saleStartMonth = StringUtil::parseInt(train.saleDateStart.substr(0, 2));
    int saleStartDay = StringUtil::parseInt(train.saleDateStart.substr(3, 2));
    int saleEndMonth = StringUtil::parseInt(train.saleDateEnd.substr(0, 2));
    int saleEndDay = StringUtil::parseInt(train.saleDateEnd.substr(3, 2));

    int queryDate = queryMonth * 100 + queryDay;
    int saleStartDate = saleStartMonth * 100 + saleStartDay;
    int saleEndDate = saleEndMonth * 100 + saleEndDay;

    if (queryDate < saleStartDate || queryDate > saleEndDate) {
        return "-1";
    }

    StationTime currentTime(queryMonth, queryDay, train.startTimeHour, train.startTimeMin);
    int cumulativePrice = 0;

    for (int i = 0; i < train.stationNum; i++) {
        result += train.stations[i] + " ";

        if (i == 0) {
            result += "xx-xx xx:xx -> ";
            result += currentTime.toString() + " ";
            result += "0 ";
            if (i == train.stationNum - 1) {
                result += "x";
            } else {
                result += std::to_string(train.seatNum);
            }
        } else if (i == train.stationNum - 1) {
            StationTime arrivalTime = currentTime;
            result += arrivalTime.toString() + " -> xx-xx xx:xx ";
            cumulativePrice += train.prices[i - 1];
            result += std::to_string(cumulativePrice) + " x";
        } else {
            StationTime arrivalTime = currentTime;
            currentTime.min += train.stopoverTimes[i - 1];
            if (currentTime.min >= 60) {
                currentTime.hour += currentTime.min / 60;
                currentTime.min %= 60;
                if (currentTime.hour >= 24) {
                    currentTime.day += currentTime.hour / 24;
                    currentTime.hour %= 24;
                }
            }

            result += arrivalTime.toString() + " -> ";
            result += currentTime.toString() + " ";

            cumulativePrice += train.prices[i - 1];
            result += std::to_string(cumulativePrice) + " ";
            result += std::to_string(train.seatNum);
        }

        if (i < train.stationNum - 1) {
            currentTime.min += train.travelTimes[i];
            if (currentTime.min >= 60) {
                currentTime.hour += currentTime.min / 60;
                currentTime.min %= 60;
                if (currentTime.hour >= 24) {
                    currentTime.day += currentTime.hour / 24;
                    currentTime.hour %= 24;
                }
            }
        }

        if (i < train.stationNum - 1) {
            result += "\n";
        }
    }

    return result;
}

int TrainManager::deleteTrain(const std::string& trainID) {
    int index = findTrain(trainID);
    if (index == -1) return -1;
    if (trains[index].isReleased) return -1;

    int h = hash(trainID);
    int prev = -1;
    int current = trainHash[h];

    while (current != -1) {
        if (current == index) {
            if (prev == -1) {
                trainHash[h] = trainNext[current];
            } else {
                trainNext[prev] = trainNext[current];
            }
            break;
        }
        prev = current;
        current = trainNext[current];
    }

    return 0;
}

bool TrainManager::trainExists(const std::string& trainID) {
    return findTrain(trainID) != -1;
}

bool TrainManager::isTrainReleased(const std::string& trainID) {
    int index = findTrain(trainID);
    return index != -1 && trains[index].isReleased;
}

int TrainManager::getStationIndex(const std::string& trainID, const std::string& station) {
    int index = findTrain(trainID);
    if (index == -1) return -1;

    for (int i = 0; i < trains[index].stationNum; i++) {
        if (trains[index].stations[i] == station) {
            return i;
        }
    }
    return -1;
}

int TrainManager::getSeatNum(const std::string& trainID) {
    int index = findTrain(trainID);
    return index != -1 ? trains[index].seatNum : 0;
}

int TrainManager::getPrice(const std::string& trainID, int fromIndex, int toIndex) {
    int index = findTrain(trainID);
    if (index == -1) return 0;

    int price = 0;
    for (int i = fromIndex; i < toIndex; i++) {
        price += trains[index].prices[i];
    }
    return price;
}

StationTime TrainManager::getDepartureTime(const std::string& trainID, int stationIndex, const std::string& date) {
    int index = findTrain(trainID);
    if (index == -1) return StationTime();

    Train& train = trains[index];
    int month = StringUtil::parseInt(date.substr(0, 2));
    int day = StringUtil::parseInt(date.substr(3, 2));

    if (stationIndex == 0) {
        return StationTime(month, day, train.startTimeHour, train.startTimeMin);
    }

    StationTime currentTime(month, day, train.startTimeHour, train.startTimeMin);

    for (int i = 0; i < stationIndex; i++) {
        currentTime.min += train.travelTimes[i];
        if (currentTime.min >= 60) {
            currentTime.hour += currentTime.min / 60;
            currentTime.min %= 60;
            if (currentTime.hour >= 24) {
                currentTime.day += currentTime.hour / 24;
                currentTime.hour %= 24;
            }
        }

        if (i < stationIndex - 1) {
            currentTime.min += train.stopoverTimes[i];
            if (currentTime.min >= 60) {
                currentTime.hour += currentTime.min / 60;
                currentTime.min %= 60;
                if (currentTime.hour >= 24) {
                    currentTime.day += currentTime.hour / 24;
                    currentTime.hour %= 24;
                }
            }
        }
    }

    return currentTime;
}

StationTime TrainManager::getArrivalTime(const std::string& trainID, int stationIndex, const std::string& date) {
    if (stationIndex == 0) {
        return StationTime();
    }

    int index = findTrain(trainID);
    if (index == -1) return StationTime();

    Train& train = trains[index];
    int month = StringUtil::parseInt(date.substr(0, 2));
    int day = StringUtil::parseInt(date.substr(3, 2));

    StationTime currentTime(month, day, train.startTimeHour, train.startTimeMin);

    for (int i = 0; i < stationIndex; i++) {
        currentTime.min += train.travelTimes[i];
        if (currentTime.min >= 60) {
            currentTime.hour += currentTime.min / 60;
            currentTime.min %= 60;
            if (currentTime.hour >= 24) {
                currentTime.day += currentTime.hour / 24;
                currentTime.hour %= 24;
            }
        }
    }

    return currentTime;
}
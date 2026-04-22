#ifndef TRAIN_H
#define TRAIN_H

#include <string>

struct Train {
    std::string trainID;
    int stationNum;
    std::string* stations;  // Dynamic allocation
    int seatNum;
    int* prices;            // Dynamic allocation
    int startTimeHour;
    int startTimeMin;
    int* travelTimes;       // Dynamic allocation
    int* stopoverTimes;     // Dynamic allocation
    std::string saleDateStart;
    std::string saleDateEnd;
    char type;
    bool isReleased;

    Train() : stationNum(0), seatNum(0), startTimeHour(0), startTimeMin(0),
              type('G'), isReleased(false), stations(nullptr), prices(nullptr),
              travelTimes(nullptr), stopoverTimes(nullptr) {}
    ~Train() {
        delete[] stations;
        delete[] prices;
        delete[] travelTimes;
        delete[] stopoverTimes;
    }
};

struct StationTime {
    int month;
    int day;
    int hour;
    int min;

    StationTime() : month(0), day(0), hour(0), min(0) {}
    StationTime(int m, int d, int h, int mi) : month(m), day(d), hour(h), min(mi) {}

    std::string toString() const;
};

class TrainManager {
private:
    static const int MAX_TRAINS = 5000;
    static const int HASH_SIZE = 10007;

    Train* trains;
    int trainCount;
    int* trainHash;
    int* trainNext;

    int hash(const std::string& trainID);
    int findTrain(const std::string& trainID);

public:
    TrainManager();
    ~TrainManager();

    int addTrain(const std::string& trainID, int stationNum, int seatNum,
                 const std::string* stations, const int* prices,
                 int startTimeHour, int startTimeMin,
                 const int* travelTimes, const int* stopoverTimes,
                 const std::string& saleDateStart, const std::string& saleDateEnd,
                 char type);

    int releaseTrain(const std::string& trainID);
    std::string queryTrain(const std::string& trainID, const std::string& date);
    int deleteTrain(const std::string& trainID);

    bool trainExists(const std::string& trainID);
    bool isTrainReleased(const std::string& trainID);

    int getStationIndex(const std::string& trainID, const std::string& station);
    int getSeatNum(const std::string& trainID);
    int getPrice(const std::string& trainID, int fromIndex, int toIndex);
    StationTime getDepartureTime(const std::string& trainID, int stationIndex, const std::string& date);
    StationTime getArrivalTime(const std::string& trainID, int stationIndex, const std::string& date);
};

#endif
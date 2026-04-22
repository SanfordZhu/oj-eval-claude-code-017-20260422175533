#ifndef TICKET_H
#define TICKET_H

#include <string>
#include "train.h"
#include "user.h"

struct Order {
    std::string username;
    std::string trainID;
    std::string date;
    std::string fromStation;
    std::string toStation;
    int numTickets;
    int totalPrice;
    std::string status;
    long long timestamp;

    Order() : numTickets(0), totalPrice(0), timestamp(0) {}
};

struct TicketQuery {
    std::string trainID;
    std::string fromStation;
    std::string toStation;
    std::string departureTime;
    std::string arrivalTime;
    int price;
    int availableSeats;
    int totalTime;
};

class TicketManager {
private:
    static const int MAX_ORDERS = 100000;
    static const int MAX_WAITLIST = 10000;

    Order* orders;
    int orderCount;

    Order* waitlist;
    int waitlistCount;

    int*** seatAvailability;

    long long currentTimestamp;

public:
    TicketManager();
    ~TicketManager();

    int queryTicket(const std::string& fromStation, const std::string& toStation,
                   const std::string& date, const std::string& sortBy,
                   TicketQuery* results, int maxResults);

    int queryTransfer(const std::string& fromStation, const std::string& toStation,
                     const std::string& date, const std::string& sortBy,
                     TicketQuery& result1, TicketQuery& result2);

    std::string buyTicket(const std::string& username, const std::string& trainID,
                         const std::string& date, int numTickets,
                         const std::string& fromStation, const std::string& toStation,
                         bool queueIfUnavailable);

    int queryOrder(const std::string& username, Order* results, int maxResults);
    int refundTicket(const std::string& username, int orderIndex);

    void updateWaitlist();
    void updateSeatAvailability(const std::string& trainID, int fromIndex, int toIndex,
                               const std::string& date, int numTickets, bool isRefund);

private:
    int getAvailableSeats(const std::string& trainID, int fromIndex, int toIndex, const std::string& date);
    bool canPurchaseTickets(const std::string& trainID, int fromIndex, int toIndex,
                           const std::string& date, int numTickets);
    int calculatePrice(const std::string& trainID, int fromIndex, int toIndex);
    int calculateTime(const std::string& trainID, int fromIndex, int toIndex);
    int dateToDay(const std::string& date);
};

#endif
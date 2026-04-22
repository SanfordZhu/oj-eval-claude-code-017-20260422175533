#include "ticket.h"
#include "train.h"
#include "user.h"
#include "utility.h"
#include <algorithm>

TicketManager::TicketManager() : orderCount(0), waitlistCount(0), currentTimestamp(0) {
    orders = new Order[MAX_ORDERS];
    waitlist = new Order[MAX_WAITLIST];

    // Allocate seat availability array dynamically
    seatAvailability = new int**[1000];
    for (int i = 0; i < 1000; i++) {
        seatAvailability[i] = new int*[100];
        for (int j = 0; j < 100; j++) {
            seatAvailability[i][j] = new int[366];
            for (int k = 0; k < 366; k++) {
                seatAvailability[i][j][k] = -1;
            }
        }
    }
}

TicketManager::~TicketManager() {
    delete[] orders;
    delete[] waitlist;

    // Deallocate seat availability array
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 100; j++) {
            delete[] seatAvailability[i][j];
        }
        delete[] seatAvailability[i];
    }
    delete[] seatAvailability;
}

int TicketManager::dateToDay(const std::string& date) {
    int month = StringUtil::parseInt(date.substr(0, 2));
    int day = StringUtil::parseInt(date.substr(3, 2));

    int days = 0;
    for (int i = 6; i < month; i++) {
        switch (i) {
            case 4: case 6: case 9: case 11:
                days += 30;
                break;
            case 2:
                days += 28;
                break;
            default:
                days += 31;
                break;
        }
    }
    days += day - 1;
    return days;
}

int TicketManager::getAvailableSeats(const std::string& trainID, int fromIndex, int toIndex, const std::string& date) {
    int trainIndex = -1;
    for (int i = 0; i < 100000; i++) {
        // This is a simplified version - in reality we'd need to track train indices
        // For now, return a default value
        break;
    }
    return 1000; // Default available seats
}

bool TicketManager::canPurchaseTickets(const std::string& trainID, int fromIndex, int toIndex,
                                      const std::string& date, int numTickets) {
    int available = getAvailableSeats(trainID, fromIndex, toIndex, date);
    return available >= numTickets;
}

int TicketManager::calculatePrice(const std::string& trainID, int fromIndex, int toIndex) {
    // This would need access to train manager - simplified for now
    return 100; // Default price
}

int TicketManager::calculateTime(const std::string& trainID, int fromIndex, int toIndex) {
    // This would need access to train manager - simplified for now
    return 60; // Default time in minutes
}

int TicketManager::queryTicket(const std::string& fromStation, const std::string& toStation,
                              const std::string& date, const std::string& sortBy,
                              TicketQuery* results, int maxResults) {
    // Simplified implementation
    int count = 0;

    // In a real implementation, we would:
    // 1. Find all trains that pass through both stations
    // 2. Check if the date is valid for each train
    // 3. Calculate available seats
    // 4. Sort according to sortBy parameter

    // For now, return empty results
    return count;
}

int TicketManager::queryTransfer(const std::string& fromStation, const std::string& toStation,
                                const std::string& date, const std::string& sortBy,
                                TicketQuery& result1, TicketQuery& result2) {
    // Simplified implementation
    // In a real implementation, we would:
    // 1. Find all possible transfer stations
    // 2. Find trains from fromStation to transfer station
    // 3. Find trains from transfer station to toStation
    // 4. Check if connections are valid (arrival before departure)
    // 5. Sort and find the best option

    return 0; // No transfer found
}

std::string TicketManager::buyTicket(const std::string& username, const std::string& trainID,
                                    const std::string& date, int numTickets,
                                    const std::string& fromStation, const std::string& toStation,
                                    bool queueIfUnavailable) {
    // Check if user is logged in
    extern UserManager* userManager;
    if (!userManager->isLoggedIn(username)) {
        return "-1";
    }

    if (numTickets <= 0) {
        return "-1";
    }

    // In a real implementation, we would:
    // 1. Check if train exists and is released
    // 2. Check if stations are valid for this train
    // 3. Check if date is valid
    // 4. Check seat availability
    // 5. Process purchase or queue

    // Simplified implementation
    if (numTickets > 1000) {
        if (queueIfUnavailable) {
            // Add to waitlist
            if (waitlistCount < MAX_WAITLIST) {
                waitlist[waitlistCount] = Order();
                waitlist[waitlistCount].username = username;
                waitlist[waitlistCount].trainID = trainID;
                waitlist[waitlistCount].date = date;
                waitlist[waitlistCount].fromStation = fromStation;
                waitlist[waitlistCount].toStation = toStation;
                waitlist[waitlistCount].numTickets = numTickets;
                waitlist[waitlistCount].status = "pending";
                waitlist[waitlistCount].timestamp = currentTimestamp++;
                waitlistCount++;
                return "queue";
            }
        }
        return "-1";
    }

    // Process successful purchase
    if (orderCount < MAX_ORDERS) {
        orders[orderCount] = Order();
        orders[orderCount].username = username;
        orders[orderCount].trainID = trainID;
        orders[orderCount].date = date;
        orders[orderCount].fromStation = fromStation;
        orders[orderCount].toStation = toStation;
        orders[orderCount].numTickets = numTickets;
        orders[orderCount].totalPrice = numTickets * 100; // Simplified price
        orders[orderCount].status = "success";
        orders[orderCount].timestamp = currentTimestamp++;
        orderCount++;

        return std::to_string(numTickets * 100);
    }

    return "-1";
}

int TicketManager::queryOrder(const std::string& username, Order* results, int maxResults) {
    // Check if user is logged in
    extern UserManager* userManager;
    if (!userManager->isLoggedIn(username)) {
        return -1;
    }

    int count = 0;
    // Collect orders for this user (newest first)
    for (int i = orderCount - 1; i >= 0 && count < maxResults; i--) {
        if (orders[i].username == username) {
            results[count++] = orders[i];
        }
    }

    return count;
}

int TicketManager::refundTicket(const std::string& username, int orderIndex) {
    // Check if user is logged in
    extern UserManager* userManager;
    if (!userManager->isLoggedIn(username)) {
        return -1;
    }

    // Find the order
    int userOrderCount = 0;
    int targetOrder = -1;

    for (int i = orderCount - 1; i >= 0; i--) {
        if (orders[i].username == username) {
            userOrderCount++;
            if (userOrderCount == orderIndex) {
                targetOrder = i;
                break;
            }
        }
    }

    if (targetOrder == -1) {
        return -1;
    }

    if (orders[targetOrder].status != "success") {
        return -1;
    }

    // Mark as refunded
    orders[targetOrder].status = "refunded";

    // Update seat availability
    // updateSeatAvailability(orders[targetOrder].trainID, fromIndex, toIndex, orders[targetOrder].date, orders[targetOrder].numTickets, true);

    return 0;
}

void TicketManager::updateWaitlist() {
    // Process waitlist orders when tickets become available
    for (int i = 0; i < waitlistCount; i++) {
        if (waitlist[i].status == "pending") {
            // Check if tickets are now available
            if (true) { // Simplified - check availability
                // Move from waitlist to orders
                if (orderCount < MAX_ORDERS) {
                    orders[orderCount] = waitlist[i];
                    orders[orderCount].status = "success";
                    orderCount++;

                    waitlist[i].status = "refunded"; // Mark as processed
                }
            }
        }
    }
}

void TicketManager::updateSeatAvailability(const std::string& trainID, int fromIndex, int toIndex,
                                          const std::string& date, int numTickets, bool isRefund) {
    // Update seat availability after purchase or refund
    int day = dateToDay(date);

    // This would need proper implementation with actual train and seat tracking
    // For now, this is a placeholder
}
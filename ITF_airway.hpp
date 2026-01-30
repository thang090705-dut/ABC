#ifndef ITF_AIRWAY_HPP
#define ITF_AIRWAY_HPP

#include <iostream>
#include <string>

#define MAX 100
#define MAXSEAT 10

class Login {
private:
    std::string Username;
    std::string Password;
public:
    void input();
    std::string getUsername() const;
    std::string getPassword() const;
    bool check();
    bool performLogin();
};

class Flight {
private:
    std::string Flightcode;
    int MaxSeats = MAXSEAT;
public:
    void inputFlight();
    void displayFlightInfo() const;
    int getMaxSeats() const;
    std::string getFlightCode() const;
};

class SeatManager {
private:
    int seatNo[MAX];
    int seatStatus[MAX];
    char seatCodes[MAX][10];
    int totalSeats;
public:
    void settotalSeats(int total);
    void setSeats(int index, int no, int status);
    void setSeatCode(int index, const char* code);
    const char* getSeatCode(int index) const;
    int gettotalSeats() const;
    int getseatNo(int index) const;
    int getseatStatus(int index) const;
    void displaySeats() const;
    int countEmptySeats();
    void findEmptySeats(int result[], int &count);
    bool assignSeats(int seat);
    void setTicketCodeForSeat(int seat, const char* code);
};

class TicketManager {
private:
    char ticketCode[MAX][10];
    int seatAssigned[MAX];
    int bookingCount;
public:
    void reset();
    bool isTicketexist(const char* newTicketCode);
    void addTicket(const char *newTicketCode, int seat);
};

class FileManager {
public:
    bool loadSeatMap(const char* ghe_ngoi, SeatManager &sm, TicketManager &tm);
    bool saveSeatMap(const char* ghe_ngoi, const SeatManager &sm);
    void printFileContent(const char* ghe_ngoi);    
};

class SystemManager {
private:
    Flight flight;
    SeatManager seatManager;
    TicketManager ticketManager;
    FileManager fileManager;
public:
    void openFlight();
    void bookingProcess();
    void closeFlight();
};

#endif
#include "ITF_airway.hpp"
#include <iostream>
#include <limits>
#include <fstream>
#include <cstring>
#include <iomanip>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// --- Login Implementation ---
void Login::input(){
    cout << "Tên đăng nhập: " << endl;
    getline(cin, Username);
    cout << "Mật khẩu: " << endl;
    getline(cin, Password);
}
string Login::getUsername() const{ return Username; }
string Login::getPassword() const{ return Password; }
bool Login::check(){
    if(getUsername() == "admin" && getPassword() == "1") return true;
    else return false;
}
bool Login::performLogin() {
    input();
    int i = 0;
    while (!check()) {
        ++i;
        if (i == 3) {
            cout << "Khóa đăng nhập" << endl;
            return false;
        }
        cout << "Đăng nhập thất bại! Vui lòng nhập lại" << endl;
        cout << "Còn " << 3 - i << " lần đăng nhập" << endl;
        input();
    }
    cout << "Đăng nhập thành công" << endl;
    return true;
}

// --- Flight Implementation ---
void Flight::inputFlight(){
    cout << "Nhập mã chuyến bay: " << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, Flightcode);
}
void Flight::displayFlightInfo() const{
    cout << "Chuyến bay mang số hiệu là: " << Flightcode << endl;
}
int Flight::getMaxSeats() const{ return MaxSeats; }
string Flight::getFlightCode() const { return Flightcode; }

// --- SeatManager Implementation ---
void SeatManager::settotalSeats(int total){ totalSeats = total; }
void SeatManager::setSeats(int index, int no, int status){
    seatNo[index] = no;
    seatStatus[index] = status;
}
void SeatManager::setSeatCode(int index, const char* code) {
    strcpy(seatCodes[index], code);
}
const char* SeatManager::getSeatCode(int index) const { return seatCodes[index]; }
int SeatManager::gettotalSeats() const{ return totalSeats; }
int SeatManager::getseatNo(int index) const{ return seatNo[index]; }
int SeatManager::getseatStatus(int index) const{ return seatStatus[index]; }

void SeatManager::displaySeats() const{
    cout << endl << "Danh sách ghế ngồi của chuyến bay: " << endl;
    cout << "Tổng số ghế ngồi: " << totalSeats << endl;
    for(int i = 0; i < totalSeats; i++){
        if (seatStatus[i] == 1)
            cout << "Ghế " << seatNo[i] << ":" << "Đã đặt" << endl;
        else
            cout << "Ghế " << seatNo[i] << ":" << "Trống" << endl;
    }
}
int SeatManager::countEmptySeats(){
    int count_empty = 0;
    for(int i = 0; i < totalSeats; i++) {
        if (seatStatus[i] == 0) ++count_empty;
    }
    return count_empty;
}
void SeatManager::findEmptySeats(int result[], int &count){
    count = 0;
    cout << "Danh sách các ghế còn trống: " << endl;
    for(int i = 0; i < totalSeats; i++){
        if(seatStatus[i] == 0){
            result[count] = seatNo[i];
            count++;
        }
    }
}
bool SeatManager::assignSeats(int seat){
    for(int i = 0; i < totalSeats; i++){
        if(seatNo[i] == seat){
            if (seatStatus[i] == 0) {
                seatStatus[i] = 1;
                seatCodes[i][0] = '\0';
                return true;
            }
            else return false;
        }
    }
    return false;
}
void SeatManager::setTicketCodeForSeat(int seat, const char* code){
    for(int i = 0; i < totalSeats; i++){
        if(seatNo[i] == seat){
            strcpy(seatCodes[i], code);
            return;
        }
    }
}

// --- TicketManager Implementation ---
void TicketManager::reset() { bookingCount = 0; }
bool TicketManager::isTicketexist(const char* newTicketCode){
    for(int i = 0; i < bookingCount; i++ ){
        if (strcmp(ticketCode[i],newTicketCode) == 0) return true;
    }
    return false;
}
void TicketManager::addTicket(const char *newTicketCode, int seat){
    strcpy(ticketCode[bookingCount], newTicketCode);
    seatAssigned[bookingCount] = seat;
    bookingCount++;
}


// --- FileManager Implementation ---
bool FileManager::loadSeatMap(const char* ghe_ngoi, SeatManager &sm, TicketManager &tm){
    ifstream fin(ghe_ngoi);
    if (!fin.is_open()) return false;
    tm.reset();
    int total;
    fin >> total;
    sm.settotalSeats(total);
    for (int i = 0; i < total; i++){
        int no, status;
        fin >> no;
        fin >> status;
        if (status == 1) {
            char code[10];
            fin >> code;
            sm.setSeatCode(i, code);
            tm.addTicket(code, no);
        }
        sm.setSeats(i, no, status);
    }
    fin.close();
    return true;
}
bool FileManager::saveSeatMap(const char* ghe_ngoi, const SeatManager &sm){
    ofstream fout(ghe_ngoi);
    if(!fout.is_open()) return false;
    int total = sm.gettotalSeats();
    fout << total << endl;
    for(int i = 0; i < total; i++){
        fout << sm.getseatNo(i) << " " << sm.getseatStatus(i);
        if (sm.getseatStatus(i) == 1) {
            fout << " " << sm.getSeatCode(i);
        }
        fout << endl;
    }
    fout.close();
    return true;
}
void FileManager::printFileContent(const char* ghe_ngoi){
    ifstream fin(ghe_ngoi);
    if (!fin.is_open()) return;
    cout << endl << "--- Nội dung file ---" << endl;
    char ch;
    while(fin.get(ch)){
        cout << ch;
    }
    cout << "--------------------------------------------------" << endl;
    fin.close();
}

// --- SystemManager Implementation ---
void SystemManager::openFlight(){
    flight.inputFlight();
    if (!fileManager.loadSeatMap("D:\\OOP_VScode\\PBL_ITFAW\\ghe_ngoi.txt", seatManager, ticketManager)) {
        cout << "Khong the tai du lieu ghe tu file!" << endl;
    }
    flight.displayFlightInfo();
    cout << "------------------" <<endl;
    seatManager.displaySeats();
    cout << "------------------" <<endl;
    cout << "Tổng số ghế còn trống: " << seatManager.countEmptySeats() << endl;
    cout << "------------------" << endl;
    int result[MAX];
    int count;
    seatManager.findEmptySeats(result, count);
    for (int i = 0; i < count; i++) {
        cout << result[i] << " ";
    }
    cout << endl << "------------------" << endl;
}

void SystemManager::bookingProcess(){
    int count_empty = seatManager.countEmptySeats();
    if (count_empty > 0){
        cout << "Nhập số lượng ghế muốn đặt: " << endl;
        int numSeats;
        int MaxSeats = flight.getMaxSeats();
        while(!(cin >> numSeats) || numSeats <= 0 || numSeats > MaxSeats || count_empty < numSeats){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Số lượng không hợp lệ. Nhập lại: ";     
        }
        for (int i = 0; i < numSeats; ++i) {
            int seatNum;
            char TicketCode[10];
            cout << "--- Đặt vé thứ " << i + 1 << " ---" << endl;
            cout << "Nhập số ghế: ";
            cin >> seatNum;

            if (seatManager.assignSeats(seatNum)) {
                while(true) {
                    cout << "Nhập mã vé (tối đa 9 ký tự): ";
                    cin >> TicketCode;
                    if (ticketManager.isTicketexist(TicketCode)) {
                        cout << "Mã vé đã tồn tại! Vui lòng nhập mã khác." << endl;
                    } else {
                        break;
                    }
                }
                ticketManager.addTicket(TicketCode, seatNum);
                seatManager.setTicketCodeForSeat(seatNum, TicketCode);
                cout << "--- Đặt thành công ghế : " << seatNum << endl;
            } else {
                cout << "--- Ghế : " << seatNum << " không tồn tại hoặc đã có người đặt. Vui lòng chọn lại." << endl;
                i--;
            }
        }
    }
    else{
        cout << "CHUYẾN BAY ĐÃ HẾT GHẾ TRỐNG!" << endl;
        return;
    }
}

void SystemManager::closeFlight(){
    if (fileManager.saveSeatMap("D:\\OOP_VScode\\PBL_ITFAW\\ghe_ngoi.txt", seatManager)) {
        cout << "Đã lưu trạng thái ghế và mã vé vào file ghe_ngoi.txt" << endl;            
        
        cout << "Bạn có muốn xem nội dung file ánh xạ không? (y/n): ";
        char c;
        cin >> c;
        if (c == 'y' || c == 'Y')
            fileManager.printFileContent("D:\\OOP_VScode\\PBL_ITFAW\\ghe_ngoi.txt");
    }
    else {
        cout << "Lỗi lưu file" << endl;
    }
}
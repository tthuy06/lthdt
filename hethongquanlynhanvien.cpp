#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

class Employee {
protected:
    int id;
    string name;
    int birthYear;
    bool isValid;
public:
    Employee() : id(0), name(""), birthYear(0), isValid(false) {}
    Employee(int id, string name, int birthYear) : id(id), name(name), birthYear(birthYear), isValid(true) {}

    int getID() const { return id; }
    void setID(int id) { this->id = id; }

    string getName() const { return name; }
    void setName(string name) { this->name = name; }

    int getBirthYear() const { return birthYear; }
    void setBirthYear(int birthYear) { this->birthYear = birthYear; }

    bool isDataValid() const { return isValid; }

    virtual long getSalary() const = 0;
    virtual void input();
    bool isAgeValid() const;
    virtual void display() const;
    virtual ~Employee() {}
};

class EmployeePT : public Employee {
protected:
    int workHour;
    long salaryPerHour;
public:
    EmployeePT() : workHour(0), salaryPerHour(0) {}
    EmployeePT(int id, string name, int birthYear, int workHour, long salaryPerHour)
        : Employee(id, name, birthYear), workHour(workHour), salaryPerHour(salaryPerHour) {}

    void input();
    void display() const;
    long getSalary() const;
    bool isSalaryValid() const;

    friend istream& operator>>(istream& in, EmployeePT& e);
    friend ostream& operator<<(ostream& out, const EmployeePT& e);
};

class EmployeeFT : public Employee {
protected:
    int yearOfEntry;
    long baseSalary;
public:
    EmployeeFT() : yearOfEntry(0), baseSalary(0) {}
    EmployeeFT(int id, string name, int birthYear, int yearOfEntry, long baseSalary)
        : Employee(id, name, birthYear), yearOfEntry(yearOfEntry), baseSalary(baseSalary) {}

    void input();
    bool checkYearOfEntryValid() const;
    void display() const;
    long getSalary() const;

    friend istream& operator>>(istream& in, EmployeeFT& e);
    friend ostream& operator<<(ostream& out, const EmployeeFT& e);
};

long roundSalary(long salary) {
    return ((salary + 999) / 1000) * 1000;
}

void standardizeName(string &t) {
    string res, token;
    stringstream ss(t);
    while (ss >> token) {
        res += toupper(token[0]);
        for (size_t i = 1; i < token.length(); i++)
            res += tolower(token[i]);
        res += " ";
    }
    if (!res.empty()) res.pop_back();
    t = res;
}

int getCurrentYear() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    return 1900 + ltm->tm_year;
}

void Employee::input() {
    cout << CYAN << "Nhap ID: " << RESET;
    if (!(cin >> id) || id <= 0) {
        cout << RED << "ID khong hop le!" << RESET << endl;
        isValid = false; cin.clear(); 
        cin.ignore(1000, '\n');
        return;
    }

    cout << CYAN << "Nhap ho va ten: " << RESET;
    cin.ignore(1000, '\n');
    if (!getline(cin, name) || name.empty()) { 
       isValid = false;
       return; 
    }

    cout << CYAN << "Nhap nam sinh: " << RESET;
    if (!(cin >> birthYear)) {
        isValid = false; cin.clear(); 
        cin.ignore(1000, '\n'); 
        return;
    }

    int currentYear = getCurrentYear();
    if (birthYear < 1900 || birthYear > currentYear) { isValid = false; return; }

    standardizeName(name);
    isValid = true;
}

bool Employee::isAgeValid() const {
    if (!isValid) return false;
    int age = getCurrentYear() - birthYear;
    return age >= 18;
}

void Employee::display() const {
    if (!isValid) {
        cout << RED << "Danh sach khong hop le" << RESET << endl;
    } else {
        cout << YELLOW << "ID: " << RESET << id << endl;
        cout << YELLOW << "Ho va ten: " << RESET << name << endl;
        cout << YELLOW << "Nam sinh: " << RESET << birthYear << endl;
    }
}

void EmployeePT::input() {
    Employee::input(); 
    if (!isValid || !isAgeValid())
    return;

    string workHourStr;
    bool validInput = false;
    do {
        cout << CYAN << "Nhap so gio lam (vd: 40h): " << RESET;
        cin >> workHourStr;

        if (!workHourStr.empty() && (workHourStr.back() == 'h' || workHourStr.back() == 'H')) {
            workHourStr.pop_back();         }

        try {
            workHour = stoi(workHourStr);
            if (workHour <= 0) {
                cout << RED << "So gio lam phai > 0. Vui long nhap lai.\n" << RESET;
            } else {
                validInput = true;
            }
        } catch (...) {
            cout << RED << "Nhap sai dinh dang so gio lam. Vui long nhap so nguyen hoac so co ky tu 'h'.\n" << RESET;
        }
    } while (!validInput);

    validInput = false;
    do {
        cout << CYAN << "Nhap luong theo gio (>= 21000): " << RESET;
        if (!(cin >> salaryPerHour)) {
            cout << RED << "Luong theo gio phai la so. Vui long nhap lai.\n" << RESET;
            cin.clear();
            cin.ignore(1000, '\n');
        } else if (!isSalaryValid()) {
            cout << RED << "Luong theo gio phai >= 21000. Vui long nhap lai.\n" << RESET;
        } else {
            validInput = true;
        }
    } while (!validInput);

    isValid = true;
}

bool EmployeePT::isSalaryValid() const {
    return salaryPerHour >= 21000;
}

void EmployeeFT::input() {
    Employee::input();
    if (!isValid || !isAgeValid()) return;

    int currentYear = getCurrentYear();
    bool validInput = false;

    do {
        cout << CYAN << "Nhap nam vao lam: " << RESET;
        if (!(cin >> yearOfEntry)) {
            cout << RED << "Nam vao lam phai la so. Vui long nhap lai.\n" << RESET;
            cin.clear(); cin.ignore(1000, '\n');
        } else if (yearOfEntry < birthYear + 18 || yearOfEntry > currentYear) {
            cout << RED << "Nam vao lam khong hop le! Nhan vien phai it nhat 18 tuoi khi vao lam.\n" << RESET;
        } else {
            validInput = true;
        }
    } while (!validInput);

    validInput = false;
    do {
        cout << CYAN << "Nhap luong co ban (>= 4200000): " << RESET;
        if (!(cin >> baseSalary)) {
            cout << RED << "Luong co ban phai la so. Vui long nhap lai.\n" << RESET;
            cin.clear(); cin.ignore(1000, '\n');
        } else if (baseSalary < 4200000) {
            cout << RED << "Luong co ban khong duoc nho hon 4.200.000. Vui long nhap lai.\n" << RESET;
        } else {
            validInput = true;
        }
    } while (!validInput);

    isValid = true;
}

bool EmployeeFT::checkYearOfEntryValid() const {
    if (!isAgeValid()) return true;
    int seniority = getCurrentYear() - yearOfEntry;
    return (seniority < 0 || seniority > 50);
}

void EmployeePT::display() const {
    if (!isValid) {
        cout << RED << "Danh sach khong hop le" << RESET << endl;
    } else if (!isAgeValid()) {
        cout << RED << getName() << ": Tuoi khong hop le" << RESET << endl;
    } else {
        Employee::display();
        cout << YELLOW << "So gio da lam: " << RESET << workHour << "h" << endl;
        cout << YELLOW << "Luong thang nay: " << RESET << getSalary() << " VND" << endl;
    }
}

long EmployeePT::getSalary() const {
    return (isValid ? roundSalary(workHour * salaryPerHour) : 0);
}

void EmployeeFT::display() const {
    if (!isValid) {
        cout << RED << "Danh sach khong hop le" << RESET << endl;
    } else if (!isAgeValid() || checkYearOfEntryValid()) {
        cout << RED << getName() << ": Tuoi hoac nam vao lam khong hop le" << RESET << endl;
    } else {
        Employee::display();
        int seniority = getCurrentYear() - yearOfEntry;
        long salaryIncrease = (seniority / 3) * (baseSalary * 0.2);
        cout << YELLOW << "Tham nien: " << RESET << seniority << endl;
        cout << YELLOW << "Luong duoc tang: " << RESET << salaryIncrease << " VND" << endl;
        cout << YELLOW << "Luong hien tai: " << RESET << getSalary() << " VND" << endl;
    }
}

long EmployeeFT::getSalary() const {
    if (!isValid) return 0;
    int seniority = getCurrentYear() - yearOfEntry;
    long salaryIncrease = (seniority / 3) * (baseSalary * 0.2);
    return roundSalary(baseSalary + salaryIncrease);
}

istream& operator >> (istream& in, EmployeePT& e) {
    e.input(); return in;
}

ostream& operator << (ostream& out, const EmployeePT& e) {
    e.display(); return out;
}

istream& operator >> (istream& in, EmployeeFT& e) {
    e.input(); return in;
}

ostream& operator << (ostream& out, const EmployeeFT& e) {
    e.display(); return out;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    vector<EmployeePT> partTimers;
    vector<EmployeeFT> fullTimers;

    char choice;
    cout << GREEN << "     === QUAN LY NHAN VIEN N8 ===\n";
    cout << GREEN << "Nhap P (p) de them nhan vien part-time.\n";
    cout << GREEN << "Nhap F (f) de them nhan vien full-time.\n";
    cout << GREEN << "Nhap X (x) de thoat chuong trinh.\n";
    cout << GREEN << "Xin moi ban lua chon: "<< RESET;
    cin >> choice;

    while (toupper(choice) != 'X') {
        if (toupper(choice) == 'P') {
            EmployeePT emp;
            cin >> emp;
            if (emp.isDataValid()) partTimers.push_back(emp);
            else cout << RED << "Du lieu khong hop le." << RESET << endl;
        } else if (toupper(choice) == 'F') {
            EmployeeFT emp;
            cin >> emp;
            if (emp.isDataValid()) fullTimers.push_back(emp);
            else cout << RED << "Du lieu khong hop le." << RESET << endl;
        } else {
            cout << YELLOW << "Chi nhap P hoac F hoac X." << RESET << endl;
        }
        cout << GREEN << "Nhap P/F hoac X: " << RESET;
        cin >> choice;
    }
    cout << CYAN << "\n=== DANH SACH NHAN VIEN PART-TIME ===\n" << RESET;
    for (const auto& e : partTimers) cout << e << endl;
    cout << CYAN << "\n=== DANH SACH NHAN VIEN FULL-TIME ===\n" << RESET;
    for (const auto& e : fullTimers) cout << e << endl;

    return 0;
}





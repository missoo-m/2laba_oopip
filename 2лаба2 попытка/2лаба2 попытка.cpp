#include <iostream>
#include <fstream>
#include <string>
#include <locale.h> 
#include <windows.h>


using namespace std;

class Employee
{
public:
    string fio;
    string post; // должность

    Employee() {}
    Employee(const string& fio, const string& post) : fio(fio), post(post) {}

    virtual void printInfo() const
    {
        cout << "Имя: " << fio << ", Должность: " << post << endl;
    }

    friend ostream& operator<<(ostream& out, const Employee& emp)
    {
        out << emp.fio << endl << emp.post << endl;
        return out;
    }

    friend istream& operator>>(istream& in, Employee& emp)
    {
        getline(in, emp.fio);
        getline(in, emp.post);
        return in;
    }

    virtual ~Employee() {}
};

class Pilot : public Employee
{
public:
    int license_number;

    Pilot() : license_number(0) {}
    Pilot(const string& fio, int license_number) : Employee(fio, "Пилот"), license_number(license_number) {}

    void printInfo() const override
    {
        Employee::printInfo();
        cout << "Номер лицензии: " << license_number << endl;
    }

    friend ostream& operator<<(ostream& out, const Pilot& pilot)
    {
        out << static_cast<const Employee&>(pilot);
        out << pilot.license_number << endl;
        return out;
    }

    friend istream& operator>>(istream& in, Pilot& pilot)
    {
        in >> static_cast<Employee&>(pilot);
        in >> pilot.license_number;
        in.ignore(); // Игнорируем переход на новую строку после license_number
        return in;
    }
};

class FlightAttendant : public Employee
{
public:
    string certification;

    FlightAttendant() {}
    FlightAttendant(const string& fio, const string& certification) : Employee(fio, "Бортпроводник"), certification(certification) {}

    void printInfo() const override
    {
        Employee::printInfo();
        cout << "Сертификация: " << certification << endl;
    }

    friend ostream& operator<<(ostream& out, const FlightAttendant& attendant)
    {
        out << static_cast<const Employee&>(attendant);
        out << attendant.certification << endl;
        return out;
    }

    friend istream& operator>>(istream& in, FlightAttendant& attendant)
    {
        in >> static_cast<Employee&>(attendant);
        getline(in, attendant.certification);
        return in;
    }
};


void addEmployeeToFile(const Employee& emp, const string& filename)
{
    ofstream file(filename, ios::app); // Открываем файл для добавления(аппенд дописываются в конец ) биты режимов 
    if (file.is_open())
    {
        file << emp;
        file.close();
        cout << "Данные сотрудника успешно добавлены в файл." << endl;
    }
    else
    {
        cout << "Не удалось открыть файл для записи." << endl;
    }
}

void viewAllEmployees(const string& filename)
{
    ifstream file(filename);
    if (file.is_open())
    {
        while (!file.eof())
        {
            Employee emp;
            file >> emp;
            if (file.good()) //если прошло все успешно(стобы пустые строки не выводил)
            {
                emp.printInfo();
            }
        }
        file.close();
    }
    else
    {
        cout << "Не удалось открыть файл для чтения." << endl;
    }
}

void searchEmployeeByName(const string& filename, const string& name)
{
    ifstream file(filename);
    if (file.is_open())
    {
        bool found = false;
        while (!file.eof())
        {
            Employee emp;
            file >> emp;
            if (emp.fio == name)
            {
                cout << "Найден сотрудник:" << endl;
                emp.printInfo();
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "Сотрудник с именем " << name << " не найден." << endl;
        }
        file.close();
    }
    else
    {
        cout << "Не удалось открыть файл для поиска." << endl;
    }
}

void deleteEmployeeFromFile(const string& filename, const string& name)
{
    ifstream file(filename);
    ofstream tempFile("temp.txt");

    if (file.is_open() && tempFile.is_open())
    {
        bool found = false;
        while (!file.eof())
        {
            Employee emp;
            file >> emp;
            if (emp.fio == name)
            {
                cout << "Сотрудник " << name << " удален." << endl;
                found = true;
            }
            else if (!file.eof())
            {
                tempFile << emp; // Записываем только тех, кто не совпадает
            }
        }
        file.close();
        tempFile.close();
        remove(filename.c_str());        // Удаляем старый файл
        rename("temp.txt", filename.c_str()); // Переименовываем временный файл
    }
    else
    {
        cout << "Не удалось открыть файл." << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "Rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Pilot pilot("Семечев А.В.", 23456789);
    FlightAttendant flightAttendant("Яровая В.А.", "29930493994");

    addEmployeeToFile(pilot, "employees.txt");
    addEmployeeToFile(flightAttendant, "employees.txt");

    cout << "\nПросмотр всех сотрудников:" << endl;
    viewAllEmployees("employees.txt");

    cout << "\nПоиск сотрудника по имени 'Семечев А.В.':" << endl;
    searchEmployeeByName("employees.txt", "Семечев А.В.");

    cout << "\nУдаление сотрудника 'Яровая В.А.':" << endl;
    deleteEmployeeFromFile("employees.txt", "Яровая В.А.");

    cout << "\nПросмотр всех сотрудников после удаления:" << endl;
    viewAllEmployees("employees.txt");

    return 0;
}
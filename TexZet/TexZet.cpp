#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <list>
#include <algorithm>
#include <locale>
#include <limits>

struct Person {
    std::wstring name;
    std::wstring last_name;
    std::wstring phone;
};

// Функция для вывода структуры
void print_person(const Person& p) {
    std::wcout << p.last_name << L" " << p.name << L": " << p.phone << std::endl;
}

int main() {
    // Устанавливаем локаль для поддержки Windows-1251
    setlocale(LC_ALL, "Russian_Russia.1251");
    std::wcin.imbue(std::locale("Russian_Russia.1251"));
    std::wcout.imbue(std::locale("Russian_Russia.1251"));

    std::ifstream test_file("C:\\project\\Person.txt");
    if (!test_file) {
        std::wcerr << L"Файл не найден или недоступен." << std::endl;
        return 1;
    }
    test_file.close();

    std::wifstream file("C:\\project\\Person.txt");
    file.imbue(std::locale("en_US.UTF-8")); // Используйте UTF-8, если файл в этой кодировке
    if (!file.is_open()) {
        std::wcerr << L"Ошибка открытия файла" << std::endl;
        return 1;
    }

    // Регулярное выражение для поиска имени, фамилии и телефона (для кириллицы и латиницы)
    std::wregex re(LR"(([А-Яа-яЁёA-Za-z]+)\s+([А-Яа-яЁёA-Za-z]+):\s*(\d+))");
    std::wstring line;
    std::list<Person> persons;

    // Чтение файла построчно
    while (std::getline(file, line)) {
        std::wsmatch match;
        if (std::regex_search(line, match, re) && match.size() == 4) {
            // Заполняем структуру найденными данными
            Person p{ match[2].str(), match[1].str(), match[3].str() };
            persons.push_back(p);
        }
        else {
            std::wcerr << L"Не удалось распознать строку: " << line << std::endl;
        }
    }

    file.close();

    // Вывод количества найденных людей
    std::wcout << L"Найдено " << persons.size() << L" человек(а)." << std::endl;

    while (true) {
        // Ввод пользователя для выбора критерия сортировки
        int choice = 0;
        std::wcout << L"Выберите параметр сортировки:\n";
        std::wcout << L"1 - по фамилии\n";
        std::wcout << L"2 - по имени\n";
        std::wcout << L"3 - по номеру телефона\n";
        std::wcout << L"0 - выход\n"; // Возможность выхода
        std::wcin >> choice;

        if (std::wcin.fail() || choice < 0 || choice > 3) {
            std::wcerr << L"Некорректный выбор. Попробуйте снова." << std::endl;
            std::wcin.clear(); // Сброс состояния потока
            std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n'); // Игнорирование оставшихся символов
            continue; // Возврат к началу цикла
        }

        if (choice == 0) {
            break; // Выход из цикла
        }

        // Сортировка списка на основе выбора пользователя
        switch (choice) {
        case 1:  // Сортировка по фамилии
            persons.sort([](const Person& a, const Person& b) {
                return a.last_name < b.last_name;
                });
            break;
        case 2:  // Сортировка по имени
            persons.sort([](const Person& a, const Person& b) {
                return a.name < b.name;
                });
            break;
        case 3:  // Сортировка по номеру телефона
            persons.sort([](const Person& a, const Person& b) {
                return a.phone < b.phone;
                });
            break;
        default:
            std::wcerr << L"Некорректный выбор. Сортировка по умолчанию (по фамилии)." << std::endl;
            persons.sort([](const Person& a, const Person& b) {
                return a.last_name < b.last_name;
                });
            break;
        }

        // Вывод отсортированных данных
        for (const auto& p : persons) {
            print_person(p);
        }

        std::wcout << L"\nНажмите любую клавишу, чтобы продолжить..." << std::endl;
        std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n'); // Ожидание ввода
        std::wcin.get(); // Ожидание нажатия клавиши
    }

    return 0;
}


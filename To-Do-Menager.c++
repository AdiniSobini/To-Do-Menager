#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

struct Task {
    std::string title;
    std::string category;
    std::string dueDate;
    bool done;
};

const std::string kDataFile = "tasks.txt";

void printMenu() {
    std::cout << "\n=== TO-DO MANAGER ===\n";
    std::cout << "1. Pokaz zadania\n";
    std::cout << "2. Dodaj zadanie\n";
    std::cout << "3. Oznacz jako zrobione\n";
    std::cout << "4. Usun zadanie\n";
    std::cout << "5. Filtruj po kategorii\n";
    std::cout << "6. Zapisz i wyjdz\n";
    std::cout << "Wybor: ";
}

std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        parts.push_back(item);
    }

    return parts;
}

void saveTasks(const std::vector<Task>& tasks) {
    std::ofstream file(kDataFile);
    if (!file) {
        std::cout << "Blad: nie mozna zapisac do pliku.\n";
        return;
    }

    for (const Task& task : tasks) {
        file << task.title << '|'
             << task.category << '|'
             << task.dueDate << '|'
             << (task.done ? "1" : "0") << '\n';
    }
}

std::vector<Task> loadTasks() {
    std::vector<Task> tasks;
    std::ifstream file(kDataFile);
    if (!file) {
        return tasks;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> parts = split(line, '|');
        if (parts.size() != 4) {
            continue;
        }

        Task task;
        task.title = parts[0];
        task.category = parts[1];
        task.dueDate = parts[2];
        task.done = (parts[3] == "1");
        tasks.push_back(task);
    }

    return tasks;
}

void printTasks(const std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << "Brak zadan.\n";
        return;
    }

    std::cout << "\n--- Lista zadan ---\n";
    for (size_t i = 0; i < tasks.size(); ++i) {
        const Task& task = tasks[i];
        std::cout << i + 1 << ". "
                  << "[" << (task.done ? 'X' : ' ') << "] "
                  << task.title
                  << " | Kategoria: " << task.category
                  << " | Termin: " << task.dueDate << '\n';
    }
}

void addTask(std::vector<Task>& tasks) {
    Task task;
    task.done = false;

    std::cout << "Tytul: ";
    std::getline(std::cin, task.title);
    std::cout << "Kategoria: ";
    std::getline(std::cin, task.category);
    std::cout << "Termin (np. 2026-03-01): ";
    std::getline(std::cin, task.dueDate);

    if (task.title.empty()) {
        std::cout << "Blad: tytul nie moze byc pusty.\n";
        return;
    }

    tasks.push_back(task);
    std::cout << "Dodano zadanie.\n";
}

void markDone(std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << "Brak zadan do oznaczenia.\n";
        return;
    }

    printTasks(tasks);
    std::cout << "Podaj numer zadania: ";

    size_t index;
    std::cin >> index;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (index == 0 || index > tasks.size()) {
        std::cout << "Nieprawidlowy numer.\n";
        return;
    }

    tasks[index - 1].done = true;
    std::cout << "Zadanie oznaczone jako zrobione.\n";
}

void deleteTask(std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << "Brak zadan do usuniecia.\n";
        return;
    }

    printTasks(tasks);
    std::cout << "Podaj numer zadania do usuniecia: ";

    size_t index;
    std::cin >> index;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (index == 0 || index > tasks.size()) {
        std::cout << "Nieprawidlowy numer.\n";
        return;
    }

    tasks.erase(tasks.begin() + static_cast<long>(index - 1));
    std::cout << "Usunieto zadanie.\n";
}

void filterByCategory(const std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << "Brak zadan.\n";
        return;
    }

    std::string category;
    std::cout << "Podaj kategorie: ";
    std::getline(std::cin, category);

    bool found = false;
    for (size_t i = 0; i < tasks.size(); ++i) {
        const Task& task = tasks[i];
        if (task.category == category) {
            found = true;
            std::cout << i + 1 << ". "
                      << "[" << (task.done ? 'X' : ' ') << "] "
                      << task.title
                      << " | Termin: " << task.dueDate << '\n';
        }
    }

    if (!found) {
        std::cout << "Brak zadan w tej kategorii.\n";
    }
}

int main() {
    std::vector<Task> tasks = loadTasks();

    while (true) {
        printMenu();

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                printTasks(tasks);
                break;
            case 2:
                addTask(tasks);
                break;
            case 3:
                markDone(tasks);
                break;
            case 4:
                deleteTask(tasks);
                break;
            case 5:
                filterByCategory(tasks);
                break;
            case 6:
                saveTasks(tasks);
                std::cout << "Zapisano. Koniec programu.\n";
                return 0;
            default:
                std::cout << "Nieprawidlowy wybor.\n";
        }
    }
}

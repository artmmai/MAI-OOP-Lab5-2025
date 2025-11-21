#include "include/vector.h"
#include "include/vector_iterator.h"
#include <iostream>
#include <string>
#include <cstddef>

// === ДЕМОНСТРАЦИЯ РАБОТЫ КОНТЕЙНЕРА С ПРОСТЫМИ И СЛОЖНЫМИ ПОЛЯМИ ===

struct Employee {
    std::string name; 
    int id;
    double salary;
    
    Employee(const std::string& n = "", int i = 0, double s = 0.0)
        : name(n), id(i), salary(s) {}
    
    friend std::ostream& operator<<(std::ostream& os, const Employee& emp) {
        os << "Employee{name: '" << emp.name << "', id: " << emp.id 
           << ", salary: " << emp.salary << "}";
        return os;
    }
};

void demonstrate_simple_types() {
    std::cout << "=== ДЕМОНСТРАЦИЯ РАБОТЫ ПРОСТЫХ ТИПОВ (int) ===" << std::endl;
    
    my_vector::PmrVector<int> vec;
    
    std::cout << "Добавление элементов: 10, 11, 12, 13, 14, 15" << std::endl;
    for (int i = 10; i <= 15; ++i) {
        vec.push_back(i);
    }
    
    std::cout << "Содержимое динамического массива: ";
    for (auto it = vec.begin(); it != vec.end(); ++it){
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Меняем элемент с индексом 2 на 555" << std::endl;
    vec[2] = 555;
    
    std::cout << "После изменения: ";
    for (auto it = vec.begin(); it != vec.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
    
    std::cout << "Удаляем все элементы из динамического массива..." << std::endl;
    vec.clear();
    std::cout << "Размер динамического массива после очистки: " << vec.size() << std::endl << std::endl;
}

void demonstrate_complex_types() {
    std::cout << "=== ДЕМОНСТРАЦИЯ РАБОТЫ СЛОЖНЫХ ТИПОВ (Employee) ===" << std::endl;
    
    my_vector::PmrVector<Employee> vec;
    
    vec.push_back(Employee("Valentin Zaitsev", 776, 77600.0));
    vec.push_back(Employee("Vladimir Putin", 777, 77777.0));
    vec.push_back(Employee("Donald Knuth", 778, 88888.0));
    
    std::cout << "Динамический массив сотрудников:" << std::endl;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << "  " << *it << std::endl;
    }
    
    std::cout << "Изменение зарплаты первого сотрудника с помощью итератора" << std::endl;
    auto it = vec.begin();
    it->salary = 85000.0;
    std::cout << "Изменёние зп первого сотрудника: " << vec.front() << std::endl;
    
    std::cout << "Удаление (увольнение) второго сотрудника..." << std::endl;
    vec.erase(1);
    
    std::cout << "После удаления второго сотрудника (erase): ";
    for (auto it2 = vec.begin(); it2 != vec.end(); ++it2)
        std::cout << it2->name << " ";
    std::cout << std::endl << std::endl;
}

void demonstrate_exception_handling() {
    std::cout << "=== ДЕМОНСТРАЦИЯ ОБРАБОТКИ ИСКЛЮЧЕНИЙ ===" << std::endl;
    
    my_vector::PmrVector<int> vec;
    
    try {
        vec.pop_back();
        std::cout << "ОШИБКА: Должно было возникнуть исключение!" << std::endl;
    } catch (const std::runtime_error& runtime_error) {
        std::cout << "Правильно перехваченное исключение: " << runtime_error.what() << std::endl;
    }
    
    try {
        vec.at(0);
        std::cout << "ОШИБКА: Должно было возникнуть исключение!" << std::endl;
    } catch (const std::out_of_range& out_of_range_error) {
        std::cout << "Правильно перехваченное исключение: " << out_of_range_error.what() << std::endl;
    }
    
    std::cout << "Добавление элементов и проверка работы методов (front, back)" << std::endl;
    vec.push_back(42);
    vec.push_back(404);
    std::cout << "Front: " << vec.front() << ", Back: " << vec.back() << std::endl << std::endl;
}

int main() {
        
    try {
        demonstrate_simple_types();
        demonstrate_complex_types();
        demonstrate_exception_handling();
        
        std::cout << "===================================" << std::endl;
        std::cout << "ВСЕ ДЕМОНСТРАЦИИ ЗАВЕРШИЛИСЬ УСПЕШНО!" << std::endl;
        
    } catch (const std::exception& err) {
        std::cerr << "ДЕМОНСТРАЦИЯ НЕ УДАЛАСЬ: " << err.what() << std::endl;
        return 1;
    }
    
    return 0;
}

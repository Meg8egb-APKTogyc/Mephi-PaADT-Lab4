#pragma once
#include "Set.hpp"
#include "Person.hpp"
#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <typeinfo>
#include <type_traits>
#include <memory>

class SetTester {
public:
    static void runTests() {
        while (true) {
            std::cout << "\n=== Set Implementation Tester ===" << std::endl;
            std::cout << "1. Test with integers" << std::endl;
            std::cout << "2. Test with doubles" << std::endl;
            std::cout << "3. Test with strings" << std::endl;
            std::cout << "4. Test with Students (auto)" << std::endl;
            std::cout << "5. Test with Teachers (auto)" << std::endl;
            std::cout << "6. Exit" << std::endl;
            std::cout << "Select option: ";

            int choice;
            std::cin >> choice;

            if (choice == 1) testWithType<int>();
            else if (choice == 2) testWithType<double>();
            else if (choice == 3) testWithType<std::string>();
            else if (choice == 4) runAutoTests<Student>();
            else if (choice == 5) runAutoTests<Teacher>();
            else if (choice == 6) break;
            else std::cout << "Invalid choice!" << std::endl;
        }
    }

private:
    template<typename T>
    static void runAutoTests() {
        std::cout << "\n=== Running " << typeid(T).name() << " Automatic Tests ===" << std::endl;
        
        Set<T> set1, set2;
        
        // Заполняем тестовыми данными
        auto testValues = getTestValues<T>();
        for (const auto& val : testValues.first) set1.insert(val);
        for (const auto& val : testValues.second) set2.insert(val);
        
        // Тест 1: Проверка размера
        if (set1.size() != testValues.first.size()) {
            std::cout << "Test 1 (Insert) FAILED" << std::endl;
            return;
        }
        
        // Тест 2: Удаление
        T toRemove = testValues.first[0];
        set1.remove(toRemove);
        if (set1.contains(toRemove)) {
            std::cout << "Test 2 (Remove) FAILED" << std::endl;
            return;
        }
        
        // Тест 3: Объединение
        auto unionSet = set1.unionWith(&set2);
        if (!unionSet->contains(testValues.second[0])) {
            std::cout << "Test 3 (Union) FAILED" << std::endl;
            return;
        }
        
        // Тест 4: Пересечение
        auto intersection = set1.intersectionWith(&set2);
        if (intersection->size() != expectedIntersectionSize<T>()) {
            std::cout << "Test 4 (Intersection) FAILED" << std::endl;
            return;
        }
        
        std::cout << "All tests PASSED!" << std::endl;
    }

    template<typename T>
    static std::pair<std::vector<T>, std::vector<T>> getTestValues() {
        if constexpr (std::is_same_v<T, Student>) {
            return {
                {
                    Student(PersonID("1234", "567890"), "Ivan", "Ivanovich", "Ivanov", 0, "S12345"),
                    Student(PersonID("2345", "678901"), "Petr", "Petrovich", "Petrov", 0, "S23456")
                },
                {
                    Student(PersonID("3456", "789012"), "Anna", "Andreevna", "Sidorova", 0, "S34567"),
                    Student(PersonID("2345", "678901"), "Petr", "Petrovich", "Petrov", 0, "S23456")
                }
            };
        } else if constexpr (std::is_same_v<T, Teacher>) {
            return {
                {
                    Teacher(PersonID("4567", "890123"), "Sergey", "Sergeevich", "Smirnov", 0, "Mathematics"),
                    Teacher(PersonID("5678", "901234"), "Olga", "Olegovna", "Kuznetsova", 0, "Physics")
                },
                {
                    Teacher(PersonID("6789", "012345"), "Dmitry", "Dmitrievich", "Volkov", 0, "Computer Science"),
                    Teacher(PersonID("5678", "901234"), "Olga", "Olegovna", "Kuznetsova", 0, "Physics")
                }
            };
        }
        return {};
    }

    template<typename T>
    static size_t expectedIntersectionSize() {
        if constexpr (std::is_same_v<T, Student> || std::is_same_v<T, Teacher>) {
            return 1;
        }
        return 0;
    }

    template<typename T>
    static void testWithType() {
        std::unique_ptr<Set<T>> set1(new Set<T>());
        std::unique_ptr<Set<T>> set2(new Set<T>());
        
        while (true) {
            std::cout << "\n=== Testing with type: ===" << std::endl;
            std::cout << "Set 1: "; set1->print();
            std::cout << "Set 2: "; set2->print();
            
            std::cout << "\nOperations:" << std::endl;
            std::cout << "1. Add element to Set 1" << std::endl;
            std::cout << "2. Add element to Set 2" << std::endl;
            std::cout << "3. Remove element from Set 1" << std::endl;
            std::cout << "4. Remove element from Set 2" << std::endl;
            std::cout << "5. Union (Set1 + Set2)" << std::endl;
            std::cout << "6. Intersection (Set1 * Set2)" << std::endl;
            std::cout << "7. Difference (Set1 - Set2)" << std::endl;
            std::cout << "8. Check subset (Set1 <= Set2)" << std::endl;
            std::cout << "9. Check equality (Set1 == Set2)" << std::endl;
            std::cout << "10. Clear Set 1" << std::endl;
            std::cout << "11. Clear Set 2" << std::endl;
            std::cout << "12. Back to main menu" << std::endl;
            std::cout << "Select operation: ";

            int op;
            std::cin >> op;

            if (op == 12) break;

            try {
                handleOperation<T>(op, *set1, *set2);
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
    }

    // Обработка операций
    template<typename T>
    static void handleOperation(int op, Set<T>& set1, Set<T>& set2) {
        T value;
        std::unique_ptr<Set<T>> result;

        switch (op) {
            case 1:
                std::cout << "Enter value to add to Set 1: ";
                std::cin >> value;
                set1.insert(value);
                break;
                
            case 2:
                std::cout << "Enter value to add to Set 2: ";
                std::cin >> value;
                set2.insert(value);
                break;
                
            case 3:
                std::cout << "Enter value to remove from Set 1: ";
                std::cin >> value;
                set1.remove(value);
                break;
                
            case 4:
                std::cout << "Enter value to remove from Set 2: ";
                std::cin >> value;
                set2.remove(value);
                break;
                
            case 5:
                result.reset(set1.unionWith(&set2));
                std::cout << "Union: " << result.get() << std::endl;
                break;
                
            case 6:
                result.reset(set1.intersectionWith(&set2));
                std::cout << "Intersection: " << result.get() << std::endl;
                break;
                
            case 7:
                result.reset(set1.differenceWith(&set2));
                std::cout << "Difference: " << result.get() << std::endl;
                break;
                
            case 8:
                std::cout << "Is Set1 subset of Set2? " 
                          << (set1.isSubsetOf(&set2) ? "Yes" : "No") << std::endl;
                break;
                
            case 9:
                std::cout << "Are sets equal? " 
                          << (set1.equals(&set2) ? "Yes" : "No") << std::endl;
                break;
                
            case 10:
                set1.clear();
                std::cout << "Set 1 cleared" << std::endl;
                break;
                
            case 11:
                set2.clear();
                std::cout << "Set 2 cleared" << std::endl;
                break;
                
            default:
                std::cout << "Invalid operation!" << std::endl;
        }
    }
};


template<>
void SetTester::handleOperation<std::string>(int op, Set<std::string>& set1, Set<std::string>& set2) {
    std::string value;
    std::unique_ptr<Set<std::string>> result;

    switch (op) {
        case 1:
            std::cout << "Enter string to add to Set 1: ";
            std::cin.ignore();
            std::getline(std::cin, value);
            set1.insert(value);
            break;
            
        case 2:
            std::cout << "Enter string to add to Set 2: ";
            std::cin.ignore();
            std::getline(std::cin, value);
            set2.insert(value);
            break;
            
        case 3:
            std::cout << "Enter string to remove from Set 1: ";
            std::cin.ignore();
            std::getline(std::cin, value);
            set1.remove(value);
            break;
            
        case 4:
            std::cout << "Enter string to remove from Set 2: ";
            std::cin.ignore();
            std::getline(std::cin, value);
            set2.remove(value);
            break;
            
        case 5:
            result.reset(set1.unionWith(&set2));
            std::cout << "Union: " << result.get() << std::endl;
            break;
            
        case 6:
            result.reset(set1.intersectionWith(&set2));
            std::cout << "Intersection: " << result.get() << std::endl;
            break;
            
        case 7:
            result.reset(set1.differenceWith(&set2));
            std::cout << "Difference: " << result.get() << std::endl;
            break;
            
        case 8:
            std::cout << "Is Set1 subset of Set2? " 
                      << (set1.isSubsetOf(&set2) ? "Yes" : "No") << std::endl;
            break;
            
        case 9:
            std::cout << "Are sets equal? " 
                      << (set1.equals(&set2) ? "Yes" : "No") << std::endl;
            break;
            
        case 10:
            set1.clear();
            std::cout << "Set 1 cleared" << std::endl;
            break;
            
        case 11:
            set2.clear();
            std::cout << "Set 2 cleared" << std::endl;
            break;
            
        default:
            std::cout << "Invalid operation!" << std::endl;
    }
}
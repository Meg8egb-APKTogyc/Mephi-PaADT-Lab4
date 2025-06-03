#pragma once
#include "Set.hpp"
#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <typeinfo>
#include <type_traits>
#include <memory>

class SetTester {
public:
    // Запуск тестирования
    static void runTests() {
        while (true) {
            std::cout << "\n=== Set Implementation Tester ===" << std::endl;
            std::cout << "1. Test with integers" << std::endl;
            std::cout << "2. Test with doubles" << std::endl;
            std::cout << "3. Test with strings" << std::endl;
            std::cout << "4. Exit" << std::endl;
            std::cout << "Select option: ";

            int choice;
            std::cin >> choice;

            if (choice == 1) testWithType<int>();
            else if (choice == 2) testWithType<double>();
            else if (choice == 3) testWithType<std::string>();
            else if (choice == 4) break;
            else std::cout << "Invalid choice!" << std::endl;
        }
    }

private:
    // Шаблонный метод для тестирования с конкретным типом
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

// Специализация для работы со строками (чтобы cin работал корректно)
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
#pragma once
#include "AVLTree.hpp"
#include "Person.hpp"
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <random>
#include <typeinfo>

class AVLTreeTesterBase {
public:
    virtual ~AVLTreeTesterBase() = default;
    virtual void runInteractiveTest() = 0;
};

template <typename T>
class AVLTreeTester : public AVLTreeTesterBase {
public:
    void runInteractiveTest() {
        if constexpr (std::is_same_v<T, Student> || std::is_same_v<T, Teacher>) {
            runAutoTests();
            return;
        }


        AVLTree<T>* tree = new AVLTree<T>();
        while (true) {
            printMenu();
            int choice;
            std::cin >> choice;
            
            if (choice == 9) break;
            
            switch (choice) {
                case 1: testInsert(tree); break;
                case 2: testRemove(tree); break;
                case 3: testContains(tree); break;
                case 4: printTree(tree); break;
                case 5: testTraversals(tree); break;
                case 6: testSubtreeOperations(tree); break;
                case 7: testMapWhere(); break;
                case 8: runAutoTests(); break;
                default: std::cout << "Invalid choice!\n";
            }
        }
    }

private:
    void printMenu() {
        std::cout << "\n=== AVL Tree Tester ===\n"
                << "1. Insert element\n"
                << "2. Remove element\n"
                << "3. Check contains\n"
                << "4. Print tree\n"
                << "5. Show traversals\n"
                << "6. Subtree operations\n"
                << "7. Test map/where\n"
                << "8. Run auto tests\n"
                << "9. Exit\n"
                << "Your choice: ";
    }

    void testInsert(AVLTree<T>* tree) {
        T value;
        std::cout << "Enter value to insert: ";
        std::cin >> value;
        tree->insert(value);
        std::cout << "Value inserted. Tree height: " << getTreeHeight(tree) << "\n";
    }

    void testRemove(AVLTree<T>* tree) {
        T value;
        std::cout << "Enter value to remove: ";
        std::cin >> value;
        tree->remove(value);
        std::cout << "Operation completed.\n";
    }

    void testContains(AVLTree<T>* tree) {
        T value;
        std::cout << "Enter value to check: ";
        std::cin >> value;
        std::cout << (tree->contains(value) ? "Value exists" : "Value not found") << "\n";
    }

    void printTree(const AVLTree<T>* tree) {
        int value;
        std::cout << "Choose print type:" << '\n';
        std::cout << "1. Level" << '\n';
        std::cout << "2. Visual" << '\n';
        std::cout << "Your choice: ";
        std::cin >> value;

        switch (value) {
            case 1: printTreeLevel(tree); break;
            case 2: printTreeVisual(tree); break;
            default: std::cout << "Invalid choice!\n";
        }
    }

    void printTreeLevel(const AVLTree<T>* tree) {
        auto nodes = tree->traverse("LKP");
        int max_level = getTreeHeight(tree);
        
        for (int lvl = 1; lvl <= max_level; ++lvl) {
            std::cout << "Level " << lvl << ": ";
            for (const auto& [val, level] : nodes) {
                if (level == lvl) std::cout << val << " ";
            }
            std::cout << "\n";
        }
    }

    void printTreeVisual(const AVLTree<T>* tree) {
        tree->printTreeVisual();
    }

    void testTraversals(const AVLTree<T>* tree) {
        std::cout << "Select traversal type:\n"
                  << "KLP (Root-Left-Right)\n"
                  << "KPL (Root-Right-Left)\n"
                  << "LKP (Left-Root-Right)\n"
                  << "LPK (Left-Right-Root)\n"
                  << "PKL (Right-Root-Left)\n"
                  << "PLK (Right-Left-Root)\n"
                  << "Your choice: ";
        
        std::string choice;
        std::cin >> choice;
        
        std::cout << "Traversal result: ";
        MutableArraySequence<std::pair<T, int>> result = tree->traverse(choice);
        std::vector<std::pair<T, int>> vec;

        for (auto el : result) {
            vec.push_back(el);
        }
        printVector(vec);
    }

    template <typename U>
    int getTreeHeight(const AVLTree<U>* tree) {
        auto traversal = tree->traverse("LKP");
        if (traversal.GetLength() == 0) return 0;
        
        int max_height = 0;
        for (const auto& [value, height] : traversal) {
            if (height > max_height) max_height = height;
        }
        return max_height;
    }

    void printVector(const std::vector<std::pair<T, int>>& vec) {
        for (const auto& p : vec) {
            std::cout << p.first << "(" << p.second << ") ";
        }
        std::cout << "\n";
    }

    void testSubtreeOperations(AVLTree<T>* tree) {
        std::cout << "\n=== Subtree Operations Test ===\n";
        
        // Тест extractSubtree
        T value;
        std::cout << "Enter value to extract subtree: ";
        std::cin >> value;
        
        AVLTree<T>* subtree = tree->extractSubtree(value);
        std::cout << "Extracted subtree:\n";
        subtree->printTreeVisual();
        
        // Тест containsSubtree
        std::cout << "Subtree " << (tree->containsSubtree(subtree) ? "exists" : "does not exist") 
                  << " in main tree\n";
        
        // Тест merge
        std::cout << "\nEnter values to merge (space separated, end with 'end'):\n";
        AVLTree<T>* treeToMerge = new AVLTree<T>();
        T mergeValue;
        while (std::cin >> mergeValue) {
            treeToMerge->insert(mergeValue);
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        tree->merge(treeToMerge);
        std::cout << "After merge:\n";
        tree->printTreeVisual();
    }

    void testMapWhere() {
        std::cout << "\n=== Map/Where Operations Test ===\n";
        AVLTree<T>* tree = new AVLTree<T>();

        // Создаем тестовое дерево
        std::vector<T> testValues = getTestValues();
        for (const auto& val : testValues) {
            tree->insert(val);
        }

        // Тест map
        std::cout << "Original tree:\n";
        tree->printTreeVisual();
        
        AVLTree<T>* mappedTree = tree->map(getMapper());
        std::cout << "\nAfter mapping:\n";
        mappedTree->printTreeVisual();
        
        // Тест where
        AVLTree<T>* filteredTree = tree->where(getPredicate());
        std::cout << "\nAfter filtering:\n";
        filteredTree->printTreeVisual();
        // printVector(filteredTree->traverse(AVLTree<T>::Traversal::LKP));
        
        // Освобождаем память
        delete tree;
        delete mappedTree;
        delete filteredTree;
    }

    void runAutoTests() {
        std::cout << "\n=== Running Automatic Tests ===\n";
        
        // Тест 1: Вставка и проверка размера
        AVLTree<int>* testTree = new AVLTree<int>();
        std::vector<int> testValues = {5, 3, 7, 2, 4, 6, 8};
        for (int val : testValues) {
            testTree->insert(val);
        }
        
        bool passed = true;
        if (testTree->traverse().GetLength() != testValues.size()) {
            std::cout << "Test 1 (Insert) FAILED\n";
            passed = false;
        }
        
        // Тест 2: Удаление
        testTree->remove(5);
        if (testTree->contains(5)) {
            std::cout << "Test 2 (Remove) FAILED\n";
            passed = false;
        }
        
        // Тест 3: Поиск поддерева
        AVLTree<int>* subtree = testTree->extractSubtree(3);
        if (!testTree->containsSubtree(subtree)) {
            std::cout << "Test 3 (Subtree) FAILED\n";
            passed = false;
        }
        
        // Тест 4: Объединение деревьев
        AVLTree<int>* tree2 = new AVLTree<int>();
        tree2->insert(10);
        tree2->insert(12);
        testTree->merge(tree2);
        if (!testTree->contains(10)) {
            std::cout << "Test 4 (Merge) FAILED\n";
            passed = false;
        }
        
        // Тест 5: Map/Where
        auto mapped = testTree->map([](int x) { return x + 1; });
        if (!mapped->contains(11)) {
            std::cout << "Test 5 (Map) FAILED\n";
            passed = false;
        }
        
        auto filtered = testTree->where([](int x) { return x > 5; });
        if (filtered->contains(2)) {
            std::cout << "Test 6 (Where) FAILED\n";
            passed = false;
        }
        
        if (passed) {
            std::cout << "All tests PASSED!\n";
        }
        
        // Генерация случайного большого дерева для теста производительности
        std::cout << "\n=== Performance Test ===\n";
        AVLTree<int>* bigTree = new AVLTree<int>();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 1000000);
        
        const int bigSize = 10000;
        std::cout << "Inserting " << bigSize << " random elements...\n";
        for (int i = 0; i < bigSize; ++i) {
            bigTree->insert(dis(gen));
        }
        std::cout << "Tree height: " << getTreeHeight(bigTree) << "\n";
    }

private:
    std::vector<T> getTestValues() {
        if constexpr (std::is_same_v<T, int>) {
            return {5, 3, 7, 2, 4, 6, 8};
        } else if constexpr (std::is_same_v<T, double>) {
            return {5.1, 3.2, 7.3, 2.4, 4.5, 6.6, 8.7};
        } else if constexpr (std::is_same_v<T, std::string>) {
            return {"apple", "banana", "cherry", "date", "fig", "grape", "kiwi"};
        } else {
            return {};
        }
    }

    std::function<T(const T&)> getMapper() {
        if constexpr (std::is_same_v<T, int>) {
            return [](const T& val) { return val * 2; };
        } else if constexpr (std::is_same_v<T, double>) {
            return [](const T& val) { return val * 1.5; };
        } else if constexpr (std::is_same_v<T, std::string>) {
            return [](const T& val) { return val + val; }; // Конкатенация строк
        } else {
            return [](const T& val) { return val; };
        }
    }

    std::function<bool(const T&)> getPredicate() {
        if constexpr (std::is_same_v<T, int>) {
            return [](const T& val) { return val % 2 == 0; };
        } else if constexpr (std::is_same_v<T, double>) {
            return [](const T& val) { return static_cast<int>(val) % 2 == 0; };
        } else if constexpr (std::is_same_v<T, std::string>) {
            return [](const T& val) { return val.length() > 4; }; // Длина строки > 4
        } else {
            return [](const T&) { return true; };
        }
    }
};

template <>
inline void AVLTreeTester<std::string>::printVector(const std::vector<std::pair<std::string, int>>& vec) {
    for (const auto& p : vec) {
        std::cout << "\"" << p.first << "\"(" << p.second << ") ";
    }
    std::cout << "\n";
}

template <>
inline std::vector<Student> AVLTreeTester<Student>::getTestValues() {
    return {
        Student(PersonID("1234", "567890"), "Ivan", "Ivanovich", "Ivanov", 0, "S12345"),
        Student(PersonID("2345", "678901"), "Petr", "Petrovich", "Petrov", 0, "S23456"),
        Student(PersonID("3456", "789012"), "Anna", "Andreevna", "Sidorova", 0, "S34567")
    };
}

template <>
inline std::function<Student(const Student&)> AVLTreeTester<Student>::getMapper() {
    return [](const Student& s) {
        return Student(s.GetID(), s.GetFirstName(), s.GetMiddleName(), s.GetLastName(), 
                      s.GetBirthDate(), "Mapped_" + s.GetStudentId());
    };
}

template <>
inline std::function<bool(const Student&)> AVLTreeTester<Student>::getPredicate() {
    return [](const Student& s) {
        return s.GetStudentId().back() == '5' || s.GetStudentId().back() == '7';
    };
}

template <>
inline void AVLTreeTester<Student>::printVector(const std::vector<std::pair<Student, int>>& vec) {
    for (const auto& p : vec) {
        std::cout << p.first << " (Level: " << p.second << ")\n";
    }
}

template <>
inline std::vector<Teacher> AVLTreeTester<Teacher>::getTestValues() {
    return {
        Teacher(PersonID("4567", "890123"), "Sergey", "Sergeevich", "Smirnov", 0, "Mathematics"),
        Teacher(PersonID("5678", "901234"), "Olga", "Olegovna", "Kuznetsova", 0, "Physics"),
        Teacher(PersonID("6789", "012345"), "Dmitry", "Dmitrievich", "Volkov", 0, "Computer Science")
    };
}

template <>
inline std::function<Teacher(const Teacher&)> AVLTreeTester<Teacher>::getMapper() {
    return [](const Teacher& t) {
        return Teacher(t.GetID(), t.GetFirstName(), t.GetMiddleName(), t.GetLastName(), 
                       t.GetBirthDate(), "Mapped_" + t.GetDepartment());
    };
}

template <>
inline std::function<bool(const Teacher&)> AVLTreeTester<Teacher>::getPredicate() {
    return [](const Teacher& t) {
        return t.GetDepartment().find("Science") != std::string::npos;
    };
}

template <>
inline void AVLTreeTester<Teacher>::printVector(const std::vector<std::pair<Teacher, int>>& vec) {
    for (const auto& p : vec) {
        std::cout << p.first << " (Level: " << p.second << ")\n";
    }
}

class AVLTreeTesterFactory {
public:
    static AVLTreeTesterBase* createTester() {
        std::cout << "Select data type:\n"
                  << "1. int\n"
                  << "2. double\n"
                  << "3. std::string\n"
                  << "4. Student (auto tests)\n"
                  << "5. Teacher (auto tests)\n"
                  << "Your choice: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: return new AVLTreeTester<int>();
            case 2: return new AVLTreeTester<double>();
            case 3: return new AVLTreeTester<std::string>();
            case 4: return new AVLTreeTester<Student>();
            case 5: return new AVLTreeTester<Teacher>();
            default:
                std::cout << "Invalid choice, using int by default\n";
                return new AVLTreeTester<int>();
        }
    }
};

template <>
inline void AVLTreeTester<Student>::runAutoTests() {
    std::cout << "\n=== Running Student Automatic Tests ===\n";
    
    AVLTree<Student>* testTree = new AVLTree<Student>();
    std::vector<Student> testValues = getTestValues();
    for (const auto& val : testValues) {
        testTree->insert(val);
    }
    
    bool passed = true;
    
    // Тест 1: Вставка
    if (testTree->traverse().GetLength() != testValues.size()) {
        std::cout << "Test 1 (Insert) FAILED\n";
        passed = false;
    }
    
    // Тест 2: Удаление
    Student toRemove = testValues[0];
    testTree->remove(toRemove);
    if (testTree->contains(toRemove)) {
        std::cout << "Test 2 (Remove) FAILED\n";
        passed = false;
    }
    
    // Тест 3: Поддерево
    AVLTree<Student>* subtree = testTree->extractSubtree(testValues[1]);
    if (!testTree->containsSubtree(subtree)) {
        std::cout << "Test 3 (Subtree) FAILED\n";
        passed = false;
    }
    delete subtree;
    
    // Тест 4: Объединение
    AVLTree<Student>* tree2 = new AVLTree<Student>();
    Student newStudent(PersonID("9999", "999999"), "New", "Student", "User", 0, "S99999");
    tree2->insert(newStudent);
    testTree->merge(tree2);
    if (!testTree->contains(newStudent)) {
        std::cout << "Test 4 (Merge) FAILED\n";
        passed = false;
    }
    delete tree2;
    
    // Тест 5: Map
    auto mapped = testTree->map([](const Student& s) {
        return Student(s.GetID(), s.GetFirstName(), s.GetMiddleName(), 
                      s.GetLastName(), s.GetBirthDate(), "Mapped_" + s.GetStudentId());
    });
    if (!mapped->contains(Student(PersonID("9999", "999999"), "New", "Student", 
                                 "User", 0, "Mapped_S99999"))) {
        std::cout << "Test 5 (Map) FAILED\n";
        passed = false;
    }
    delete mapped;
    
    // Тест 6: Where
    auto filtered = testTree->where([](const Student& s) {
        return s.GetStudentId().back() == '6';
    });
    bool whereTestPassed = true;
    if (filtered->contains(testValues[0])) {
        std::cout << "Test 6 (Where) FAILED\n";
        whereTestPassed = false;
    }
    passed = passed && whereTestPassed;
    delete filtered;
    
    if (passed) {
        std::cout << "All Student tests PASSED!\n";
    }
    
    delete testTree;
}

template <>
inline void AVLTreeTester<Teacher>::runAutoTests() {
    std::cout << "\n=== Running Teacher Automatic Tests ===\n";
    
    // Тест 1: Вставка и проверка размера
    AVLTree<Teacher>* testTree = new AVLTree<Teacher>();
    std::vector<Teacher> testValues = getTestValues();
    for (const auto& val : testValues) {
        testTree->insert(val);
    }
    
    bool passed = true;
    if (testTree->traverse().GetLength() != testValues.size()) {
        std::cout << "Test 1 (Insert) FAILED\n";
        passed = false;
    }
    
    // Тест 2: Удаление
    Teacher toRemove = testValues[0];
    testTree->remove(toRemove);
    if (testTree->contains(toRemove)) {
        std::cout << "Test 2 (Remove) FAILED\n";
        passed = false;
    }
    
    // Тест 3: Поиск поддерева
    AVLTree<Teacher>* subtree = testTree->extractSubtree(testValues[1]);
    if (!testTree->containsSubtree(subtree)) {
        std::cout << "Test 3 (Subtree) FAILED\n";
        passed = false;
    }
    
    // Тест 4: Объединение деревьев
    AVLTree<Teacher>* tree2 = new AVLTree<Teacher>();
    Teacher newTeacher(PersonID("9999", "999999"), "New", "Teacher", "User", 0, "New Department");
    tree2->insert(newTeacher);
    testTree->merge(tree2);
    if (!testTree->contains(newTeacher)) {
        std::cout << "Test 4 (Merge) FAILED\n";
        passed = false;
    }
    
    // Тест 5: Map/Where
    auto mapped = testTree->map([](const Teacher& t) {
        return Teacher(t.GetID(), t.GetFirstName(), t.GetMiddleName(), 
                      t.GetLastName(), t.GetBirthDate(), "Mapped_" + t.GetDepartment());
    });
    if (!mapped->contains(Teacher(PersonID("9999", "999999"), "New", "Teacher", 
                                 "User", 0, "Mapped_New Department"))) {
        std::cout << "Test 5 (Map) FAILED\n";
        passed = false;
    }
    
    auto filtered = testTree->where([](const Teacher& t) {
        return t.GetDepartment().find("Science") != std::string::npos;
    });
    if (filtered->contains(testValues[0]) && testValues[0].GetDepartment().find("Science") == std::string::npos) {
        std::cout << "Test 6 (Where) FAILED\n";
        passed = false;
    }
    
    if (passed) {
        std::cout << "All Teacher tests PASSED!\n";
    }
    
    delete testTree;
    delete subtree;
    delete tree2;
    delete mapped;
    delete filtered;
}
#pragma once
#include "AVLTree.hpp"
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <random>

template <typename T>
class AVLTreeTester {
public:
    void runInteractiveTest() {
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
        auto nodes = tree->traverse(AVLTree<T>::Traversal::LKP); // Используем новый метод
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
                  << "1. KLP (Root-Left-Right)\n"
                  << "2. KPL (Root-Right-Left)\n"
                  << "3. LKP (Left-Root-Right)\n"
                  << "4. LPK (Left-Right-Root)\n"
                  << "5. PKL (Right-Root-Left)\n"
                  << "6. PLK (Right-Left-Root)\n"
                  << "Your choice: ";
        
        int choice;
        std::cin >> choice;
        
        typename AVLTree<T>::Traversal type;
        switch (choice) {
            case 1: type = AVLTree<T>::Traversal::KLP; break;
            case 2: type = AVLTree<T>::Traversal::KPL; break;
            case 3: type = AVLTree<T>::Traversal::LKP; break;
            case 4: type = AVLTree<T>::Traversal::LPK; break;
            case 5: type = AVLTree<T>::Traversal::PKL; break;
            case 6: type = AVLTree<T>::Traversal::PLK; break;
            default:
                std::cout << "Invalid choice, using LKP by default\n";
                type = AVLTree<T>::Traversal::LKP;
        }
        
        std::cout << "Traversal result: ";
        printVector(tree->traverse(type));
    }

    int getTreeHeight(const AVLTree<T>* tree) {
        auto traversal = tree->traverse(AVLTree<T>::Traversal::LKP);
        if (traversal.empty()) return 0;
        
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
        
        tree->merge(treeToMerge);
        std::cout << "After merge:\n";
        tree->printTreeVisual();
    }

    void testMapWhere() {
        std::cout << "\n=== Map/Where Operations Test ===\n";
        AVLTree<T>* tree = new AVLTree<T>();
        
        // Создаем тестовое дерево
        std::vector<T> testValues = {5, 3, 7, 2, 4, 6, 8};
        for (const auto& val : testValues) {
            tree->insert(val);
        }
        
        // Тест map
        std::cout << "Original tree:\n";
        tree->printTreeVisual();
        
        AVLTree<T>* mappedTree = tree->map([](const T& val) { return val * 2; });
        std::cout << "\nAfter mapping (values * 2):\n";
        mappedTree->printTreeVisual();
        
        // Тест where
        AVLTree<T>* filteredTree = tree->where([](const T& val) { return val % 2 == 0; });
        std::cout << "\nAfter filtering (even values only):\n";
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
        if (testTree->traverse().size() != testValues.size()) {
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
};

template <>
inline void AVLTreeTester<std::string>::printVector(const std::vector<std::pair<std::string, int>>& vec) {
    for (const auto& p : vec) {
        std::cout << "\"" << p.first << "\"(" << p.second << ") ";
    }
    std::cout << "\n";
}
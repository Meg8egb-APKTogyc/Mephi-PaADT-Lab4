#pragma once
#include "Sequence/Sequence.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <queue>
#include <math.h>
#include <iomanip>
#include <sstream>


template<typename T>
class AVLTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        int height, size;

        Node(const T& val) : data(val), left(nullptr), right(nullptr), height(1), size(1) {}
    };

    Node* root;

private:
    int height(Node* node) const {
        return node ? node->height : 0;
    }

    int size(Node* node) const {
        return node ? node->size : 0;
    }

    int balanceFactor(Node* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    void updateNode(Node* node) {
        if (node) {
            node->height = 1 + std::max(height(node->left), height(node->right));
            node->size = 1 + (node->left ? node->left->size : 0) + (node->right ? node->right->size : 0);
        }
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateNode(y);
        updateNode(x);

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateNode(x);
        updateNode(y);

        return y;
    }

    Node* balance(Node* node) {
        if (!node) return node;

        int bf = balanceFactor(node);

        if (bf > 1 && balanceFactor(node->left) >= 0) {
            return rotateRight(node);
        } else if (bf < -1 && balanceFactor(node->right) <= 0) {
            return rotateLeft(node);
        } else if (bf > 1 && balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            updateNode(node);
            return rotateRight(node);
        } else if (bf < -1 && balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            updateNode(node);
            return rotateLeft(node);
        }

        updateNode(node);

        return node;
    }

    Node* insert(Node* node, const T& val) {
        if (!node) return new Node(val);

        if (val < node->data)
            node->left = insert(node->left, val);
        else if (node->data < val)
            node->right = insert(node->right, val);
        else
            return node;
        
        return balance(node);
    }

    Node* findMin(Node* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;   
    }

    Node* remove(Node* node, const T& val) {
        if (!node) return node;

        if (val < node->data) {
            node->left = remove(node->left, val);
        } else if (node->data < val) {
            node->right = remove(node->right, val);
        } else {
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                if (!temp) {
                    delete node;
                    return nullptr;
                } else {
                    node->data = temp->data;
                    node->left = temp->left;
                    node->right = temp->right;

                    delete temp;
                }
            } else {
                Node* temp = findMin(node->right);
                node->data = temp->data;
                node->right = remove(node->right, temp->data);
            }
        }

        updateNode(node);
        return balance(node);
    }

    bool contains(Node* node, const T& val) {
        if (!node) return false;

        if (val < node->data)
            return contains(node->left, val);
        else if (node->data < val)
            return contains(node->right, val);

        return true;
    }

    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }
    
    void traverse(Node* node, Sequence<std::pair<T, int>>& result, int height, std::string type="LKP") const {
        if (!node) return;
        
        for (int i = 0; i < 3; ++i) {
            if (type[i] == 'K') result.Append(std::make_pair(node->data, height));
            else if (type[i] == 'L') traverse(node->left, result, height + 1, type);
            else traverse(node->right, result, height + 1, type);
        }
    }

    Node* mapTree(Node* node, const std::function<T(const T&)>& func) {
        if (!node) return nullptr;

        Node* newNode = new Node(func(node->data));
        newNode->left = mapTree(node->left, func);
        newNode->right = mapTree(node->right, func);

        updateNode(newNode);
        return newNode;
    }

    Node* whereTree(Node* node, const std::function<bool(const T&)>& predicate) {
        if (!node) return nullptr;

        Node* left = whereTree(node->left, predicate);
        Node* right = whereTree(node->right, predicate);

        if (predicate(node->data)) {
            Node* newNode = new Node(node->data);
            newNode->left = left;
            newNode->right = right;
            updateNode(newNode);
            return newNode;
        }
        
        if (left && right) {
            MutableArraySequence<std::pair<T, int>> vec;
            traverse(left, vec, 1, "LKP");

            for (int i = 0; i < vec.GetLength(); ++i) {
                auto val = vec.Get(i).first;
                right = insert(right, val);
            }
            
            return right;
        }
        
        return left ? left : right;
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        clear();
    }

    int size() {
        return this->size(root);
    }

    void insert(const T& val) {
        root = insert(root, val);
    }

    void remove(const T& val) {
        root = remove(root, val);
    }

    bool contains(const T& val) {
        return this->contains(root, val);
    }

    void clear() {
        clear(root);
        root = nullptr;
    }

    bool empty() const {
        return root == nullptr;
    }
    
    MutableArraySequence<std::pair<T, int>> traverse(std::string type="LKP") const {
        MutableArraySequence<std::pair<T, int>> result;
        if (root) traverse(root, result, 1, type);
        return result;
    }  
    
    AVLTree<T>* map(const std::function<T(const T&)>& func) {
        AVLTree<T>* newTree = new AVLTree<T>();
        newTree->root = mapTree(root, func);
        return newTree;
    }

    AVLTree<T>* where(const std::function<bool(const T&)>& predicate) {
        AVLTree<T>* newTree = new AVLTree<T>();
        bool changed = false;
        newTree->root = whereTree(root, predicate);
        return newTree;
    }

private:
    int maxDepth(Node* node) const {
        if (!node) return 0;
        return 1 + std::max(maxDepth(node->left), maxDepth(node->right));
    }

    Node* findNode(Node* node, const T& val) const {
        if (!node) return nullptr;

        if (val < node->data)
            return findNode(node->left, val);
        else if (node->data < val)
            return findNode(node->right, val);
        else
            return node;
    }

    void copySubtree(Node* src, Node*& dest) const {
        if (!src) return;
        
        dest = new Node(src->data);
        dest->height = src->height;
        dest->size = src->size;
        
        copySubtree(src->left, dest->left);
        copySubtree(src->right, dest->right);
    }

    void findCandidates(Node* node, const T& target, Sequence<Node*>& candidates) const {
        if (!node) return;
        
        if (node->data == target) {
            candidates.Append(node);
        }
        
        findCandidates(node->left, target, candidates);
        findCandidates(node->right, target, candidates);
    }

    bool compareSubtrees(Node* treeNode, Node* subtreeNode) const {
        if (!subtreeNode) return true;
        if (!treeNode) return false;
        
        return treeNode->data == subtreeNode->data &&
               compareSubtrees(treeNode->left, subtreeNode->left) &&
               compareSubtrees(treeNode->right, subtreeNode->right);
    }

    static Node* buildTree(const Sequence<T>& elements, size_t& index, std::string type) {
        if (index >= elements.size()) return nullptr;

        Node* node = nullptr;

        for (int i = 0; i < 3; ++i) {
            if (type[i] == 0)
                node = new Node(elements[index++]);
            else if (type[i] == 1)
                node->left = buildTree(elements, index, type);
            else
                node->right = buildTree(elements, index, type);
        }

        return node;
    }

public:
    void printTreeVisual(std::ostream& os = std::cout) const {
        if (!root) {
            os << "(empty tree)\n";
            return;
        }

        struct PrintNode {
            Node* node;
            int level;
            int pos;
        };

        int tree_height = maxDepth(root);
        int max_pos = 1 << (tree_height + 1);

        MutableArraySequence<std::string> levels;
        for (int i = 0; i < tree_height; ++i) {
            levels.Append(std::string(max_pos, ' '));
        }

        std::queue<PrintNode> q;
        q.push({root, 0, max_pos / 2});

        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            if (current.node) {
                std::string val_str;
                if constexpr (std::is_same_v<T, int>) {
                    val_str = std::to_string(current.node->data);
                } else if constexpr (std::is_same_v<T, double>) {
                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(1) << current.node->data;
                    val_str = ss.str();
                } else if constexpr (std::is_same_v<T, std::string>) {
                    val_str = current.node->data;
                }

                int start_pos = current.pos - val_str.length() / 2;
                
                for (int i = 0; i < val_str.length(); i++) {
                    if (start_pos + i >= 0 && start_pos + i < max_pos) {
                        levels[current.level][start_pos + i] = val_str[i];
                    }
                }

                int offset = max_pos >> (current.level + 2);
                if (current.node->left) {
                    q.push({current.node->left, current.level + 1, current.pos - offset});
                }
                if (current.node->right) {
                    q.push({current.node->right, current.level + 1, current.pos + offset});
                }
            }
        }

        for (const auto& level : levels) {
            size_t end = level.find_last_not_of(' ');
            if (end != std::string::npos) {
                os << level.substr(0, end + 1) << "\n";
            } else {
                os << "\n";
            }
        }
    }

    void merge(const AVLTree<T>* other) {
        MutableArraySequence<std::pair<T, int>> elements = other->traverse("LKP");
        
        for (auto val : elements)
            this->insert(val.first);
    }

    AVLTree<T>* extractSubtree(const T& val) const {
        AVLTree<T>* subtree = new AVLTree<T>();
        Node* subRoot = findNode(root, val);
        
        if (subRoot) {
            copySubtree(subRoot, subtree->root);
        }
        return subtree;
    }

    bool containsSubtree(AVLTree<T>* subtree) const {
        if (!subtree || subtree->empty()) return true;
    
        MutableArraySequence<Node*> candidates;
        findCandidates(root, subtree->root->data, candidates);
        
        for (Node* candidate : candidates) {
            if (compareSubtrees(candidate, subtree->root)) {
                return true;
            }
        }
        return false;
    }

    static AVLTree<T>* buildFromTraversal(const Sequence<T>& elements, std::string type) {
        AVLTree<T>* tree = new AVLTree<T>();
        if (elements.empty()) return tree;

        size_t index = 0;
        tree.root = buildTree(elements, index, type);

        if (index != elements.size()) {
            throw std::invalid_argument("Invalid traversal sequence for tree construction");
        }

        return tree;
    }
};
#pragma once
#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include <queue>


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

public:
    enum class Traversal {
        KLP,  // Корень-Лево-Право (pre-order)
        KPL,  // Корень-Право-Лево
        LKP,  // Лево-Корень-Право (in-order)
        LPK,  // Лево-Право-Корень (post-order)
        PKL,  // Право-Корень-Лево
        PLK   // Право-Лево-Корень
    };

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
            node->size = 1 + size(node->left) + size(node->right);
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

        updateNode(y);
        updateNode(x);

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
            return rotateRight(node);
        } else if (bf < -1 && balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
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

        if (val < node->data)
            node->left = remove(node->left, val);
        else if (node->data < val)
            node->right = remove(node->right, val);
        else {
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;

                if (!temp) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }

                delete temp;
            } else {
                Node* temp = findMin(node->right);
                node->data = temp->data;
                node->right = remove(node->right, temp->data);
            }
        }

        if (!node) return node;

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
    
    void traverse(Node* node, std::vector<std::pair<T, int>>& result, int height, Traversal type) const {
        if (!node) return;

        static constexpr std::array<std::array<int, 3>, 6> orders = {{
            {0, 1, 2}, // KLP
            {0, 2, 1}, // KPL
            {1, 0, 2}, // LKP
            {1, 2, 0}, // LPK
            {2, 0, 1}, // PKL
            {2, 1, 0}  // PLK
        }};
        
        const auto& order = orders[static_cast<int>(type)];
        
        for (int step : order) {
            if (step == 0) result.emplace_back(node->data, height);
            else if (step == 1) traverse(node->left, result, height + 1, type);
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
            std::vector<std::pair<T, int>> vec;
            traverse(left, vec, 1, Traversal::LKP);

            for (auto [val, hei] : vec)
                right = insert(right, val);
            
            return right;
        }
        
        return left ? left : right;
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        clear();
    }

    void insert(const T& val) {
        root = insert(root, val);
    }

    void remove(const T& val) {
        root = remove(root, val);
    }

    bool contains(const T& val) {
        return contains(root, val);
    }

    void clear() {
        clear(root);
        root = nullptr;
    }

    bool empty() const {
        return root == nullptr;
    }
    
    std::vector<std::pair<T, int>> traverse(Traversal type = Traversal::LKP) const {
        std::vector<std::pair<T, int>> result;
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

    void findCandidates(Node* node, const T& target, std::vector<Node*>& candidates) const {
        if (!node) return;
        
        if (node->data == target) {
            candidates.push_back(node);
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

    static Node* buildTree(const std::vector<T>& elements, size_t& index, Traversal type) {
        if (index >= elements.size()) return nullptr;

        static constexpr std::array<std::array<int, 3>, 6> buildOrders = {{
            {0, 1, 2}, // KLP
            {0, 2, 1}, // KPL
            {1, 0, 2}, // LKP
            {1, 2, 0}, // LPK
            {2, 0, 1}, // PKL
            {2, 1, 0}  // PLK
        }};

        const auto& order = buildOrders[static_cast<int>(type)];
        Node* node = nullptr;

        for (int step : order) {
            if (step == 0) {
                node = new Node(elements[index++]);
            } 
            else if (step == 1) {
                node->left = buildTree(elements, index, type);
            } 
            else {
                node->right = buildTree(elements, index, type);
            }
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
        std::vector<std::string> levels(tree_height, std::string(max_pos, ' '));

        std::queue<PrintNode> q;
        q.push({root, 0, max_pos / 2});

        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            if (current.node) {
                std::string val_str = std::to_string(current.node->data);
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
        std::vector<std::pair<T, int>> elements = other->traverse(Traversal::LKP);
        
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
        if (subtree->empty()) return true;
        
        auto subtreeRoot = subtree->traverse(AVLTree<T>::Traversal::KLP).front().first;
        std::vector<Node*> candidates;
        findCandidates(root, subtreeRoot, candidates);
        
        for (Node* candidate : candidates) {
            if (compareSubtrees(candidate, subtree->root)) {
                return true;
            }
        }
        return false;
    }

    static AVLTree<T>* buildFromTraversal(const std::vector<T>& elements, Traversal type) {
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
#pragma once
#include "AVLTree.hpp"


template<typename T>
class Set {
private:
    AVLTree<T>* tree;

public:
    Set() : tree(new AVLTree<T>()) {}

    template<typename Sequence>
    Set(const Sequence& sequence) : tree(new AVLTree<T>()) {
        for (const auto& item : sequence) {
            this->insert(item);
        }
    }

    ~Set() {
        delete tree;
    }

    void insert(const T& value) {
        if (!this->tree->contains(value))
            this->tree->insert(value);
    }

    void remove(const T& value) {
        this->tree->remove(value);
    }

    bool contains(const T& value) const {
        return this->tree->contains(value);
    }

    bool empty() const {
        return this->tree->empty();
    }

    void clear() {
        this->tree->clear();
    }

    int size() const {
        return this->tree->size();
    }

    Set<T>* unionWith(const Set<T>* other) const {
        Set<T>* result = new Set<T>();
        auto elements = this->tree->traverse();

        for (const auto& pair : elements) {
            result->insert(pair.first);
        }

        auto otherElements = other->tree->traverse();
        for (const auto& pair : otherElements) {
            result->insert(pair.first);
        }


        return result;
    }

    Set<T>* intersectionWith(const Set<T>* other) {
        Set<T>* result = new Set<T>();
        auto elements = this->tree->traverse();
        for (const auto& pair : elements) {
            if (other->contains(pair.first)) {
                result->insert(pair.first);
            }
        }
        return result;
    }

    Set<T>* differenceWith(const Set<T>* other) {
        Set<T>* result = new Set<T>();
        auto elements = this->tree->traverse();
        for (const auto& pair : elements) {
            if (!other->contains(pair.first)) {
                result->insert(pair.first);
            }
        }
        return result;
    }

    void print() const {
        auto elements = tree->traverse();
        std::cout << "{ ";
        for (const auto& pair : elements) {
            std::cout << pair.first << " ";
        }
        std::cout << "}" << std::endl;
    }

    bool isSubsetOf(const Set<T>* other) const {
        auto elements = tree->traverse();
        for (const auto& pair : elements) {
            if (!other->contains(pair.first)) {
                return false;
            }
        }
        return true;
    }

    bool equals(const Set<T>* other) const {
        return this->isSubsetOf(other) && other->isSubsetOf(this);
    }

    Set<T>* operator+(const Set<T>* other) const {
        return this->unionWith(other);
    }
    
    Set<T>* operator*(const Set<T>* other) const {
        return this->intersectionWith(other);
    }
    
    Set<T>* operator-(const Set<T>* other) const {
        return this->differenceWith(other);
    }
    
    Set<T>* operator^(const Set<T>* other) const {
        return this->symmetricDifferenceWith(other);
    }
    
    bool operator<=(const Set<T>* other) const {
        return this->isSubsetOf(other);
    }
    
    bool operator==(const Set<T>* other) const {
        return this->equals(other);
    }

    Set<T>* map(const std::function<T(const T&)>& func) const {
        Set<T> result = new Set<T>();
        auto elements = tree->traverse();
        for (const auto& pair : elements) {
            result->insert(func(pair.first));
        }
        return result;
    }
    
    Set<T> where(const std::function<bool(const T&)>& predicate) const {
        Set<T> result = new Set<T>();
        auto elements = tree->traverse();
        for (const auto& pair : elements) {
            if (predicate(pair.first)) {
                result->insert(pair.first);
            }
        }
        return result;
    }
    
    T reduce(const std::function<T(const T&, const T&)>& func, const T& initial) const {
        T result = initial;
        auto elements = tree->traverse();
        for (const auto& pair : elements) {
            result = func(result, pair.first);
        }
        return result;
    }
    
    AVLTree<T>* getTree() const {
        return tree;
    }
};


template<typename T>
std::ostream& operator<<(std::ostream& os, const Set<T>* set) {
    os << "{ ";
    bool first = true;
    auto elements = set->getTree()->traverse();
    for (const auto& pair : elements) {
        if (!first) {
            os << ", ";
        }
        os << pair.first;
        first = false;
    }
    os << " }";
    return os;
}
#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

template<class TypeKey, class TypeValue>
class Tree {
private:

    class Node {
    private:

        TypeKey key;
        TypeValue value;
        Node* left;
        Node* right;

        friend class Tree;

    public:
        Node(const TypeKey& key, const TypeValue& value) {
            this->key = key;
            this->value = value;
            this->left = nullptr;
            this->right = nullptr;
        }

        TypeValue& getValue() { return this->value; }
        TypeKey& getKey() { return this->key; }
    };

    class Iterator_base {
    protected:
        Node* current;
        Node* parent;

        std::vector<Node*> nodes;

        virtual void fillNodes(Node* n) = 0;

        friend class Tree;

    public:
        Iterator_base();
        Iterator_base(Node* node, Node* parent);
        Iterator_base(Tree<TypeKey, TypeValue>* tr);
        Node* operator*() const;
        bool operator==(const Iterator_base& anotherIt) const;
        bool operator!=(const Iterator_base& anotherIt) const;
        Iterator_base& operator++(int);
        Iterator_base& operator--(int);
    };

    Node* _begin;
    size_t length;
    mutable size_t operations;
    mutable std::vector<TypeKey> keys;

    // Вспомогательные методы
    void get_keys_helper(Node* n) const;
    Node* get_helper(Node* n, const TypeKey& key) const;
    Node* insert_helper(Node* n, const TypeKey& key, const TypeValue& value);
    void print_helper(Node* n) const;
    void print_elegant(Node* node, int indent) const;
    void copy_helper(Node* n);
    void clear_helper(Node* n);
    Node* remove_helper(Node* n, const TypeKey& key);
    size_t height_helper(Node* nl) const;
    Node* get_begin() const { return this->_begin; }

public:
    class Iterator : public Iterator_base {
    protected:
        virtual void fillNodes(Node* n);

    public:
        Iterator(Node* node, Node* parent);
        Iterator(Tree<TypeKey, TypeValue>* tr);
    };

    class Inverse_iterator : public Iterator_base {
    protected:
        virtual void fillNodes(Node* n);

    public:
        Inverse_iterator(Node* node, Node* parent);
        Inverse_iterator(Tree<TypeKey, TypeValue>* tr);
    };

    Tree();
    Tree(const Tree& anotherTree);
    ~Tree();
    size_t size() const;
    void clear();
    bool isEmpty() const;
    Node& get(const TypeKey& key);
    void insert(const TypeKey& key, const TypeValue& value);
    void remove(const TypeKey& key);
    std::vector<TypeKey> getKeys() const;
    size_t height() const;
    Iterator begin();
    Iterator end();
    Inverse_iterator rBegin();
    Inverse_iterator rEnd();
    size_t getLastOperationIterations() const;
    void print() const;
};


template<class TypeKey, class TypeValue>
Tree<TypeKey, TypeValue>::Tree() {
    this->length = 0;
    this->_begin = nullptr;
    this->operations = 0;
}

template<class TypeKey, class TypeValue>
Tree<TypeKey, TypeValue>::Tree(const Tree& anotherTree) {
    clear();
    copy_helper(_begin);
}

template<class TypeKey, class TypeValue>
Tree<TypeKey, TypeValue>::~Tree() {
    clear_helper(this->_begin);
}

template<class TypeKey, class TypeValue>
void Tree<TypeKey, TypeValue>::get_keys_helper(Node* n) const {
    if (n != nullptr) {
        get_keys_helper(n->left);
        keys.push_back(n->key);
        get_keys_helper(n->right);
    }
}

template<class TypeKey, class TypeValue>
void Tree<TypeKey, TypeValue>::copy_helper(Node* n) {
    if (n != nullptr) {
        get_keys_helper(n->left);
        insert(n->key, n->value);
        get_keys_helper(n->right);
    }
}

template<class TypeKey, class TypeValue>
size_t Tree<TypeKey, TypeValue>::size() const {
    return length;
}

template<class TypeKey, class TypeValue>
void Tree<TypeKey, TypeValue>::clear_helper(Node* n) {
    if (n != nullptr) {
        clear_helper(n->left);
        clear_helper(n->right);
        delete n;
        n = nullptr;
    }
}

template<class TypeKey, class TypeValue>
void Tree<TypeKey, TypeValue>::clear() {
    this->length = 0;
    clear_helper(this->_begin);
    this->_begin = nullptr;
}

template<class TypeKey, class TypeValue>
bool Tree<TypeKey, TypeValue>::isEmpty() const {
    return length == 0;
}

template<class TypeKey, class TypeValue>
typename Tree<TypeKey, TypeValue>::Node*
Tree<TypeKey, TypeValue>::get_helper(Node* n, const TypeKey& key) const {
    this->operations++;

    if (n == nullptr || key == n->key)
        return n;
    if (key < n->key)
        return get_helper(n->left, key);
    else
        return get_helper(n->right, key);
}


template<class TypeKey, class TypeValue>
typename Tree<TypeKey, TypeValue>::Node& Tree<TypeKey, TypeValue>::get(const TypeKey& key) {
    this->operations = 0;
    Node* temp = get_helper(this->_begin, key);
    if (temp == nullptr)
        throw std::exception("Элемент не найден!\n");

    return *temp;
}

template<class TypeKey, class TypeValue>
typename Tree<TypeKey, TypeValue>::Node*
Tree<TypeKey, TypeValue>::insert_helper(Node* n, const TypeKey& key, const TypeValue& value) {
    this->operations++;

    if (n == nullptr)
        return new Node(key, value);

    if (key < n->key)
        n->left = insert_helper(n->left, key, value);
    else if (key > n->key)
        n->right = insert_helper(n->right, key, value);
    else if (key == n->key)
        throw std::exception("Такой ключ уже существует!\n");

    return n;
}

template<class TypeKey, class TypeValue>
void Tree<TypeKey, TypeValue>::insert(const TypeKey& key, const TypeValue& value) {
    try
    {
        this->operations = 0;
        this->_begin = insert_helper(this->_begin, key, value);
        this->length++;
    }
    catch (const std::exception& ex)
    {
        //std::cout << ex.what();
    }
}

template<class TypeKey, class TypeValue>
typename Tree<TypeKey, TypeValue>::Node*
Tree<TypeKey, TypeValue>::remove_helper(Node* n, const TypeKey& key) {
    this->operations++;

    if (n == nullptr)
        throw std::exception("Элемент не найден!");

    if (key < n->key)
        n->left = remove_helper(n->left, key);
    else if (key > n->key)
        n->right = remove_helper(n->right, key);
    else if (n->left != nullptr && n->right != nullptr)
    {
        Node* temp = n->right;
        while (temp->left != nullptr)
            temp = temp->left;

        n->key = temp->key;
        n->value = temp->value;
        n->right = remove_helper(n->right, n->key);
    }
    else
    {
        if (n->left != nullptr)
        {
            Node* temp = n->left;
            n->key = temp->key;
            n->value = temp->value;
            n->left = temp->left;
            n->right = temp->right;

            temp->left = nullptr;
            temp->right = nullptr;

            delete temp;
        }
        else if (n->right != nullptr)
        {
            Node* temp = n->right;
            n->key = temp->key;
            n->value = temp->value;
            n->left = temp->left;
            n->right = temp->right;

            temp->left = nullptr;
            temp->right = nullptr;

            delete temp;
        }
        else
        {
            delete n;
            n = nullptr;
        }
    }
    return n;
}

template<class TypeKey, class TypeValue>
void Tree<TypeKey, TypeValue>::remove(const TypeKey& key) {
    this->operations = 0;
    try
    {
        this->_begin = remove_helper(this->_begin, key);
        this->length--;
        if (length == 0)
            this->_begin = nullptr;
    }
    catch (const std::exception& ex)
    {
        //std::cout << ex.what();
    }
}

template<class TypeKey, class TypeValue>
std::vector<TypeKey> Tree<TypeKey, TypeValue>::getKeys() const {
    keys.clear();
    keys.reserve(this->length);
    get_keys_helper(this->_begin);

    return keys;
}

template<class TypeKey, class TypeValue>
size_t Tree<TypeKey, TypeValue>::height_helper(Node* n) const {
    int left_depth, right_depth;
    if (n == nullptr)
        return 0;

    if (n->left != nullptr)
        left_depth = height_helper(n->left);
    else
        left_depth = -1;

    if (n->right != nullptr)
        right_depth = height_helper(n->right);
    else
        right_depth = -1;
    int max = left_depth > right_depth ? left_depth : right_depth;
    return max + 1;
}

template<class TypeKey, class TypeValue>
size_t Tree<TypeKey, TypeValue>::height() const {
    return height_helper(this->_begin);
}

template<class TypeKey, class TypeValue>
typename Tree<TypeKey, TypeValue>::Iterator Tree<TypeKey, TypeValue>::begin() {
    Node* temp = this->get_begin();
    while (temp->left != nullptr)
        temp = temp->left;

    return Iterator(temp, this->_begin);
}

template<class TypeKey, class TypeValue>
typename Tree<TypeKey, TypeValue>::Iterator Tree<TypeKey, TypeValue>::end() {
    return Iterator(nullptr, this->_begin);
}

template<class TypeKey, class TypeValue>
typename Tree<TypeKey, TypeValue>::Inverse_iterator Tree<TypeKey, TypeValue>::rBegin() {
    Node* temp = this->_begin;
    while (temp->right != nullptr)
        temp = temp->right;

    return Inverse_iterator(temp, this->_begin);
}

template<class TypeKey, class TypeValue>
typename Tree<TypeKey, TypeValue>::Inverse_iterator Tree<TypeKey, TypeValue>::rEnd() {
    return Inverse_iterator(nullptr, this->_begin);
}

template<class TypeKey, class TypeValue>
size_t Tree<TypeKey, TypeValue>::getLastOperationIterations() const {
    return this->operations;
}

template<class TypeKey, class TypeValue>
void Tree<TypeKey, TypeValue>::print_helper(Node* n) const {
    if (n != nullptr) {
        std::cout << n->key << " " << n->value << std::endl;
        print_helper(n->left);
        print_helper(n->right);
    }
}

template<class TypeKey, class TypeValue>
void Tree<TypeKey, TypeValue>::print() const {
    //print_helper(this->_begin);

    print_elegant(this->_begin, 1);
}

template<class TypeKey, class TypeValue>
Tree<TypeKey, TypeValue>::
Iterator_base::Iterator_base() {
    this->current = nullptr;
    this->parent = nullptr;
}

template<class TypeKey, class TypeValue>
Tree<TypeKey, TypeValue>::
Iterator_base::Iterator_base(Node* node, Node* parent) {
    this->current = node;
    this->parent = parent;
}

template<class TypeKey, class TypeValue>
Tree<TypeKey, TypeValue>::
Iterator_base::Iterator_base(Tree<TypeKey, TypeValue>* tr) {
    this->current = tr->_begin;
    this->parent = tr->_begin;
}

template<class TypeKey, class TypeValue>
typename Tree<TypeKey, TypeValue>::Node* Tree<TypeKey, TypeValue>::
Iterator_base::operator*() const {
    return this->current;
}

template<class TypeKey, class TypeValue>
bool Tree<TypeKey, TypeValue>::
Iterator_base::operator==(const Iterator_base& anotherIt) const {
    if (anotherIt.current == nullptr) {
        if (this->current == nullptr)
            return true;
        else
            return false;
    }
    else {
        if (this->current == nullptr)
            return false;
        else
            return this->current->getKey() == anotherIt.current->getKey();
    }
}

template<class TypeKey, class TypeValue>
bool Tree<TypeKey, TypeValue>::
Iterator_base::operator!=(const Iterator_base& anotherIt) const {
    return !(*this == anotherIt);
}

template<class TypeKey, class TypeValue>
typename Tree<TypeKey, TypeValue>::Iterator_base& Tree<TypeKey, TypeValue>::
Iterator_base::operator++(int) {
    for (size_t i = 0; i < nodes.size(); i++) {
        if (nodes[i]->getKey() == this->current->getKey()) {
            if (i == nodes.size() - 1)
                this->current = nullptr;
            else
                this->current = nodes[i + 1];
            break;
        }
    }

    return *this;
}

template<class TypeKey, class TypeValue>
typename Tree<TypeKey, TypeValue>::Iterator_base& Tree<TypeKey, TypeValue>::
Iterator_base::operator--(int) {
    for (size_t i = nodes.size(); i >= 0; i--) {
        if (nodes[i]->getKey() == this->current->getKey()) {
            if (i == 0)
                this->current = nullptr;
            else
                this->current = nodes[i - 1];
            break;
        }
    }

    return *this;
}

template<class TypeKey, class TypeValue>
Tree<TypeKey, TypeValue>::Iterator::Iterator(Node* node, Node* parent) :
    Iterator_base(node, parent) {
    fillNodes(parent);
}

template<class TypeKey, class TypeValue>
Tree<TypeKey, TypeValue>::Iterator::Iterator(Tree<TypeKey, TypeValue>* tr) :
    Iterator_base(tr) {
    fillNodes(tr->_begin);
}

template<class TypeKey, class TypeValue>
void Tree<TypeKey, TypeValue>::Iterator::fillNodes(Node* n) {
    if (n != nullptr) {
        fillNodes(n->right);
        this->nodes.push_back(n);
        fillNodes(n->left);
    }
}

template<class TypeKey, class TypeValue>
Tree<TypeKey, TypeValue>::Inverse_iterator::Inverse_iterator(Node* node, Node* parent) :
    Iterator_base(node, parent) {
    fillNodes(parent);
}

template<class TypeKey, class TypeValue>
Tree<TypeKey, TypeValue>::Inverse_iterator::Inverse_iterator(Tree<TypeKey, TypeValue>* tr) :
    Iterator_base(tr) {
    fillNodes(tr->_begin);
}

template<class TypeKey, class TypeValue>
void Tree<TypeKey, TypeValue>::Inverse_iterator::fillNodes(Node* n) {
    if (n != nullptr) {
        fillNodes(n->right);
        this->nodes.push_back(n);
        fillNodes(n->left);
    }
}

template<class TypeKey, class TypeValue>
void Tree<TypeKey, TypeValue>::print_elegant(Node* node, int indent) const {
    if (node != nullptr) {
        if (node->right)
            print_elegant(node->right, indent + 3);

        if (indent)
            std::cout << std::setw(indent) << ' ';

        if (node->right)
            std::cout << "/\n" << std::setw(indent) << ' ';

        std::cout << node->key << "\n ";

        if (node->left) {
            std::cout << std::setw(indent) << ' ' << "\\ \n";
            print_elegant(node->left, indent + 3);
        }
    }
}

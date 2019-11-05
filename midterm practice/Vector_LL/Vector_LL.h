//
// Created by dbottch1 on 11/5/19.
//

#ifndef VECTOR_LL_VECTOR_LL_H
#define VECTOR_LL_VECTOR_LL_H
#include <iostream>
#include <string>
#include <initializer_list>

template <typename T>
class Vector {
public:
    /*Rule of Three*/
    struct Node {
    public:
        Node() : next(nullptr), prev(nullptr) {

        }
        Node(const Node& n) : next(n.next), prev(n.prev) {

        }
        Node& operator=(const Node& n) {
            next = n.next;
            prev = n.prev;
            return *this;
        }
        Node(Node* next, Node* prev) : next(next), prev(prev) {}
        ~Node() {

        }
        Node* next;
        Node* prev;
    };
    struct DataNode : public Node {
    public:
        DataNode(const T& val) : Node(),  data(val) {}
        DataNode(const DataNode& n) : Node::next(n.next), Node::prev(n.prev), data(n.data) {}
        T data;
    };

    Vector() : _size(0), _capacity(16), cap_idx(2), sentinel(new Node()) {
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
    }
    Vector(const Vector& v) {}
    Vector& operator=(const Vector& v) {
        clear();
        for(auto i = v.sentinel->next; i != v.sentinel; i = i->next) {
            push_back(static_cast<DataNode*>(i)->data);
        }
        return *this;
    }
    Vector& operator+(const Vector& v) {
        for(auto i = v.sentinel->next; i != v.sentinel; i = i->next) {
            push_back(static_cast<DataNode*>(i)->data);
        }
        return *this;
    }
    ~Vector() {
        auto curr = sentinel;
        while(curr->next != sentinel) {
            auto next = curr;
            curr = curr->next;
            //std::cout << *static_cast<typename SkipList::DataNode*>(i);
            delete next;
        }
        delete curr;
    }

    void clear() {
        auto curr = sentinel;
        while(curr->next != sentinel) {
            auto next = curr;
            curr = curr->next;
            //std::cout << *static_cast<typename SkipList::DataNode*>(i);
            delete next;
        }
    }
    //insert before position
    void insert(Node* pos, const T& val) {
        auto n = new DataNode(val); //B
        //std::cout << "Data: " << static_cast<DataNode*>(n)->data << std::endl;
        //A->C => A->B->C where C is pos
        n->prev = pos->prev; //B->prev = A
        n->next = pos; //B->next = C
        pos->prev->next = n;//C->prev = A->next = B
        pos->prev = n; //C->prev = B
        //std::cout << "done with insert" << std::endl;
    }
    void push_front(const T& val) {
        insert(sentinel->next, val);
    }
    void push_back(const T& val) {
        insert(sentinel, val);
    }
    T& pop_front() {
        remove(head());
    }
    T& pop_back() {
        remove(tail()->prev);
    }
    void remove(const T& val) {
        Node* i;
        for(i = sentinel->next; i != sentinel; i = i->next) {
            if(dynamic_cast<DataNode*>(i)->val == val) break;
        }
        remove(i);
    }

    void
    print_forward() {
        for (Node *n = sentinel->next; n != sentinel; n = n->next) {
            std::cout << static_cast<DataNode *>(n)->data << " ";
        }
        std::cout << std::endl;
    }
    void remove(Node* pos) {
        //A->C => A->B->C where C is pos
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        pos->prev = pos->next = nullptr;
        delete pos;
    }
    Node* head() {
        return sentinel->next;
    }
    Node* tail() {
        return sentinel;
    }

private:
    size_t _size;
    size_t _capacity;
    size_t cap_idx;
    Node* sentinel;

};
#endif //VECTOR_LL_VECTOR_LL_H

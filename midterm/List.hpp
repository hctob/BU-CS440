#ifndef CS540_LIST_HPP
#define CS540_LIST_HPP



#include <assert.h>
#include <iostream>
#include <string>
/*
Hints:
1) Doubly linked is easier to implement.
2) Delegate logic to helper classes if possible
*/

template <typename T>
class List {
    private:
 	// Add necessary helper classes
    struct Node {
    public:
        Node() : next(nullptr), prev(nullptr) {

        }
        Node(const Node& n) : next(n.next), prev(n.prev) {

        }
        Node(Node* next, Node* prev) : next(next), prev(prev) {}
        Node& operator=(const Node& n) {
            next = n.next;
            prev = n.prev;
            return *this;
        }
        //Node(Node* next, Node* prev) : next(next), prev(prev) {}
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
        friend std::ostream& operator<<(std::ostream &os, const DataNode& dataNode) {
            os << dataNode.data;
            return os;
        }
    };

    size_t _size;
    size_t _capacity;
    size_t cap_idx;

    public:
        class Iterator {
        public:
            Node* pos;
            Iterator() = delete;
            Iterator(Node* n) : pos(n) {}
            Iterator(const Iterator& it) : pos(it.pos) {}
            Iterator& operator=(const Iterator& it) {
                pos = it.pos;
                return *this;
            }
            ~Iterator() = default;
            Iterator& operator++() {
                this->pos = this->pos->next;
                return *this;
            }
            Iterator operator++(int) {
                auto temp = *this;
                this->operator++();
                //this->pos = this->pos->next;
                return temp;
            }
            Iterator& operator--() {
                this->pos = this->pos->prev;
                return *this;
            }
            Iterator operator--(int) {
                auto temp = *this;
                this->operator--();
                //this->pos = this->pos->next;
                return temp;
            }
            T &operator*() const {
                return static_cast<DataNode*>(this->pos)->data; //returns dereferenced KV - key value pair
            }
            T *operator->() const {
                return &static_cast<DataNode*>(this->pos)->data; //returns dereferenced KV - key value pair
            }
            bool operator==(const Iterator& i) {
                return (this->pos == i.pos) ? true : false;
            }
            bool operator!=(const Iterator& i) {
                return (this->pos == i.pos) ? !true : !false;
            }
// Add the necessary methods here for loop
        };
    public:
    Node* sentinel;
        //List() : sentinel{sentinel, sentinel}  {}
        List() {
            sentinel = new Node();
            sentinel->next = sentinel;
            sentinel->prev = sentinel;
        }
        ~List() {
            auto curr = sentinel;
            while(curr->next != sentinel) {
                auto next = curr;
                curr = curr->next;
                //std::cout << *static_cast<typename SkipList::DataNode*>(i);
                delete next;
            }
            delete curr;
        }
        auto begin() { return Iterator{sentinel->next}; }
        auto end() { return Iterator{sentinel}; }
        auto insert(const Iterator &it, const T &val) {
          //Implement this
            //printf("beginning insert\n");
            auto n = new DataNode(val); //B
            //std::cout << "Data: " << static_cast<DataNode*>(n)->data << std::endl;
            //A->C => A->B->C where C is pos
            //printf("pointer arithmetic\n");
            n->prev = it.pos->prev; //B->prev = A
            n->next = it.pos; //B->next = C
            it.pos->prev->next = n;//C->prev = A->next = B
            it.pos->prev = n; //C->prev = B
            _size++;
            return Iterator(n);
        }
        void push_front(const T& val) {
            insert(sentinel->next, val);
        }
        void push_back(const T& val) {
            insert(sentinel, val);
        }
        Node* head() {
            return sentinel->next;
        }
        Node* tail() {
            return sentinel;
        }
        //Node* begin() {}
        T& pop_front() {
            remove(head());
        }
        T& pop_back() {
            remove(tail()->prev);
        }
        void
        print_forward() {
            for (Node *n = sentinel->next; n != sentinel; n = n->next) {
                std::cout << static_cast<DataNode *>(n)->data << " ";
            }
            std::cout << std::endl;
        }
        void erase(const Iterator &it) {
            assert(it.pos != sentinel);
            it.pos->prev->next = it.pos->next;
            it.pos->next->prev = it.pos->prev;
            it.pos->prev = it.pos->next = nullptr;
            delete it.pos;
          //Implement this
        }
};

#endif

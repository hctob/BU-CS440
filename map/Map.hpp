#ifndef BOTTCHER_MAP_H
#define BOTTCHER_MAP_H
#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <limits>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <ctime>
#include <cassert>
#include <initializer_list>

/*http://ticki.github.io/blog/skip-lists-done-right/
 * https://en.wikipedia.org/wiki/Skip_list
*/

namespace cs540 {
    /* begin class Map */
    template <typename Key_T, typename Mapped_T> class Map {
        using ValueType = std::pair<const Key_T, Mapped_T>;
        //using KeyType = const Key_T;
        //using MappedType = Mapped_T;
        //typedef typename std::pair<const Key_T, Mapped_T> ValueType;
        /* begin class SkipList */
        class SkipList {
            //using ValueType = std::pair<const Key_T, Mapped_T>;
        public:
            struct Node;
            struct DataNode;
            SkipList() : max_level(128), _probability(0.78), current_level(0), size(0), head(new Node(128)), tail(new Node(128)) {

                //printf("Constructor called\n");
                srand(13905);

                std::fill(head->forward.begin(), head->forward.end(), tail);
                std::fill(tail->forward.begin(), tail->forward.end(), head);
                std::fill(tail->forward.begin(), tail->forward.end(), head);

            }

            SkipList(const size_t &MAX_LEVEL, double prob) : max_level(MAX_LEVEL), _probability(prob), current_level(0),
                                                             size(0), head(new Node(MAX_LEVEL)), tail(new Node(MAX_LEVEL)) {
                //printf("Constructor called\n");
                srand(13905);

                std::fill(head->forward.begin(), head->forward.end(), tail);
		        //std::fill(tail->forward.begin(), tail->forward.end(), head);
                //std::fill(tail->forward.begin(), tail->forward.end(), head);
            }
	    SkipList(const SkipList &other) = default;

            ~SkipList() {
                //printf("SkipList deconstructor beginning: \n");
                auto i = head;
                //std::cout << *static_cast<typename SkipList::DataNode*>(i);
                while(i->forward[0] != tail) {
                    auto next = i;
                    i = i->forward[0];
                    //std::cout << *static_cast<typename SkipList::DataNode*>(i);
                    delete next;
                }
                delete i;
                delete tail;

            }


            /*working*/
            DataNode* find(Key_T k) {
                Node* current = head;
                //printf("SkipList find(): testing 1\n");
                for (auto i = max_level; i-- > 0;) {
                    //std::cout << "iteration " << i << std::endl;
                    while ((current->forward[i] != tail) && dynamic_cast<typename SkipList::DataNode*>(current->forward[i])->getKV().first < k) {
                        current = current->forward[i];
                    }
                }
                //printf("hello there\n");
                //if(current->forward[0] != nullptr) //#remember to get back to this
                current = current->forward[0];
                //printf("%s\n", current->kv.first);
                //printf("hello there\n");
                if (current == nullptr || current == tail || current == head || !(static_cast<typename SkipList::DataNode*>(current)->getKV().first == k)) {
                    //if (static_cast<typename SkipList::DataNode*>(current)->getKV().first == k
                    //std::cout << *current << " found for key " << k << std::endl;
                    //return static_cast<DataNode*>(current);
                    return static_cast<DataNode*>(nullptr);
                } else {
                    //std::cout << "Could not find any node associated with " << k << std::endl;
                    //std::cout << *tail << std::endl;
                    //return static_cast<DataNode*>(nullptr);
                    return static_cast<DataNode*>(current);
                }
            }

            /*working*/
            DataNode* insert(const ValueType &kvpair) {

                std::vector<Node *> predecessors = traverse_predecessors(kvpair.first);
		              DataNode* n;
                      Node* next = predecessors[0]->forward[0];

			unsigned int new_level = assign_level();
			if(new_level > current_level) {
				for(auto i = current_level + 1; i <= new_level; i++) {
					predecessors[i] = head;
				}
				current_level = new_level;

			}
			n = new DataNode(DataNode(ValueType(kvpair.first, kvpair.second), new_level)); //copy constructor call (but uses value constructor explicit temp)
				//DataNode* n =
				//DataNode* n = DataNode{n};
				for (auto i = 0; i < new_level; i++) {
				    //linking of forward and backward node pointers
				    n->forward[i] = predecessors[i]->forward[i];
				    n->backward[i] = predecessors[i];
				    predecessors[i]->forward[i]->backward[i] = n;
				    predecessors[i]->forward[i] = n;
				}
				//std::cout << "Successfully inserted at level " << new_level << std::endl;
				//std::cout << "Successfully inserted " << kvpair.second <<  " at level " << new_level << ", " << *n << std::endl;
				size++;
		//}
                //DataNode* n = new DataNode(std::make_pair(kvpair.first, kvpair.second), new_level);
                //DataNode* n = DataNode(new DataNode(std::make_pair(kvpair.first, kvpair.second), new_level));
		return n; //return DataNode to avoid double find in Map::insert()
            }

            void erase(const Key_T &k) {
                std::vector<Node *> predecessors = traverse_predecessors(k);
                auto next = predecessors[0]->forward[0];
                if(static_cast<typename SkipList::DataNode*>(next)->getKV().first == k) {
                    for(auto i = 0; i < predecessors.size(); ++i) {
                        if(predecessors[i]->forward[i] != next) {
                            break;
                        }
                        predecessors[i]->forward[i] = next->forward[i];
			next->forward[i]->backward[i] = predecessors[i];
                    }
                    delete next; //clear old value
                }
            }


            size_t getSize() const {
                return size;
            }

            bool empty() const {
                return (size == 0);
            }

        public:
            struct Node {
                //Key_T key;
                //Mapped_T value;
                //ValueType kv; //keyvalue pair <key, value>
            public:
                size_t level; //level randomly assigned during creation, [1, max_level]
                auto getLevel() const { return level; }
                bool containsData;
                std::vector<Node *> forward; //vector of "layers" -
		std::vector<Node *> backward;
                Node(const unsigned int &level, bool dataNode = false) : forward(level, nullptr), backward(level, nullptr), level(level), containsData(dataNode) {
                    //forward.reserve(10000000); //lol, ken your performance test is ridiculous
                }
                Node(const Node &n) : forward(n.level, nullptr), backward(n.level, nullptr), level(n.level), containsData(n.containsData) {} //forward.reserve(10000000);
                //Node(const Key_T &key, const unsigned int &level) : kv(std::make_pair(key, 0)), forward(level, nullptr), level(level) {}
                //Node(const Mapped_T &val, const unsigned int &level) : kv(std::make_pair(0, val)), forward(level, nullptr), level(level) {}
                /*Node(const Key_T &key, Mapped_T &val, const unsigned int &level) : kv(std::make_pair(key, val)),
                                                                                   forward(level, nullptr), level(level) {
                }*/
                //~Node(){}
                virtual ~Node() = default;

            };

            struct DataNode : public Node {
                ValueType kv;
                DataNode(const ValueType &v, const unsigned int &level) : Node(level, true), kv(v) {}
                DataNode(const DataNode &n) : Node(n.getLevel(), true), kv(n.kv) {}
                DataNode &operator=(const DataNode &other) = default;
                ValueType &getKV() {return this->kv;} //getter and setter for key-value
                //ValueType *getKVP() {return &kv;}
                //ValueType &getKV() const {return this->kv;} //getter and setter for key-value

                friend std::ostream &operator<<(std::ostream &os, const DataNode &node) {
                    if(&node == nullptr) {
                        os << "Node not found." << std::endl;
                        return os;
                    }
                    if(node.containsData == false) {
                        os << "[ sentinel node, lvl=" << node.level << " ]" << std::endl;
                    }
                    //else
                    //os << "key [" << node.kv.first << "]" << std::endl;
                        //os << "[" << node.kv.first << "," << "some value"
                        //os << "[ <" << node.kv.first << "," << node.kv.second << ">, lvl=" << node.level << " ]";*/
                    return os;
                }
            };

            Node *head;
            Node *tail;
            double _probability;
            const size_t max_level;
            size_t current_level;
            size_t size;

            /*
            Returns a pointer to a new node on the heap
            */
            Node* createDataNode(Key_T key, Mapped_T val, size_t level) {
                //std::cout << "test 2-1" << std::endl;
                //Node *ret = new Node(key, val, level);
                //std::cout << "test 2-2" << std::endl;
                return new DataNode(std::make_pair(key, val), level);
            }
            /*https://codereview.stackexchange.com/questions/116345/skip-list-implementation*/
            std::vector<Node*> traverse_predecessors(Key_T k) const {
                //printf("\ntraverse\n");
                //std::cout << current->level;
                //Node* new_level[max_level + 1]; //max_level + 1?
                std::vector<Node *> new_level(max_level, nullptr);
                Node *current = head;
                //printf("will probably fail after here\n");
                for (size_t i = max_level; i-- > 0;) {
                    //printf("iteration %d\n", i);
                    //printf("will probably fail after here\n");
                    while (current->forward[i] && current->forward[i]->containsData && reinterpret_cast<typename SkipList::DataNode*>(current->forward[i])->getKV().first < k) {
                        //if(!current->forward[i]->containsData) break; //todo: might need to do continue
                        current = current->forward[i];
                    }
                    //printf("out of while loop\n");
                    new_level[i] = current;
                }
                //printf("didn't fail!\n");
                return new_level;
            }

            /*
            Flip coin until 'tails' is encountered and return value of heads. heads/heads/tails = 3
            */
            unsigned int assign_level() const {
                unsigned int level = 1;
                //random coin flip functionality for increasing level
                while (((double) std::rand() / RAND_MAX) < _probability && level + 1 != max_level) {
                    level += 1;
                }
                //std::cout << "Level generated " << level << "\n";
                return level;
            }
        };
        SkipList* internal_list;
        /* end class SkipList */
    public:
        class Iterator;
        class ConstIterator;
        class ReverseIterator;
        /* begin classes Iterator, ConstIteator, and ReverseIterator */
        class Iterator {
            //does not permit default construction
        public:
            typename SkipList::Node* pos;
            //size_t idx;
            //ValueType keyval;
            Iterator() = delete;
            Iterator(const Iterator &) = default;
            Iterator(typename SkipList::Node* n) : pos(n) {
                //printf("Constructed Iterator from SkipList node");
                //pos = n;
                //keyval = n->kv;
                //printf("Constructed Iterator from SkipList node\n");
                //keyval = std::make_pair(n->kv.first, n->kv.second);
            }
            friend std::ostream &operator<<(std::ostream &os, const Iterator &it) {
        	    /*if(node == nullptr) {
        		os << "<invalid,invalid>" << std::endl;
        		return os;
        	    }*/
                    os << "Iterator: node=" << *(static_cast<typename SkipList::DataNode*>(it.pos));
                    return os;
            }
            ~Iterator() = default;
            Iterator &operator=(const Iterator &n) {
                pos = n.pos;
                return *this;
            }
            //https://stackoverflow.com/questions/1303899/performance-difference-between-iterator-and-iterator
            Iterator &operator++() { //preincrement
                if(this->pos->containsData) {
                    //printf("incremented\n");
                    this->pos = this->pos->forward[0];
                }
                /*else if(!pos->containsData) {
                    printf("reached sentinel\n");
                    //this->pos = cs540::Map::end();
                }*/
                return *this;
            }
            //https://stackoverflow.com/questions/1303899/performance-difference-between-iterator-and-iterator
            Iterator operator++(int) { //postincrement
                //Iterator& temp(*this);
		auto temp = *this;
                this->operator++();
                return temp;
            }
            Iterator &operator--() {
		this->pos = this->pos->backward[0];
		return *this;
	    } //preincrement
            Iterator operator--(int) {
		//Iterator& temp(*this);
		auto temp = *this;
                this->operator--();
                return temp;
	    } //

            ValueType &operator*() const {
                return static_cast<typename SkipList::DataNode*>(this->pos)->getKV(); //returns dereferenced KV - key value pair
            }
            ValueType *operator->() const {
                return (&static_cast<typename SkipList::DataNode*>(this->pos)->getKV()); //returns KVP - key value pointer
            }
            bool operator==(const Iterator &other) {

                return (this->pos == other.pos) ? true : false;
            }
            bool operator!=(const Iterator &other) {

                return !(*this == other);
            }

            bool operator==(const ConstIterator &other) {
                //if(this->pos != nullptr && other->pos == nullptr) return false;

                return (this->pos == other.pos) ? true : false;
            }
            bool operator!=(const ConstIterator &other) {
                //if(this->pos != nullptr && other->pos == nullptr) return true;

                return (this->pos == other.pos) ? !true : !false;
            }
        private:
        };
        class ConstIterator {
            //does not permit default construction
        public:
            typename SkipList::Node* pos;
            //ValueType keyval;
            ConstIterator() = delete;
            ConstIterator(const ConstIterator &) = default;
            ConstIterator(const Iterator &n) : pos(n.pos) {

            }
            ~ConstIterator() = default;
            ConstIterator(typename SkipList::Node* n) {
                //printf("Constructed Iterator from SkipList node");
                pos = n;
                //keyval = n->kv;
                //printf("Constructed Iterator from SkipList node\n");
                //keyval = std::make_pair(n->kv.first, n->kv.second);
            }
            friend std::ostream &operator<<(std::ostream &os, const ConstIterator &it) {
                /*if(node == nullptr) {
                os << "<invalid,invalid>" << std::endl;
                return os;
                }*/
                os << "Iterator: node=[" << *(static_cast<typename SkipList::DataNode*>(it.pos)) << "]" <<  std::endl;
                return os;
            }
            ConstIterator &operator=(const ConstIterator &n) {
                this->pos = n.pos;
                return *this;
            }
            ConstIterator &operator++() {
                if(this->pos->containsData) {
                    //printf("incremented\n");
                    this->pos = this->pos->forward[0];
                }
                /*else if(!pos->containsData) {
                    printf("reached sentinel\n");
                    //this->pos = cs540::Map::end();
                }*/
                return *this;
            } //preincrement
            ConstIterator operator++(int) {
                //Iterator& temp(*this);
		auto temp = *this;
                this->operator++();
                return temp;
            } //postincrement*/
            ConstIterator &operator--() {
		this->pos = this->pos->backward[0];
		return *this;
	    }//preincrement
            ConstIterator operator--(int) {
		//Iterator& temp(*this);
		auto temp = *this;
                this->operator--();
                return temp;
	    } //postincrement
            const ValueType &operator*() const {
                return (static_cast<typename SkipList::DataNode*>(this->pos)->getKV());
                //return (static_cast<typename SkipList::DataNode*>(this->pos)->getKV());
            }
            const ValueType *operator->() const {
                //return (static_cast<typename SkipList::DataNode*>(this->pos)->getKVP());
		return (&static_cast<typename SkipList::DataNode*>(this->pos)->getKV());
            }

            bool operator==(const ConstIterator &other) {
                //if(this->pos != nullptr && other->pos == nullptr) return false;

                return (this->pos == other.pos) ? true : false;
            }
            bool operator!=(const ConstIterator &other) {
                //if(this->pos != nullptr && other->pos == nullptr) return false;

                return (this->pos == other.pos) ? !true : !false;
            }

            bool operator==(const Iterator &other) {
                //if(this->pos != nullptr && other->pos == nullptr) return false;

                return (this->pos == other.pos) ? true : false;
            }
            bool operator!=(const Iterator &other) {
                //if(this->pos != nullptr && other->pos == nullptr) return false;

                return (this->pos == other.pos) ? !true : !false;
            }
        private:
        };
        class ReverseIterator {
	public:
            //does not permit default construction
	    typename SkipList::Node* pos;
	    ReverseIterator() = delete;
            ReverseIterator(const ReverseIterator &) = default;
            //ConstIterator(const ConstIterator &);
            ~ReverseIterator() = default;
            ReverseIterator(typename SkipList::Node* n) {
                //printf("Constructed Iterator from SkipList node");
                pos = n;
                //keyval = n->kv;
                //printf("Constructed Iterator from SkipList node\n");
                //keyval = std::make_pair(n->kv.first, n->kv.second);
            }
	    ReverseIterator& operator=(const ReverseIterator& n) {
		this->pos = n.pos;
                return *this;
	    }
	    ReverseIterator& operator++() {
		//Iterator::operator--();
		this->pos = pos->backward[0];
		return *this;
	    }
	    ReverseIterator operator++(int) {
		//Iterator::operator--(int);
		auto iter = *this;
		this->operator++();
		return iter;
	    }
	    ReverseIterator& operator--() {
		//Iterator::operator++();
		this->pos = pos->forward[0];
		return *this;
	    }
	    ReverseIterator operator--(int) {
		//Iterator::operator++();
		auto iter = *this;
		this->operator--();
		return iter;
	    }
	    ValueType &operator*() const {
                return (static_cast<typename SkipList::DataNode*>(this->pos)->getKV());
                //return (static_cast<typename SkipList::DataNode*>(this->pos)->getKV());
            }
            ValueType *operator->() const {
                return (&static_cast<typename SkipList::DataNode*>(this->pos)->getKV());
            }
	    bool operator==(const ReverseIterator &other) {
                //if(this->pos != nullptr && other->pos == nullptr) return false;

                return (this->pos == other.pos) ? true : false;
            }
            bool operator!=(const ReverseIterator &other) {
                //if(this->pos != nullptr && other->pos == nullptr) return false;

                return (this->pos == other.pos) ? !true : !false;
            }
	    friend std::ostream &operator<<(std::ostream &os, const ReverseIterator &it) {
                /*if(node == nullptr) {
                os << "<invalid,invalid>" << std::endl;
                return os;
                }*/
                os << "Iterator: node=[" << *(static_cast<typename SkipList::DataNode*>(it.pos)) << "]" <<  std::endl;
                return os;
            }
        };
        /*Begin Class Map */

        /* Constructor and Assignment Operator */
        Map() : internal_list(new SkipList()) {
            //printf("map ctor called\n");
            //std::cout << "test empty: " << std::boolalpha << internal_list->empty() << std::endl;
            //std::cout << *internal_list->find(8) << std::endl;
        }
        Map(const Map &m) : internal_list(new SkipList()) { //todo: generates what seems like a million and a half valgrind warnings, so probably find a better way
	    //this->internal_list->head->level = internal_list;
	    //this->internal_list->tail = m.getInternalList()->tail;
            for(auto i = m.begin(); i != m.end(); ++i) {
		    insertCopy(static_cast<typename SkipList::DataNode*>(i.pos));
	    }
	    this->internal_list->size = m.size();
        }

	void insertCopy(typename SkipList::DataNode* other) {
				typename SkipList::DataNode* n = new typename SkipList::DataNode(typename SkipList::DataNode(ValueType(other->getKV().first, other->getKV().second), other->level));
		//n = new DataNode(DataNode(ValueType(kvpair.first, kvpair.second), new_level)); //copy constructor call (but uses value constructor explicit temp)
				//DataNode* n =
				//DataNode* n = DataNode{n};
				for (auto i = 0; i < other->level; i++) {
				    //linking of forward and backward node pointers
				    n->forward[i] = this->getInternalList()->tail;
				    //printf();
				    n->backward[i] = this->getInternalList()->tail->backward[i];
				    this->getInternalList()->tail->forward[i] = n;
				    this->getInternalList()->tail->backward[i] = n;

				    //predecessors[i]->forward[i]->backward[i] = n;
				    //predecessors[i]->forward[i] = n;
				}
				delete n;
	}
        SkipList* getInternalList() const { return this->internal_list; }

        //https://en.cppreference.com/w/cpp/utility/initializer_list
        Map(std::initializer_list<ValueType> t) : internal_list(new SkipList()) {
             for(auto &pairs : t) {
                insert(pairs);
             }
            //internal_list.insert(t.begin());
        }
        Map &operator=(const Map &m) {
            //this->internal_list = m.internal_list;
            this->clear();
            for(auto i = m.begin(); i != m.end(); ++i) {
                this->insert(*i);
            }
            return *this;
        }
        ~Map() { delete internal_list; }
        /* Size */
        size_t size() const {
            return internal_list->size;
        }
        bool empty() const {
            return (size() == 0) ? true : false;
        }
        /* Iterators */
        Iterator begin() {
            return Iterator(internal_list->head->forward[0]);
        }
        Iterator end() {
            //printf("end\n");
            return Iterator(internal_list->tail); //not tail->forward[0]
        }
        ConstIterator begin() const {
            return ConstIterator(internal_list->head->forward[0]);
        }
        ConstIterator end() const {
            return ConstIterator(internal_list->tail);
        }
        ReverseIterator rbegin() {
	    return ReverseIterator(internal_list->tail->backward[0]); //last element before tail()
	}

        ReverseIterator rend() {
	    return ReverseIterator(internal_list->head);
	}
        /* Element Access */
        Iterator find(const Key_T &k) {
            //printf("testing: find()\n");
            typename SkipList::Node* node = internal_list->find(k);
            //printf("hehe\n");
            if(node == static_cast<typename SkipList::Node*>(internal_list->tail) || node == static_cast<typename SkipList::Node*>(nullptr)) {
                //printf("could not find node\n");
                return end();
            }
            else {
                //printf("found node, returning iterator\n");
                return Iterator(node);
            }
            //printf("found node\n");
        }

        ConstIterator find(const Key_T &k) const {
            typename SkipList::Node* node = internal_list->find(k);
            //printf("hehe\n");
            if(node == static_cast<typename SkipList::Node*>(internal_list->tail) || node == static_cast<typename SkipList::Node*>(nullptr)) {
                //printf("could not find node\n");
                return end();
            }
            else {
                //printf("found node, returning iterator\n");
                return ConstIterator(node);
            }
        }

        Mapped_T &at(const Key_T &k) {
                auto ret = find(k);
                if(ret == end()) {
                    throw std::out_of_range("Requested key does not exist in the map.");
                }
                else {
                        return static_cast<typename SkipList::DataNode*>(ret.pos)->getKV().second;
                }
        }
        const Mapped_T &at(const Key_T &key) const {
            auto ret = find(key);
            if(ret == end()) {
                throw std::out_of_range("Requested key does not exist in the map.");
            }
            else {
                return static_cast<typename SkipList::DataNode*>(ret.pos)->getKV().second;
            }
        }
        Mapped_T &operator[](const Key_T &k) {
            auto ret = find(k); //
            if(ret != end()) {
                return static_cast<typename SkipList::DataNode*>(ret.pos)->getKV().second;
            }
            else {
                ValueType v = ValueType(k, Mapped_T()); //value intialize new Mapped_T object
                //printf("hello\n");
                internal_list->insert(v); //insert internally to avoid calling find() once more
                return internal_list->find(k)->getKV().second; //return Mapped_T object after insertion
            }
        }
        /* Modifiers */
        std::pair<Iterator, bool> insert(const ValueType &v) {
            auto it = find(v.first);
            //printf("ye, find made it through");
            if(it == end()) {
                //printf("inserting into internal list\n");
                typename SkipList::DataNode* gb = internal_list->insert(v); //pointer to recently inserted node
                return std::make_pair(Iterator(gb), true);
            }
            else {
                return std::make_pair(it, false);
            }
        }
        void insert_skiplist(const ValueType& v) {
            internal_list->insert(v);
            //printf("\nSkiplist inserted into internally\n");
        }
        template <typename IT_T>
        void insert(IT_T range_beg, IT_T range_end) {
            for(IT_T i = range_beg; i != range_end; ++i) { //todo: might be < range_end
                insert(*i); //from Huanyi on piazza, thank you bro
            }
        }
        void erase(Iterator pos) {
            erase(static_cast<typename SkipList::DataNode*>(pos.pos)->getKV().first);
            std::cout << "Successfully erased!" << std::endl;
        }
        void erase(const Key_T &key) {
            auto ret = find(key);
            if(ret == end()) {
                throw std::out_of_range("Requested key does not exist in the map.");
            }
            else {
                internal_list->erase(key);
            }
        }
        void clear() {
            //for(auto it = begin(); it != end(); ++it) erase(it);
            delete internal_list;
            internal_list = new SkipList();
        }//remove all elements from the map
    };
    /* free comparison functions */
    /* Map */

    template <typename Key_T, typename Mapped_T> bool operator==(const Map<Key_T, Mapped_T> &m1, const Map<Key_T, Mapped_T> &m2) {
        if(m1.size() != m2.size()) return false;
        else {
                for(auto i1 = m1.begin(), i2 = m2.begin(); i1 != m1.end() && i2 != m2.end(); ++i1, ++i2) {
                        if(*i1 == *i2) continue;
                        else if(*i1 != *i2) {
                                return false;
                        }
                }
        }
        return true;
    }
    template <typename Key_T, typename Mapped_T> bool operator!=(const Map<Key_T, Mapped_T> &m1, const Map<Key_T, Mapped_T> &m2) {
        return !(m1 == m2);
    }
    template <typename Key_T, typename Mapped_T> bool operator<(const Map<Key_T, Mapped_T> &m1, const Map<Key_T, Mapped_T> &m2) {
	for(auto i = m1.begin(), k = m2.begin(); i != m1.end(); ++i, ++k) {
		if(*i < *k) return true; //if an element of map1 is less than an element of map2 at the same position, M1 < M2
		else if(*i == *k) {
			printf("same element at this position\n");
			continue;
		} //if elements are the same, continue
		//else if(*i > *k) return false;
	}
	//theoretically all elements would be equal at this point
	if(m1.size() < m2.size()) {
		return true;
	}
	return false;
    }
    /* Iterators */
    /*bool operator==(const Iterator &, const Iterator &);
    bool operator==(const ConstIterator &, const ConstIterator &);
    bool operator==(const Iterator &, const ConstIterator &);
    bool operator==(const ConstIterator &, const Iterator &);
    bool operator!=(const Iterator &, const Iterator &);
    bool operator!=(const ConstIterator &, const ConstIterator &);
    bool operator!=(const Iterator &, const ConstIterator &);
    bool operator!=(const ConstIterator &, const Iterator &);

    bool operator==(const ReverseIterator &, const ReverseIterator &);
    bool operator!=(const ReverseIterator &, const ReverseIterator &);*/
}
#endif

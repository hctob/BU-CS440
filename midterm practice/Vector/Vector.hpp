#ifndef BOTTCHER_VECTOR_H
#define BOTTCHER_VECTOR_H
#include <iostream>
#include <string>
#include <initializer_list>

template <typename T>
class Vector {
public:
	/*Rule of Three*/
	Vector() : _size(0), _capacity(16), cap_idx(2), internal_buf(new T[_capacity]) {
	}
	Vector(const Vector& v) : _size(v.size()), _capacity(v.capacity()), internal_buf(new T[_capacity]) {
		internal_buf = new T[_size];
		for(auto i = 0; i < _size; i++) {
			internal_buf[i] = v.internal_buf[i];
		}
	}
	~Vector() {
		delete[] internal_buf;
	}
	Vector& operator=(const Vector& v) {
		delete[] internal_buf;
		_size = v.size();
		_capacity = v.capacity();
		internal_buf = new T[_size];
		for(auto i = 0; i < _size; i++) {
			internal_buf[i] = v.internal_buf[i];
		}
		return *this;
	}

	Vector& operator+(const Vector& v) {
		size_t new_size = this->size() + v.size();
		size_t g_cap = (capacity() > v.capacity()) ? capacity() : v.capacity();
		T* new_buf = new T[new_size];
		if(new_size > g_cap) {
			reserve(1 << cap_idx);
			cap_idx++;
		}
		for(auto i = 0; i < size(); i++) {
			new_buf[i] = internal_buf[i];
		}
		for(auto i = size(), k = 0; i < new_size; i++, k++) {
			new_buf[i] = v.internal_buf[k];
		}
		_size = new_size;
		delete[] internal_buf;
		internal_buf = new_buf;
		return *this;
	}
	/*Value Constructors*/
	Vector(size_t cap) : _size(0), _capacity(cap), cap_idx(2), internal_buf(new T[_capacity]) {}
	Vector(size_t size, size_t capacity) : _size(size), _capacity(capacity), cap_idx(2), internal_buf(new T[capacity]) {}
	Vector(std::initializer_list<T> t) : _size(0), _capacity(16), internal_buf(new T[_size]) {
		for(auto _t : t) 
			this->push_back(_t);
	}
	void print() {
		if(empty()) { printf("empty\n"); }
		else {
			for(auto i = 0; i < size(); i++)
				std::cout << i << ". " << internal_buf[i] << ", ";
			std::cout << std::endl;
		}
	}
	T& front() {
		return internal_buf[0];
	}
	T& back() {
		return internal_buf[0] + this->size();
	}
	T& operator[] (size_t idx) {
		if(idx > size()) {
			throw std::out_of_range("Requested index is > than size()");
		}
		return internal_buf[idx];
	}
	void push_back(const T& val) {
		if(_size >= capacity()) {
			reserve(1 << cap_idx);
			cap_idx++;
		}
		internal_buf[_size++] = val;
	}
	/*void push_front(const T& val) {
		
	}
	T& pop_front() {
		
	}
	T& pop_back() {
		_size--;
	}*/
	/*Size and Capacity*/
	size_t capacity() const {
		return _capacity;
	}
	
	size_t size() const {
		return _size;
	}
	bool empty() const {
		return (size() == 0);
	}
	void resize(size_t new_cap) {
		this->_size = new_cap; 
	}
	void reserve(size_t new_size) {
		T* new_buf = new T[new_size];
		for(auto i = 0; i < _size; i++) {
			new_buf[i] = internal_buf[i];
		}
		_capacity = new_size;
		delete[] internal_buf;
		internal_buf = new_buf;
	}
	
private:
	size_t _size;
	size_t _capacity;
	size_t cap_idx;
	T* internal_buf;
};
#endif

#ifndef BOTTCHER_SMART_PTR_H
#define BOTTCHER_SMART_PTR_H

class RC {
private:
	size_t count = 0;
public:
	void addRef() {
		count++;
	}
	size_t release() {
		--count;
		//return count;
	}
};

//https://www.codeproject.com/Articles/15351/Implementing-a-simple-smart-pointer-in-c

template <typename T>
class SmartPtr {
public:
	explicit SmartPtr(T* p = nullptr) : ptr(p), ref(0) {
		ref = new RC();
		ref->addRef();
	}
	SmartPtr(const SmartPtr& s) : ptr(s.ptr), ref(s.ref->count) {
		ref->addRef();
	}
	SmartPtr& operator=(const SmartPtr& s) {
		if(this != &s) {
			if(ref->release() == 0) {
				delete ptr;
				delete ref;
			}
			ptr = s.ptr;
			ref = s.ref;
			ref->addRef();
		}
		return *this;
	}
	~SmartPtr() {
		if(ref->release() == 0) {	
			delete ptr;
			delete ref;
		}
	}
	SmartPtr& operator*() {
		return *ptr;
	}
	SmartPtr* operator->() {
		return ptr;
	}
private:
	T* ptr; //actual pointer to T
	RC* ref;
	//size_t count = 0;
};
#endif

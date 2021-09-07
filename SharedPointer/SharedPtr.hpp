#ifndef BOTTCHER_ATOMIC_SHARED_PTR_H
#define BOTTCHER_ATOMIC_SHARED_PTR_H
#include <iostream>
#include <utility>
#include <cassert>
#include <atomic>
#include <mutex>
#include <pthread.h>
//https://www.codeproject.com/Articles/15351/Implementing-a-simple-smart-pointer-in-c

namespace cs540 {
    static pthread_mutex_t ref_owner_lock;
    //forward declerations
    template<typename T>
    class SharedPtr;
    template <typename T, typename U>
    SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp);
    template <typename T, typename U>
    SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp);

    template <typename T>
    void del(const void* ptr) {
        delete static_cast<const T*>(ptr);
        ptr = nullptr; //avoid garbagio
    }
	class SharedDestructor {
	public:
	    //U* p;
        virtual ~SharedDestructor() {}
        SharedDestructor() : ref_count{1} {}
        //void* ptr;

        void acquire() {
            pthread_mutex_lock(&cs540::ref_owner_lock);
            ++ref_count;
            pthread_mutex_unlock(&cs540::ref_owner_lock);
        }
        void release() {
            //predecrement in order to check if value is 0 for SmartPtr destructor
            pthread_mutex_lock(&cs540::ref_owner_lock);
            --ref_count;
            pthread_mutex_unlock(&cs540::ref_owner_lock);

            if(ref_count == 0) delete this;
        }
    private:
        //std::atomic<int> ref_count; //ATOMIC SIZE_T
        int ref_count;
	};

	//Begin class RefCount
    template <typename T>
    class RefCount : public SharedDestructor {
    public:
        //template <typename U>
        //SharedDestructor* destroy;
        //const void* ptr;
        T* ptr;
        //void (*destroy)(const void* _del_ptr);
        bool _default;

        //this->acquire(); //increase refCount to 1 on heap creation
        explicit RefCount(T* ptr) : SharedDestructor{}, ptr(ptr) {

        }


        ~RefCount() {
            //if(destroy)
            delete ptr;
        }
        //size_t ref_count = 0;
        //unsigned int ref_count = 0;
    };
	//Begin class SharedPtr
	template<typename T>
	class SharedPtr {
    private:
        //T* internal_ptr; //-Wreorder
	public:
		/* Constructors, Assignment Operators, and Destructor */
		SharedPtr() : internal_ptr(nullptr), ref_count(nullptr) { //todo: fix memory leaks/errors occuring here
            //printf("Default construction\n");
		}
		template <typename U>
		SharedPtr(U *p) : internal_ptr{p}, ref_count{new RefCount<U>(p)} {

		}
		//copy constructors
		SharedPtr(const SharedPtr &p) : internal_ptr{p.internal_ptr}, ref_count{p.ref_count} {
            if(p != nullptr) {
                ref_count->acquire();
            }
            //p_thread_mutex_unlock(&ref_owner_lock);
		}
		template <typename U>
		SharedPtr(const SharedPtr<U> &p) : internal_ptr{(T*)p.internal_ptr}, ref_count{p.ref_count} {

            if(p) {
                ref_count->acquire();
            }
            //p_thread_mutex_unlock(&ref_owner_lock);
            ////p_thread_mutex_unlock(&ref_owner_lock);
		}
		//rvalue reference constructors
		SharedPtr(SharedPtr &&p) : internal_ptr(std::move(p.internal_ptr)), ref_count(std::move(p.ref_count)) {
            //printf("Move construction\n");
            p.internal_ptr = nullptr;
            p.ref_count = nullptr;

		}
		template <typename U>
		SharedPtr(SharedPtr<U> &&p) {
            //printf("Move construction templated\n");
            internal_ptr = std::move((T*)static_pointer_cast(p));
            ref_count = std::move(p.ref_count);
            p.internal_ptr = nullptr;
            p.ref_count = nullptr;
		}
        SharedPtr& operator=(const SharedPtr &sp) {
            //printf("Copy assignment\n");
            if(*this == sp) return *this;
                //inline deconstructor logic
                //decrement counter of current object
                if(ref_count != nullptr) {
                    //delete internal_ptr;
                    //delete &//ref_count->destroy;
                    ref_count->release();
                }
                internal_ptr = sp.internal_ptr;
                ref_count = sp.ref_count;
                //p_thread_mutex_lock(&ref_owner_lock);
                if(ref_count != nullptr) {
                    ref_count->acquire(); //increment ref count after copying
                }
                //p_thread_mutex_unlock(&ref_owner_lock);
            return *this;
        }
        template <typename U>
        SharedPtr<T> &operator=(const SharedPtr<U> &sp) {
            //printf("Copy assignment with cast\n");
            if(sp == *this) return *this;
            else {
                if(ref_count != nullptr) {
                    //delete internal_ptr;
                    ref_count->release();
                    //delete ref_count;
                }
                internal_ptr = sp.internal_ptr;
                //ref_count = static_cast<RefCount<T> *>(sp.ref_count);
                ref_count = sp.ref_count;
                //ref_count->destroy = &del<U>;
                //p_thread_mutex_lock(&ref_owner_lock);
                if(ref_count)
                    ref_count->acquire(); //increment ref count after copying
                //p_thread_mutex_unlock(&ref_owner_lock);
            }
            return *this;
        }
		SharedPtr &operator=(SharedPtr &&sp) {
		    //printf("Move assignment\n");
		    //if()
            if(sp == *this) return *this;

            internal_ptr = std::move(sp.internal_ptr);
            ref_count = std::move(sp.ref_count);
            sp.internal_ptr = nullptr;
            sp.ref_count = nullptr;
			return *this;
		}
		template <typename U>
		SharedPtr &operator=(SharedPtr<U> &&sp) {
		    //////printf("Move assignment templated\n");
            if(*this == sp) return *this;
            /*if(ref_count != nullptr) {
                //delete internal_ptr;
                ref_count->release();
                //delete ref_count;
            }*/
            internal_ptr = std::move(sp.internal_ptr);
            ref_count = std::move(sp.ref_count);
            //ref_count->destroy = &del<U>;
            sp.ref_count = nullptr;
            sp.internal_ptr = nullptr;
			return *this;
		}
		~SharedPtr() {
            //this->internal_ptr = nullptr; //just to make sure it doesn't point to garbage
            if(ref_count) ref_count->release();
		}
		/* Modifiers */
		void reset() {
            //printf("Reset to nullptr\n");
            if(ref_count != nullptr) {
                ref_count->release();
                //internal_ptr = nullptr;
                //ref_count = nullptr;
                if(ref_count == 0) delete ref_count;
            } //increment ref count after copying
            ref_count = nullptr;
            //p_thread_mutex_unlock(&ref_owner_lock);
            internal_ptr = nullptr;
		}
		template <typename U>
		void reset(U *p) {
            //printf("Reset to address\n");
            if(ref_count != nullptr) {
                ref_count->release();
                //internal_ptr = nullptr;
                //ref_count = nullptr;
                if(ref_count == 0) delete ref_count;
            } //increment ref count after copying
            internal_ptr = p;
            //if(p != nullptr)
            ref_count = new RefCount<U>(p);
            //ref_count->destroy = &del<U>;
            //p_thread_mutex_unlock(&ref_owner_lock);
		}
		T* get() const {
			return &(*internal_ptr);
		}
		T& operator*() const {
			return *(internal_ptr);
		}
		T* operator->() const {
			return (internal_ptr);
		}
		explicit operator bool() const {
			return (internal_ptr != nullptr); //true if not null, false if nullptr
		}
        T* internal_ptr;
        SharedDestructor* ref_count;
	};
	//non-member (free) functions
	template<typename T1, typename T2>
	bool operator==(const SharedPtr<T1> &sp1, const SharedPtr<T2> &sp2) {
		return (sp1.get() == sp2.get());
		//return false;
	}
	template <typename T>
	bool operator==(const SharedPtr<T> &sp, std::nullptr_t np) {
		return (sp.get() == np); //compare internal pointer to null pointer
		//return false;
	}
	template <typename T>
	bool operator==(std::nullptr_t np, const SharedPtr<T> &sp) {
		return (np == sp); //compare actual smart pointer to null
		//return false;
	}
    template <typename T1, typename T2>
	bool operator!=(const SharedPtr<T1> &sp1, const SharedPtr<T2> &sp2) {
		return (sp1.get() != sp2.get());
		//return false;
	}
	template <typename T>
	bool operator!=(const SharedPtr<T> &sp, std::nullptr_t np) {
		return(sp.get() != np);
	}
	template <typename T>
	bool operator!=(std::nullptr_t np, const SharedPtr<T> &sp) {
		return(sp != np);
	}
	template <typename T, typename U>
	SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp) {
	    T* ret_ptr = static_cast<T*>(sp.get());
        SharedPtr<T> ret_SP = SharedPtr<T>(); //copy construct new SharedPtr from U*
        ret_SP.internal_ptr = ret_ptr;
        ret_SP.ref_count = sp.ref_count;
        //p_thread_mutex_lock(&ref_owner_lock);
        ret_SP.ref_count->acquire(); //todo: multithreading
        //p_thread_mutex_unlock(&ref_owner_lock);
        return ret_SP;
	}
	template <typename T, typename U>
	SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp) {
        T* ret_ptr = dynamic_cast<T*>(sp.get());
        SharedPtr<T> ret_SP = SharedPtr<T>(); //copy construct new SharedPtr from U*
        ret_SP.internal_ptr = ret_ptr;
        ret_SP.ref_count = sp.ref_count;
        //p_thread_mutex_lock(&ref_owner_lock);
        ret_SP.ref_count->acquire(); //todo: multithreading
        //p_thread_mutex_unlock(&ref_owner_lock);
        return ret_SP;
	}
}
#endif

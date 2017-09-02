#pragma once
//This MainWinP template class is to be used mainly for a main window that,
//when deleted, will clean up all its child windows 
//convenience added for implicit conversion to window type
//and dereferencing
//main convienience is handling destructor

//by choice, T will be type that pointer references to
//T shouldn't represent pointer type itself
//like, for an int smart_pointer here, T should be int, not int* 
template <class T>
class MainWinP {
private:
	T* pointer_holder = nullptr;
public:
	MainWinP() {

	}

	MainWinP(T* window_pointer) {
		pointer_holder = window_pointer;
	}

	operator T*() {
		return pointer_holder;
	}

	T* operator->() {
		return pointer_holder;
	}

	~MainWinP() {
		if (pointer_holder != nullptr) {
			delete pointer_holder;
			pointer_holder = nullptr;
		}
	}
};
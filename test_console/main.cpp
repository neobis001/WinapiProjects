#include <iostream>

using namespace std;

class Parent {
public:
	Parent() {

	}

	Parent(int a) {
		init();
	}

	void init() {
		initA();
		initB();
		initC();
	}

	virtual void initA() {
		cout << "in Parent::initA" << endl;
	}

	void initB() {
		cout << "in Parent::initB" << endl;
	}

	void initC() {
		cout << "in Parent::initC" << endl;
	}

	~Parent() {
		cout << "Parent D" << endl;
	}

};

class Middle : public Parent {
public:
	Middle() {

	}

	Middle(int a) {
		init();
	}

	virtual void initA() {
		cout << "in Middle::initA" << endl;
	}

	~Middle() {
		cout << "Middle D" << endl;
	}
};

class Child: public Middle{
protected:

public:
	Child(int a): Middle(a) {
	}

	virtual void initA() {
		cout << "in Child::initA" << endl;
	}

	~Child() {
		cout << "Child D" << endl;
	}

};

int main() {
	Child a(2);
	return 0;
}
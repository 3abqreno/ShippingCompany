#pragma once
#include <iostream>
using namespace std;
template <class T>
class Node
{
	T data;
	Node<T>* next;

public:
	Node(T D) {
		setData(D);
		next = NULL;
	}
	void setNext(Node<T>* N) {
		next = N;
	}
	Node <T>* getNext()const {

		return next;
	}
	T getData()const {
		return data;
	}
	void setData(T D) {
		data = D;
	
	}

};

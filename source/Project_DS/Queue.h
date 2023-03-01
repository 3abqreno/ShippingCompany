#pragma once
#include"Node.h"
template<class T>
class Queue {
	Node<T>* head;
	Node<T>* tail;
	int count;
public:
	Queue() {
		head = NULL;
		tail = NULL;
		count = 0;
	}
	void enqueue(T data) {
		Node<T>* newNode = new Node<T>(data);
		count++;
		if (!head) {

			head = newNode;
			tail = newNode;
		}
		else
		{
			tail->setNext(newNode);
			tail = newNode;
		}
	}
	
	T dequeue() {
		Node<T>* temp = head;
	
		if (head) {
			T data = head->getData();;
			if (head == tail)
			{
				head = NULL;
				tail = NULL;
				
			}
			else
			{ 
			head = head->getNext();
			}
			count--;
			delete temp;
			return data;
		}
		return NULL;
		
	
	}
	 
	T getFront()const {
		if (head)
			return head->getData();
		else
			return NULL;
	}

	bool isEmpty() {
		
		if (head)
			return 0;
		else
			return 1;
	}

	void PrintInfo()
	{
		if (isEmpty()) 
			return;

		
		Node<T>* temp = head;
		while (temp->getNext())
		{
			cout << *(temp->getData()) << ",";
			temp = temp->getNext();
		}
		cout << *(temp->getData());
	}

	int getCount()
	{
		return count;
	}
};

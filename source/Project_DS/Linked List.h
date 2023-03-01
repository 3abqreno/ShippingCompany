#pragma once
#include"Node.h"
template<class T>
class LinkedList {

	Node<T>* head;
	Node<T>* tail;
	int count;
public:
	LinkedList() {
		head = NULL;
		tail = NULL;
		count = 0;
	}

	void insertFront(T data) {
		Node<T>* newNode = new Node<T>(data);
		count++;
		if (!head)
		{
			head = newNode;
			tail = newNode;
		}
		else
		{
			newNode->setNext(head);
			head = newNode;
		}
	}

	void insertEnd(T data) {
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

	bool search(const int key, T& ptr)const {

		Node<T>* temp = head;
		if (temp)
		{
			while (temp)
			{
				if (*(temp->getData()) == key)
				{
					ptr = temp->getData();
					return 1;
					
				}
				temp = temp->getNext();
			}


		}
		return 0;

	}

	bool Delete(const int key, T& ptr) {
		Node<T>* temp;
		if (head)
		{
			
			temp = head;
			if (*(head->getData()) == key) {
				ptr = head->getData();
				head = head->getNext();
				count--;
				return 1;
			}
			while (temp->getNext())
			{
				if (*(temp->getNext()->getData()) == key)
				{
					if (temp->getNext() == tail)
					{
						tail = temp;
					}
					ptr = temp->getNext()->getData();
					Node<T>* temp_2 = temp->getNext();
					temp->setNext(temp->getNext()->getNext());
					count--;
					
					return 1;

				}
				temp = temp->getNext();
			}


		}
		return 0;
	}


	T deleteFront() {
		
		if (head) {
			T  ptr = head->getData();
			count--;
			if (head->getNext() == NULL) {
				delete head;
				head = NULL;
				tail = NULL;
			}
			else
			{
				Node<T>* temp = head;
				head = head->getNext();
				delete temp;
			}
			return ptr;
		}
		return NULL;
		

	}

	void print()const {
		Node<T>* temp = head;
		if (temp) {
			cout << *(temp->getData());
			temp = temp->getNext();
		}
		while (temp)
		{
			cout << "," << *(temp->getData());
			temp = temp->getNext();
		}
	

	}
	T getFront()const {
		if (head)
			return head->getData();
		else
			return nullptr;
 	}
	int getCount()
	{
		return count;
	}
};
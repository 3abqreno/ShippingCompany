#pragma once
#include <iostream>
#include"Cargo.h"
using namespace std;


template<class T>
class minHeap {
	T* arr;
	int count, size;


	int parent(int i) {
		return (i - 1) / 2;
	}
	int right(int i) {
		return 2 * i + 1;
	}

	int left(int i) {
		return 2 * i + 2;
	}

	void minHeapify(int i) {
		int l, r, smallest;
		r = right(i);
		l = left(i);
		if (l < count && *arr[l] < *arr[i]) {
			smallest = l;
		}
		else
		{
			smallest = i;
		}

		if (r < count && *arr[r] < *arr[smallest])
		{
			smallest = r;
		}

		if (smallest != i) {
			swap(arr[i], arr[smallest]);
			minHeapify(smallest);
		}
	}
public:
	minHeap() {
		arr = NULL;
		count = 0;
	}

	bool initialize(int num) {
		if (arr == NULL) {
			arr = new T[num];
			return 1;
		}
		return 0;
	
	}


	void print() {
		cout << "\n\n";
		for (int i = 0; i < count; i++)
		{
			cout << *arr[i] << endl;
		}

		cout << "\n\n";
	}

	T heapMin()const {
		if (count != 0)
			return arr[0];
		return nullptr;
	}

	T heapExtractMin( ) {
		if (count == 0)return NULL;
		T min = arr[0];
		arr[0] = arr[count - 1];
		count--;
		minHeapify(0);
		return min;


	}

	void insertValue(T key) {
		arr[count] = key;
		int placment = count;
		count++;
		while (placment > 0 && *arr[parent(placment)] > *arr[placment]) {
			swap(arr[placment], arr[parent(placment)]);
			placment = parent(placment);
		}
		
		

	}

	int getcount() {
		return count;
	}

	void print()const {

		if (count)
			cout << *arr[0];
		for (int i = 1; i < count; i++)
		{
			cout <<","<<*arr[i];
		}
	}

	bool isEmpty() {
		if (count == 0)
			return 1;
		else
			return 0;
	}

};

template <class T>
class pQueue {

	minHeap<T> pArray;//priority array

public:
	pQueue() {
	
	}

	pQueue(int num) {
		pArray.initialize(num);
	}

	bool initialize(int num) {
		
		return pArray.initialize(num);
	}

	void enqueue(T data) {
		pArray.insertValue(data);
	}


	T front() {
		return pArray.heapMin();
	}

	T dequeue() {
		
		return pArray.heapExtractMin();

	}

	int getCount() {

		return pArray.getcount();
	}

	void print()const {
		pArray.print();
	}

	bool isEmpty() {
		return pArray.isEmpty();
	}

};













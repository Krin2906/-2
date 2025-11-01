// Written by Manju Muralidharan on 10/19/25
// A quick & clean MinHeap implementation for managing indices with weights

#ifndef HEAP_H
#define HEAP_H

#include <iostream>
using namespace std;

struct MinHeap {
    int data[64];
    int size;

    // Default constructor nothing fancy
    MinHeap() {
        size = 0;
    }

    // Push new index (with its weight reference) into the heap
    void push(int index, int weights[]) {
        if (size >= 64) {
            cerr << "[warn] Heap full! Ignoring push for index " << index << endl;
            return;
        }
        data[size] = index;
        upheap(size, weights);
        size++;
    }

    // pop the smallest (root) element
    int pop(int weights[]) {
        if (size == 0) {
            //exception here???
            return -1;
        }
        int minIdx = data[0];
        data[0] = data[size - 1];
        size--;

        if (size > 0)
            downheap(0, weights);

        return minIdx;
    }

    // Bubble upward to maintain heap order
    void upheap(int pos, int weights[]) {
        while (pos > 0) {
            int parent = (pos - 1) / 2;
            int currIdx = data[pos];
            int parentIdx = data[parent];

            bool needSwap = false;

            if (weights[currIdx] < weights[parentIdx]) // Smaller weight wins tie goes to smaller index

                needSwap = true;
            else if (weights[currIdx] == weights[parentIdx] && currIdx < parentIdx)
                needSwap = true;
            //postion are swapped

            if (needSwap) {
                int tmp = data[pos];
                data[pos] = data[parent];
                data[parent] = tmp;
                pos = parent;
            } else {
                break;
            }
        }
    }

    // Bubble downward if a child is smaller
    void downheap(int pos, int weights[]) {
        while (true) {
            int left = 2 * pos + 1;
            int right = 2 * pos + 2;
            int smallest = pos;

            if (left < size) {
                int leftIdx = data[left];
                int smallestIdx = data[smallest];
                if (weights[leftIdx] < weights[smallestIdx] ||
                    (weights[leftIdx] == weights[smallestIdx] && leftIdx < smallestIdx))
                    smallest = left;
            }

            if (right < size) {
                int rightIdx = data[right];
                int smallestIdx = data[smallest];
                if (weights[rightIdx] < weights[smallestIdx] ||
                    (weights[rightIdx] == weights[smallestIdx] && rightIdx < smallestIdx))
                    smallest = right;
            }

            if (smallest != pos) {
                int temp = data[pos];
                data[pos] = data[smallest];
                data[smallest] = temp;
                pos = smallest;
            } else {
                break;
            }
        }
    }
};

#endif

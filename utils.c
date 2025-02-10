/*
 * utils.c
 *
 *  Created on: 10 de fev de 2025
 *      Author: thiag
 */

#include <utils.h>

int max(const int arr[], int size, char check_only_module) {
    if (size <= 0) return 0; // Return 0 if the array is invalid

    int largest = arr[0];
    int i;
    for (i = 1; i < size; i++) {
        if (check_only_module ? module(arr[i]) > module(largest) : (arr[i]) > largest)
            largest = arr[i];
    }
    return largest;
}

int min(const int arr[], int size, char check_only_module) {
    if (size <= 0) return 0; // Return 0 if the array is invalid

    int smallest = arr[0];
    int i;
    for (i = 1; i < size; i++) {
        if (check_only_module ? module(arr[i]) < module(smallest) : arr[i] < smallest)
            smallest = arr[i];
    }
    return smallest;
}

int max_index(const int arr[], int size, char check_only_module) {
    if (size <= 0) return -1;

    int max_idx = 0;
    int i;
    for (i = 1; i < size; i++) {
        if (check_only_module ? module(arr[i]) > module(arr[max_idx]) : arr[i] > arr[max_idx])
            max_idx = i;
    }
    return max_idx;
}

int min_index(const int arr[], int size, char check_only_module) {
    if (size <= 0) return -1;

    int min_idx = 0;
    int i;
    for (i = 1; i < size; i++) {
        if (check_only_module ? module(arr[i]) < module(arr[min_idx]) : arr[i] < arr[min_idx])
            min_idx = i;
    }
    return min_idx;
}

unsigned int module(int x) {
    return (x < 0) ? -x: x;
}

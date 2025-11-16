// merge_sort.c
#include <stdlib.h>
#include <string.h>
#include "merge_sort.h"

static void merge(void **arr, void **tmp, int left, int mid, int right, cmp_func cmp) {
    int i = left;
    int j = mid;
    int k = left;
    while (i < mid && j < right) {
        if (cmp(arr[i], arr[j]) <= 0) {
            tmp[k++] = arr[i++];
        } else {
            tmp[k++] = arr[j++];
        }
    }
    while (i < mid) {
        tmp[k++] = arr[i++];
    }
    while (j < right) {
        tmp[k++] = arr[j++];
    }
    for (i = left; i < right; i++) {
        arr[i] = tmp[i];
    }
}

static void msort_rec(void **arr, void **tmp, int left, int right, cmp_func cmp) {
    if (right - left <= 1) {
        return;
    }
    int mid = (left + right) / 2;
    msort_rec(arr, tmp, left, mid, cmp);
    msort_rec(arr, tmp, mid, right, cmp);
    merge(arr, tmp, left, mid, right, cmp);
}

void merge_sort(void **arr, int n, cmp_func cmp) {
    if (n <= 1) {
        return;
    }
    void **tmp = (void **)malloc(sizeof(void *) * n);
    if (!tmp) {
        return;
    }
    msort_rec(arr, tmp, 0, n, cmp);
    free(tmp);
}

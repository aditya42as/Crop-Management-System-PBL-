// merge_sort.h
#ifndef MERGE_SORT_H
#define MERGE_SORT_H

typedef int (*cmp_func)(const void *, const void *);

void merge_sort(void **arr, int n, cmp_func cmp);

#endif

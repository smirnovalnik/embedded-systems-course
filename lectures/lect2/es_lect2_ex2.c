#include <stdio.h>
#include <stdlib.h>

int cmp(const void * a, const void * b)
{
    return *(int*)a - *(int*)b;
}

int main()
{
    int test[] = {88, 56, 100, 2, 25};
    int len = sizeof(test)/sizeof(int);

    /* Печать исходного массива */
    for(int i = 0; i < len; i++)
       printf("%d ", test[i]);
    printf("\n");

    /* Быстрая сортировка */
    qsort(test, len, sizeof(int), cmp);
    /* Печать отсортированного массива */
    for(int i = 0; i < len; i++)
        printf("%d ", test[i]);
    printf("\n");

    /* Поиск в отсортированном массиве методом деления пополам (бинарный поиск) */
    int key = 56;
    int* item = (int*)bsearch(&key, test, len, sizeof(int), cmp);
    if (item != NULL)
        printf("found\n");
    else
        printf("not be found\n");

    return 0;
}

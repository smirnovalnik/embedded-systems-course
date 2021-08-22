
#include <stdio.h>
/* Работа с символами char */
#include <ctype.h>
/* Строковые операции */
#include <string.h>
/* Математические операции */
#include <math.h>

#include <stdlib.h>

/* Обратить порядок элементов в массиве */
void reverse(char a[], int n)
{
    int i, j, tmp;
    for (i = 0, j = n - 1; i < j; i++, j--) {
        tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
    return; /* Необязательно */
}


int main()
{
    int h, m, s;
    if (scanf("Time: %d h %d m %d s", &h, &m, &s) == 3)
    {
        printf("OK %d h %d m %d s \r\n", h, m, s);
    }

    return 0;
}

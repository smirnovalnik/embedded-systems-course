/**
  ******************************************************************************
  * \file    main.c
  * \author  Александр Смирнов
  * \version 1.0.0
  * \date    1.08.2018
  * \brief   Пример программы для выполнения лабораторной работы № 1.
  *          Программа читает текстовый файл, содержащий по четыре числа
  *          на каждой строке и выводит наибольшее число из каждой строки
  *          на терминал.
  *          Граничные условия: 0 < количество строк <= 100
  *          Пример исходного текстового файла:
  *          1 2 3 4
  *          5 6 7 8
  *          9 10 11 12
  *          13 14 15 16
  *          Пример вывода:
  *          4
  *          8
  *          12
  *          14
  ******************************************************************************
  */

/* Подключение функций стандартного ввода/вывода
   (printf, scanf, fopen, fclose, fprintf, fscanf и т.д.)
   из стандартной библиотеки C */
#include <stdio.h>

/**
  * \brief  Вспомогательная функция для поиска наибольшего числа
  *         в массиве чисел.
  * \param  a - указатель на массив, len - длина массива
  * \retval наибольшее число в массиве
  */
int arr_max(int* a, int len)
{
    int max = a[0];

    for (int i = 1; i < len; i++)
        if (a[i] > max)
            max = a[i];

    return max;
}

/* Функция main. Точка входа в программу */
int main(void)
{
    /* Объявление файлового указателя */
    FILE *fp;

    /* Открытие файла test.txt на чтение ("r").
       Если файла не существует, то fopen возвратит NULL. */
    if ((fp = fopen("test.txt", "r")) == NULL)
    {
        printf("Could not open file\n"); /* Сообщаем об ошибке */
        return 0;
    }

    /* Объявление буфера для чисел */
    int numbers[4];

    /* Функция fscanf аналогична функции scanf, только первым
       аргументом функции является файловый указатель.
       Ожидаем четыре разделенных пробелами числа.
       Если fscanf не смог прочитать четыре числа (ошибка в формате или
       конец файла), то выходим из цикла. */
    while (fscanf(fp, "%d %d %d %d", &numbers[0], &numbers[1], &numbers[2], &numbers[3]) == 4)
    {
        /* Поиск наибольшего числа */
        int max = arr_max(numbers, 4);
        printf("%d\n", max); /* Печать числа */
    }

    /* Закрываем файл */
    fclose(fp);

    /* Код завершения программы.
       Код 0 - означает корректно завершение */
    return 0;
}

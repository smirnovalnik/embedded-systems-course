/**
  ******************************************************************************
  * \file    main.c
  * \author  Александр Смирнов
  * \version 1.0.0
  * \date    1.08.2018
  * \brief   Пример программы для выполнения лабораторной работы № 1.
  *          Программа определяет является ли пароль надежным или нет.
  *          Пароль является надежным, если его длина больше или равна
  *          10 символам, в нем содержится хотя бы одна цифра, одна буква нижнего
  *          регистра, одна буква верхнего регистра.
  *          Граничные условия: 0 <= длина строки <= 20.
  *          Пример ввода 1:
  *          qwerty1234
  *          Пример вывода 1:
  *          no
  *          Пример ввода 1:
  *          F8oWSc2KkF
  *          Пример вывода 1:
  *          yes
  ******************************************************************************
  */

/* Подключение функций стандартного ввода/вывода
   (printf, scanf, puts, gets, putchar, getchar и т.д.)
   из стандартной библиотеки C */
#include <stdio.h>
/* Подключение определения типа bool из стандартной библиотеки C */
#include <stdbool.h>

/**
  * \brief  Проверка пароля на надежность.
  * \param  str - нуль терминированная строка с паролем для анализа
  * \retval true - пароль надежный, false - нет
  */
bool is_strong_password(char* str)
{
    /* Подсчитываем количество символов в строке за исключением символа '\0'.
       Данный фрагмент можно заменить функцией strlen из стандартной библиотеки:
       #include <string.h>
       ...
       int str_len = strlen(s);
    */
    int str_len;
    for (str_len = 0; str[str_len] != '\0'; str_len++)
        ;

    /* Если строка короткая, то сразу выходим из функции */
    if (str_len < 10)
        return false;

    bool digit = false, upper = false, lower = false;

    /* Организуем цикл и проверяем каждый символ на соответствие условию */
    for (int i = 0; i < str_len; i++)
    {
        /* Данную проверку на цифру можно заменить функцией из стандартной
           библиотеки C.
           #include <ctype.h>
           ...
           if (isdigit(str[i]))
           ...
           Аналогичные функции: isupper - верхний регистр,
           islower - нижний регистр. */
        if (str[i] >= '0' && str[i] <= '9')
            digit = true;

        if (str[i] >= 'A' && str[i] <= 'Z')
            upper = true;

        if (str[i] >= 'a' && str[i] <= 'z')
            lower = true;
    }

    /* Пароль является надежным если истинны все три условия */
    return digit && upper && lower;
}

/* Функция main. Точка входа в программу.
   Выход из программы осуществляется комбинацией клавиш Ctrl - C */
int main(void)
{
    /* Выделяем буфер под вводимую строку.
       Для строки отводится памяти на один байт больше для символа
       окончания строки '\0'. */
    char str_buf[20 + 1];

    /* Печать строки приветствия в стандартный поток */
    printf("Check password\n");

    /* Бесконечный цикл */
    for(;;)
    {
        /* Чтение символов из потока ввода и сохранение их в str_buf */
        scanf("%20s", str_buf);

        /* Печатаем строку "yes" или "no" в зависимости от результата работы
           функции is_strong_password */
        if (is_strong_password(str_buf) == true)
            printf("Strong password\n");
        else
            printf("Weak password\n");
    }
}

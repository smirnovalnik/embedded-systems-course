/**
  ******************************************************************************
  * \file    main.c
  * \author  Александр Смирнов
  * \version 1.0.0
  * \date    1.08.2018
  * \brief   Пример программы на работу для выполнения лабораторной работы № 1.
  *          Программа осуществляет поиск с заменой одной строки
  *          на другую. Замене подвергается первое вхождение строки.
  *          Граничные условия: 0 <= длина строки <= 20.
  *          Пример ввода 1:
  *          This is a test string
  *          test
  *          beautiful
  *          Пример вывода 1:
  *          This is a beautiful string
  ******************************************************************************
  */

/* Подключение функций стандартного ввода/вывода
   (printf, scanf, puts, gets, getchar и т.д.) из стандартной библиотеки C */
#include <stdio.h>
/* Подключение функций для работы со строками (strlen, strstr, memcpy)
   из стандартной библиотеки C */
#include <string.h>

/**
  * \brief  Функция поиска и замены первого вхождения подстроки find
  *         на строку replace_with.
  * \param  origin - исходная строка
  *         find - строка для поиска и замены
  *         replace_with - строка
  *         result - строка с результатом
  * \retval нет
  */
void find_and_replace_with(const char* origin,
                           const char* find, const char* replace_with,
                           char* result)
{
    /* Поиск подстроки в строке с помощью функции из стандартной библиотеки strstr.
       Функция возвращает указатель на символ первого вхождения строки в подстроку.
       Если подстрока не найдена, то возвращается пустой указатель NULL */
    char* find_pointer = strstr(origin, find);

    if (find_pointer == NULL)
    {
        /* Для копирования памяти используется функция memcpy из стандартной
           библиотеки.
           Первым аргументов является указатель куда копируем,
           вторым - откуда копируем, третий аргумент - количество байт
           для копирования.
           Оператор sizeof возвращает размер литерала, то есть длину строки + 1 */
        memcpy(result, "String not found", sizeof("String not found"));
        return;
    }

/*
    Исходная строка (origin) разбивается на три части:
    начало исходной строки (head), найденная строка (find) и
    окончание исходной строки (tail).
    -----------------------
    |       origin        |
    -----------------------
    | head  | find | tail |
    -----------------------
    | This a test   string|
    -----------------------
             ^ - find_pointer
    Вначале определяем длины всех строк, затем последовательно копируем
    в буфер result вначале head из строки origin, затем replace_with и
    в конце tail, также из строки origin.

    Для определения начала подстрок используем адресную арифметику.
    Например строка tail начинается с адреса find_pointer плюс длина
    строки find (tail = find_pointer + find_len). Аналогично для других строк. */

    int origin_len = strlen(origin);
    int find_len = strlen(find);
    int replace_with_len = strlen(replace_with);
    int tail_len = strlen(find_pointer + find_len);
    int head_len = origin_len - (find_len + tail_len);

    memcpy(result, origin, head_len);
    memcpy(result + head_len, replace_with, replace_with_len);
    memcpy(result + head_len + replace_with_len, find_pointer + find_len, tail_len);

    /* Устанавливаем символ окончания строки */
    result[head_len + replace_with_len + tail_len] = '\0';
}

/**
  * \brief  Примитивная реализация функции чтения строки из потока ввода
  *         с контролем переполнения буфера
  * \param  buf - буфер для сохранения строки
  *         buf_len - размер буфера
  * \retval количество символов в строке без учета символа '\0'
  */
int _getline(char* buf, int buf_len)
{
    char c;
    int pos = 0;

    /* Сохранение символов в buf пока позволяет размер и
       не ввели символ '\n' */
    while (--buf_len > 0 && (c = getchar()) != '\n')
    {
        buf[pos] = c;
        pos++;
    }

    /* Если строка была слишком длинной (последний символ не '\n'),
       то очищаем буфер ввода до символа '\n' */
    if (c != '\n')
        while(getchar() != '\n');

    /* Устанаваливаем символ окончания строки */
    buf[pos] = '\0';

    /* Возвращаем длину прочитанной строки без учета '\0' */
    return pos;
}

/* Функция main. Точка входа в программу.
   Выход из программы осуществляется комбинацией клавиш Ctrl - C */
int main(void)
{
    /* Объявление буферов для строк. Максимальная длина строки - 20, но буферы
       увеличены на один байт для символа окончания строки '\0' */
    char origin_str[20 + 1];
    char find_str[20 + 1];
    char replace_with_str[20 + 1];

    /* Буфер результата замены. Возможно ситуация когда в строке длиной 20
       символов меняется один символ на другую строку длиной также 20 символов */
    char result_str[40 + 1];

    /* Печать строки приветствия в стандартный поток */
    printf("Find and replace substring\n");

    /* Бесконечный цикл */
    for (;;)
    {
        printf("Input original string\n");
        /* Прочитать строку из стандартного потока и поместить в origin_str.
           Чтение с помощью gets является небезопасной операцией и может вызвать
           переполнение буфера origin_str.
           При разработке программ на C, в особенности для встраиваемых систем,
           переполнение буфера может приводить к непредсказуемым результат.
           Для чтения рекомендуется использовать функцию _getline:
           _getline(origin_str, sizeof(origin_str), '\n');
           Вызов данной функции ожидает строку оканчивающуюся символом '\n' и
           помещает в буфер origin_str размером sizeof(origin_str).
           Оператор sizeof возвращает размер массива, в данном случае 21 байт. */
        if (_getline(origin_str, sizeof(origin_str)) == 0)
        {
            printf("Incorrect input\n"); /* Сообщаем о некорректном вводе */
            continue; /* Начинаем цикл заново */
        }

        printf("Find:\n");
        if (_getline(find_str, sizeof(find_str)) == 0)
        {
            printf("Incorrect input\n");
            continue;
        }

        printf("Replace with:\n");
        if (_getline(replace_with_str, sizeof(replace_with_str)) == 0)
        {
            printf("Incorrect input\n");
            continue;
        }

        /* Вызываем функцию поиска и замены */
        find_and_replace_with(origin_str, find_str, replace_with_str, result_str);

        /* Печатаем результат */
        printf("Result:\n%s\n", result_str);
    }
}

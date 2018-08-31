# Курс "Отладочные средства микропроцессорных систем"

Несмотря на название курс охватывает не только средства отладки, но и принципы
построения и разработки современных встраиваемых микропроцессорных систем.

Целями данного курса являются:

* изучение языка программирования C и его применения для встраиваемых
  микропроцессорных систем
* знакомство с микроконтроллером Миландр К1986ВЕ92QI(MDR32F9Q2I)
* изучение современных методик разработки и отладки встраиваемых
  микропроцессорных систем

Материалы курса размещены на [GitHub](https://github.com),
который является большим хранилищем программ (исходных текстов)
под управлением системы контроля версий git.

Освоение системы контроля версий git не входит в данный курс,
однако рекомендуется ознакомится с git, а также с проектами для встраиваемых
систем, находящимися на [GitHub](https://github.com).
Примером является
[Стандартная библиотека периферии для МК семейства 1986x](https://github.com/eldarkg/emdr1986x-std-per-lib),
которая активно используется в данном курсе, а также
[примеры и документация для нее](https://github.com/eldarkg/emdr1986x-std-per-lib-doc).

Перечень директорий репозитория:

* docs - содержит документацию на микроконтроллер и на отладочную плату
* labs - содержит примеры лабораторных работ
  * lab1a_ex - примеры лабораторной работы №1 выполняемой на персональном
    компьютере. В примерах показано применение языка C и
    средств стандартного ввода вывода,
    а также средств компиляции программы на персональном компьютере
  * lab1b_ex - пример лабораторной работы №1 выполняемой на микроконтроллере.
    В примере показано использование языка C и средств стандартного ввода вывода
    уже на микроконтроллере с применением интегрированной среды разработки Keil.
  * lab2_ex - в лабораторной работе №2 впервые явно используется периферия
    микроконтроллера с помощью библиотеки периферии SPL. Показан пример
    использования таймера в режиме входного захвата.
  * lab3_ex - пример лабораторной работы №3, в которой изучается модуль ЦАП и
    контроллер ПДП. Данные модули позволяют генерировать сигнал произвольной
    формы на выходе микроконтроллера.
  * lab4_ex - в лабораторной работе №4 продолжается изучение периферии
    микроконтроллера (АЦП, SPI, таймер). Показан пример измерения аналогового
    сигнала на входе микроконтроллера и вывод результата
    на жидко-кристаллический индикатор.
* lectures - содержит лекционные слайды и тексты программ
  использованных на лекциях
* manuals - содержит методические пособия

Выполнение лабораторных работ предполагает предварительную
самостоятельную подготовку: подготовка проекта, исходных кодов программы.

Разработка программ для микроконтроллера проводится
в интегрированной среде разработки (IDE)
[Keil MDK Lite Edition](http://www2.keil.com/mdk5/editions/lite).
Для загрузки потребуется регистрация на сайте Keil.
Эта версия обладает практически полной функциональностью профессиональной версии,
но с ограничением на размер скомпилированной программы в 32 кБайта.
Для лабораторных работ данного курса этого более чем достаточно.

Keil MDK Lite Edition имеет симулятор, поэтому работу программы
можно проверить без отладочной платы. Однако, данный симулятор поддерживает
только центральный процессор, но не периферийные модули микроконтроллера.

Keil MDK Lite Edition изначально ничего не знает о микроконтроллерах
фирмы Миландр. Поэтому потребуется установить пакет
[mdr_spl_v1.4.1.pack](https://ic.milandr.ru/upload/iblock/b7c/b7cac540c48efc2c8ca688318a1ddd18.rar).
После установке данного пакета при создании проекта можно будет выбрать
микроконтроллер К1986ВЕ92QI (MDR32F9Q2I). В среде появится возможность
добавлять модули из *Стандартной библиотеки периферии для МК семейства 1986x (SPL)*,
а также модули от компании ARM.
Сведения об применении *Стандартной библиотеки периферии (SPL)* можно получить из
[примеров](https://github.com/eldarkg/emdr1986x-std-per-lib-doc).

Первая часть лабораторной работы №1 выполняется на персональном компьютере.
Инструкция по запуску компилятора находится в директории labs/lab1a_ex.

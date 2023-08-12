/* Программа scat */

/* Для программ на ПК */
#include <stdio.h>
int main(int argc, char* argv[])
{
    /* argc - количество аргументов */
    /* argv[0] - имя исполняемого файла,
       argv[1] - аргумент */
    int i;
    for (i = 0; i < argc; i++)
        printf("argv[%d]: %s\n", i, argv[i]);

    return 0;
}

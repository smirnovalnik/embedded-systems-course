/* Программа scat */

#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    /* Проверка наличия параметра -n */
    int n = 0;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-n") == 0)
        {
            n = 1;
        }
    }

    /* Вывод содержимого файлов */
    FILE *fd;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-n") == 0)
            continue;
        if (argv[i][0] == '-')
        {
            printf("scat: %s: Invalid option\n", argv[i]);
            break;
        }
        if ((fd = fopen(argv[i], "r")) == NULL)
        {
            printf("scat: %s: No such file or directory\n", argv[i]);
            continue;
        }
        char buf[100];
        int j = 1;
        while (fgets(buf, sizeof(buf), fd))
        {
            if (n)
                printf("%d %s", j, buf);
            else
                printf("%s", buf);
            j++;
        }
        fclose(fd);
    }

    return 0;
}

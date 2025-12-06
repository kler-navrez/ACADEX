#include <stdio.h>
#include <stdlib.h>

void readme(void);

int main(void)
{
    readme();
    return 0;
}

void readme(void)
{
    FILE *readme = fopen("readme.txt", "r");

    if (readme == NULL)
    {
        printf("Error: Could not open readme.txt\n");
        return;
    }

    int ch;
    while ((ch = fgetc(readme)) != EOF)
    {
        putchar(ch);
    }

    fclose(readme);
}

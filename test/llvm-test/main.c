#include <stdio.h>

void write(int a) { printf("%d\n", a); }

int read()
{
    int a;
    scanf("%d", &a);
    return a;
}

extern int do_main();

int main()
{
    do_main();
    return 0;
}
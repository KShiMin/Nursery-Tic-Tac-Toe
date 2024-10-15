#include <stdio.h>

int main(int argc, char const *argv[])
{
    char str[] = "Hello World!\nThis is a test for the compilation script!!\n";

    printf("%s", &str);

    return 0;
}

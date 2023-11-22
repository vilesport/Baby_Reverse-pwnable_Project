#include <stdio.h>
#include <string.h>

int main() {
    int s = 3;
    int * str = &s;
    printf("%x\n", str);
    str++;
    printf("%x\n", str);
    //printf("%s", s);
}

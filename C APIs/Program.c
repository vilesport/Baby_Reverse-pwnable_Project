#include <stdio.h>
#include <string.h>


int main() {
    char *str = malloc(8 * (sizeof(char)));
    str[0] = 'a';
    str[1] = '0';
    printf("%c", *(str + 1));
}

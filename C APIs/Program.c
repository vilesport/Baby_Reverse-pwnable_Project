#include <stdio.h>
#include <string.h>

int main() {
    char str1[] = "abdef";
    char str2[] = "cd";
    char str3[] = "lmao";
    int x = 6;
    int *k = &x;
    memmove(str1, str3, *k);
    printf("%s\n%s\n%s", str1, str2, str3);
}

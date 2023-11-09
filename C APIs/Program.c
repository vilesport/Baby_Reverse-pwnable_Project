#include <stdio.h>
#include <string.h>

int main() {
    char str1[] = "abdef";
    char str2[] = "cd";
    strncpy(str2, str1, strlen(str1));
    printf("%s\n%s", str1, str2);
}

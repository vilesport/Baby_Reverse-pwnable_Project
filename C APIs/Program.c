#include <stdio.h>
#include <string.h>

int main() {
    char str1[] = "abcde";
    char str2[] = "fgh";

    printf("%s\n%s\n", str1, str2);

    int cases = strcmp(str1, str2);

    if(cases < 0)
    {
        char str3[] = "";
        strcpy(str3, str1);
        printf("%s %s\n", str3, str2);
        strcpy(str1, str2);
        printf("%s\n", str1);
        strcpy(str2, str3);
        printf("%s\n%s\n", str1, str2);
        return 0;
    }
}

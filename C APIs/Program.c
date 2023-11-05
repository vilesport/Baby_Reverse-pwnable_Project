#include <stdio.h>
#include <string.h>

int main() {
    char str1[] = "memmove can be very useful......";
    memmove(str1 + 20, str1 + 15, 11);
    printf("%s\n", str1);
    return 0;
}

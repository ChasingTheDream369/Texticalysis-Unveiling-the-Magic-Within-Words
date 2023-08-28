#include <stdio.h>
#include <string.h>

int main()
{
    char str1[] = "ew", str2[] = "abCdef", str3[] = "abcdef";
    int result;

    // comparing strings str1 and str2
    result = strcmp(str1, str2);
    printf("strcmp(str1, str2) = %d\n", result);

    // comparing strings str1 and str3
    result = strcmp(str2, str3);
    printf("strcmp(str1, str3) = %d\n", result);

    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main() {
    char input[MAX_INPUT_LEN];
    char *p = input;
    char *end = input + MAX_INPUT_LEN - 1;
    int ch;
    size_t len = 0;

    printf("Enter a string (max %d chars): ", MAX_INPUT_LEN - 1);
    
    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (p < end) {
            *p++ = (char)ch;
            len++;
        } else {
            break;
        }
    }
    *p = '\0';

    if (len == 0) {
        printf("Empty input.\n");
        return 1;
    }

    char *start = input;
    char *q = input;
    
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    
    if (*start == '\0') {
        printf("String contains only whitespace.\n");
        return 1;
    }

    char *last_char = input + len - 1;
    while (last_char > start && isspace((unsigned char)*last_char)) {
        *last_char = '\0';
        last_char--;
    }

    p = start;
    while (*p) {
        if (!isspace((unsigned char)*p)) {
            *q++ = *p;
        }
        p++;
    }
    *q = '\0';

    if (q == input) {
        printf("No non-whitespace characters after trimming.\n");
        return 1;
    }

    len = (size_t)(q - input);
    
    p = input;
    char *mid = input + len / 2;
    char *rev = input + len - 1;
    
    while (p < mid) {
        char temp = *p;
        *p = *rev;
        *rev = temp;
        p++;
        rev--;
    }

    printf("Processed string (trimmed, spaces removed, reversed): %s\n", input);
    
    return 0;
}
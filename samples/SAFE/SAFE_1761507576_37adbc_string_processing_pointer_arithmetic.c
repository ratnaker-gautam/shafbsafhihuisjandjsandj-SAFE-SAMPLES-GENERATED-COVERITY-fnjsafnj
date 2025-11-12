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
    int c;
    size_t len = 0;

    printf("Enter a string (max %d chars): ", MAX_INPUT_LEN - 1);
    
    while ((c = getchar()) != '\n' && c != EOF) {
        if (p < end) {
            *p++ = (char)c;
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

    p = start;
    while (*p) {
        if (!isspace((unsigned char)*p)) {
            *q++ = *p;
        }
        p++;
    }
    *q = '\0';

    len = q - start;
    if (len == 0) {
        printf("String became empty after trimming.\n");
        return 1;
    }

    p = start;
    char *r = start + len - 1;
    while (p < r) {
        char temp = *p;
        *p = *r;
        *r = temp;
        p++;
        r--;
    }

    printf("Reversed trimmed string: %s\n", start);

    p = start;
    int word_count = 0;
    int in_word = 0;
    
    while (*p) {
        if (!isspace((unsigned char)*p)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        p++;
    }

    printf("Word count: %d\n", word_count);

    return 0;
}
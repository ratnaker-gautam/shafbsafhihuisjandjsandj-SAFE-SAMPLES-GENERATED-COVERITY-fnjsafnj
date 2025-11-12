//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main(void) {
    char input[MAX_INPUT_LEN];
    char *p = input;
    char *end = input + MAX_INPUT_LEN - 1;
    int c;
    size_t len = 0;

    printf("Enter a string (max %d chars): ", MAX_INPUT_LEN - 1);
    
    while ((c = getchar()) != EOF && c != '\n' && p < end) {
        *p++ = (char)c;
        len++;
    }
    *p = '\0';

    if (len == 0) {
        printf("No input provided.\n");
        return 1;
    }

    printf("Original: %s\n", input);
    printf("Length: %zu\n", len);

    char *start = input;
    char *q = input;
    int word_count = 0;
    int in_word = 0;

    while (*start) {
        if (isspace((unsigned char)*start)) {
            if (in_word) {
                word_count++;
                in_word = 0;
            }
        } else {
            if (!in_word) {
                in_word = 1;
            }
        }
        start++;
    }
    if (in_word) {
        word_count++;
    }

    printf("Word count: %d\n", word_count);

    start = input;
    char *r = input;
    in_word = 0;

    while (*start) {
        if (isspace((unsigned char)*start)) {
            if (in_word) {
                *r++ = ' ';
                in_word = 0;
            }
        } else {
            *r++ = *start;
            in_word = 1;
        }
        start++;
    }
    if (r > input && *(r - 1) == ' ') {
        r--;
    }
    *r = '\0';

    printf("Normalized: %s\n", input);
    printf("New length: %zu\n", strlen(input));

    start = input;
    char *s = input;
    char *t = input + strlen(input) - 1;

    while (s < t) {
        char temp = *s;
        *s = *t;
        *t = temp;
        s++;
        t--;
    }

    printf("Reversed: %s\n", input);

    return 0;
}
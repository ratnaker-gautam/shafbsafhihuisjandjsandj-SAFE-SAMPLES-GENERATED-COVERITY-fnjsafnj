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
        if (isalpha((unsigned char)*start)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        start++;
    }

    printf("Word count: %d\n", word_count);

    start = input;
    char *r = input;
    in_word = 0;

    while (*start) {
        if (isalpha((unsigned char)*start)) {
            if (!in_word) {
                *r = (char)toupper((unsigned char)*start);
            } else {
                *r = (char)tolower((unsigned char)*start);
            }
            r++;
            in_word = 1;
        } else {
            *r = *start;
            r++;
            in_word = 0;
        }
        start++;
    }
    *r = '\0';

    printf("Capitalized words: %s\n", input);

    size_t new_len = strlen(input);
    char *left = input;
    char *right = input + new_len - 1;

    while (left < right) {
        char temp = *left;
        *left = *right;
        *right = temp;
        left++;
        right--;
    }

    printf("Reversed: %s\n", input);

    return 0;
}
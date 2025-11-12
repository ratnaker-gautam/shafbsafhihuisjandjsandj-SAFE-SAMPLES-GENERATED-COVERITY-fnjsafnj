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

    if (len >= MAX_INPUT_LEN - 1) {
        printf("Input too long. Truncated.\n");
    }

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
            *q++ = *start;
        } else if (isspace((unsigned char)*start)) {
            if (in_word) {
                *q++ = ' ';
                in_word = 0;
            }
        }
        start++;
    }

    if (q > input && *(q - 1) == ' ') {
        q--;
    }
    *q = '\0';

    printf("Processed string: \"%s\"\n", input);
    printf("Word count: %d\n", word_count);

    char *rev = input;
    char *left = input;
    char *right = q - 1;

    while (left < right) {
        char temp = *left;
        *left = *right;
        *right = temp;
        left++;
        right--;
    }

    printf("Reversed string: \"%s\"\n", input);

    return 0;
}
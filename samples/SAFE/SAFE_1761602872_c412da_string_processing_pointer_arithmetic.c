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

    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
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
    char *word_start = NULL;
    char *word_end = NULL;
    in_word = 0;

    printf("Words reversed: ");
    while (*start) {
        if (isalpha((unsigned char)*start)) {
            if (!in_word) {
                word_start = start;
                in_word = 1;
            }
        } else {
            if (in_word) {
                word_end = start - 1;
                char *rev = word_end;
                while (rev >= word_start) {
                    putchar(*rev);
                    rev--;
                }
                putchar(' ');
                in_word = 0;
            }
        }
        start++;
    }

    if (in_word && word_start) {
        word_end = start - 1;
        char *rev = word_end;
        while (rev >= word_start) {
            putchar(*rev);
            rev--;
        }
    }
    putchar('\n');

    start = input;
    char *dest = input;
    char *src = input;
    int space_flag = 0;

    while (*src) {
        if (isspace((unsigned char)*src)) {
            if (!space_flag) {
                *dest++ = ' ';
                space_flag = 1;
            }
        } else {
            *dest++ = *src;
            space_flag = 0;
        }
        src++;
    }
    *dest = '\0';

    printf("Normalized: %s\n", input);

    start = input;
    char *mid = input + len / 2;
    char *left = input;
    char *right = input + len - 1;

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
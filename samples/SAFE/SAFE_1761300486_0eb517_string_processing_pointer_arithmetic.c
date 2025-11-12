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
            printf("Input too long. Truncating.\n");
            break;
        }
    }
    *p = '\0';

    if (len == 0) {
        printf("No input provided.\n");
        return 1;
    }

    char *start = input;
    char *q = input;
    int word_count = 0;
    int in_word = 0;

    while (*start != '\0') {
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

    if (word_count == 0) {
        printf("No alphabetic words found.\n");
        return 1;
    }

    printf("Processed string (%d words): ", word_count);
    
    char *output = input;
    while (*output != '\0') {
        putchar(*output);
        output++;
    }
    putchar('\n');

    char *reverse = input + strlen(input) - 1;
    printf("Reversed: ");
    while (reverse >= input) {
        putchar(*reverse);
        reverse--;
    }
    putchar('\n');

    return 0;
}
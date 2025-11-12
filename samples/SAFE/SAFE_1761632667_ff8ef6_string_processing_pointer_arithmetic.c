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
        printf("No input provided.\n");
        return 1;
    }

    printf("Original: %s\n", input);
    printf("Length: %zu\n", len);

    char *start = input;
    char *q = input;
    while (*start) {
        if (!isspace((unsigned char)*start)) {
            *q++ = *start;
        }
        start++;
    }
    *q = '\0';
    len = q - input;

    printf("Without spaces: %s\n", input);
    printf("New length: %zu\n", len);

    if (len > 0) {
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
    }

    size_t word_count = 0;
    char *ptr = input;
    int in_word = 0;
    while (*ptr) {
        if (isalnum((unsigned char)*ptr)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        ptr++;
    }
    printf("Word count: %zu\n", word_count);

    return 0;
}
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

    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN - 1);
    
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

    while (*start != '\0') {
        if (isalpha((unsigned char)*start)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
            *q++ = *start;
        } else {
            in_word = 0;
        }
        start++;
    }
    *q = '\0';

    printf("Alpha-only: %s\n", input);
    printf("Word count: %d\n", word_count);

    size_t new_len = q - input;
    if (new_len > 0) {
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
    } else {
        printf("No alphabetic characters found.\n");
    }

    return 0;
}
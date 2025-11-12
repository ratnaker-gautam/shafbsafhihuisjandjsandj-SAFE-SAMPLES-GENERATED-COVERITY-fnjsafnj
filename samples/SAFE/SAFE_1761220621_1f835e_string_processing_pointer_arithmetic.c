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

    if (word_count == 0) {
        printf("No alphabetic words found.\n");
        return 1;
    }

    printf("Processed string (%d words): %s\n", word_count, input);

    char *rev_start = input;
    char *rev_end = input + strlen(input) - 1;
    
    while (rev_start < rev_end) {
        char temp = *rev_start;
        *rev_start = *rev_end;
        *rev_end = temp;
        rev_start++;
        rev_end--;
    }

    printf("Reversed: %s\n", input);

    char *word_start = input;
    char *word_ptr = input;
    
    while (*word_ptr) {
        if (*word_ptr == ' ') {
            char *word_end = word_ptr - 1;
            while (word_start < word_end) {
                char temp = *word_start;
                *word_start = *word_end;
                *word_end = temp;
                word_start++;
                word_end--;
            }
            word_start = word_ptr + 1;
        }
        word_ptr++;
    }
    
    if (word_start < word_ptr) {
        char *word_end = word_ptr - 1;
        while (word_start < word_end) {
            char temp = *word_start;
            *word_start = *word_end;
            *word_end = temp;
            word_start++;
            word_end--;
        }
    }

    printf("Words reversed: %s\n", input);

    return 0;
}
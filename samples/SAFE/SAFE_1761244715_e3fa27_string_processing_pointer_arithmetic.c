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
        last_char--;
    }
    *(last_char + 1) = '\0';

    p = start;
    q = start;
    int in_word = 0;
    size_t word_count = 0;
    size_t char_count = 0;

    while (*p) {
        if (!isspace((unsigned char)*p)) {
            if (!in_word) {
                in_word = 1;
                word_count++;
            }
            char_count++;
            *q++ = *p;
        } else {
            if (in_word) {
                in_word = 0;
                *q++ = ' ';
            }
        }
        p++;
    }
    if (q > start && *(q - 1) == ' ') {
        q--;
    }
    *q = '\0';

    if (word_count == 0) {
        printf("No words found after processing.\n");
        return 1;
    }

    printf("Processed string: \"%s\"\n", start);
    printf("Word count: %zu\n", word_count);
    printf("Character count (excluding spaces): %zu\n", char_count);

    p = start;
    char *word_start = start;
    size_t max_len = 0;
    char *longest_word = NULL;

    while (*p) {
        if (!isspace((unsigned char)*p)) {
            word_start = p;
            while (*p && !isspace((unsigned char)*p)) {
                p++;
            }
            size_t word_len = p - word_start;
            if (word_len > max_len) {
                max_len = word_len;
                longest_word = word_start;
            }
        } else {
            p++;
        }
    }

    if (longest_word != NULL) {
        printf("Longest word: \"");
        for (size_t i = 0; i < max_len; i++) {
            putchar(longest_word[i]);
        }
        printf("\" (length: %zu)\n", max_len);
    }

    p = start;
    char *rev_start = start;
    char *rev_end = start + strlen(start) - 1;

    while (rev_start < rev_end) {
        char temp = *rev_start;
        *rev_start = *rev_end;
        *rev_end = temp;
        rev_start++;
        rev_end--;
    }

    printf("Reversed string: \"%s\"\n", start);

    return 0;
}
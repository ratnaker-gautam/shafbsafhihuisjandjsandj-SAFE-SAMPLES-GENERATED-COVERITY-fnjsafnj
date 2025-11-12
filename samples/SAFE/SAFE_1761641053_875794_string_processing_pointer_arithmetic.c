//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char output[MAX_LENGTH];
    char *src, *dest;
    int word_count = 0;
    int in_word = 0;

    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    if (len >= MAX_LENGTH - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    src = input;
    dest = output;

    while (*src != '\0') {
        if (isspace((unsigned char)*src)) {
            if (in_word) {
                in_word = 0;
                *dest++ = ' ';
            }
            src++;
        } else if (isalnum((unsigned char)*src)) {
            if (!in_word) {
                in_word = 1;
                word_count++;
            }
            *dest++ = *src++;
        } else {
            src++;
        }
    }

    if (dest > output && *(dest - 1) == ' ') {
        dest--;
    }
    *dest = '\0';

    char reversed[MAX_LENGTH];
    char *rev_ptr = reversed;
    char *word_start = output;
    char *word_end = output;

    while (*word_end != '\0') {
        if (*word_end == ' ') {
            char *temp = word_end - 1;
            while (temp >= word_start) {
                *rev_ptr++ = *temp--;
            }
            *rev_ptr++ = ' ';
            word_start = word_end + 1;
        }
        word_end++;
    }

    char *temp = word_end - 1;
    while (temp >= word_start) {
        *rev_ptr++ = *temp--;
    }
    *rev_ptr = '\0';

    printf("Original: %s\n", input);
    printf("Processed: %s\n", output);
    printf("Word count: %d\n", word_count);
    printf("Reversed words: %s\n", reversed);

    return 0;
}
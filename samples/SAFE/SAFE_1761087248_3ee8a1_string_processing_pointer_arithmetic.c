//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main() {
    char input[MAX_INPUT_LEN];
    char *p = input;
    char *end = input + MAX_INPUT_LEN - 1;
    int c;
    size_t len = 0;

    printf("Enter a string: ");
    fflush(stdout);

    while ((c = getchar()) != EOF && c != '\n' && p < end) {
        *p++ = (char)c;
        len++;
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

    char *words[word_count];
    int word_index = 0;
    char *token = input;
    char *next = input;

    while (*next != '\0') {
        if (*next == ' ') {
            *next = '\0';
            if (word_index < word_count) {
                words[word_index++] = token;
            }
            token = next + 1;
        }
        next++;
    }
    if (word_index < word_count) {
        words[word_index] = token;
    }

    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            char *a = words[j];
            char *b = words[j + 1];
            while (*a && *b && *a == *b) {
                a++;
                b++;
            }
            if (*a > *b) {
                char *temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }

    printf("Sorted words: ");
    for (int i = 0; i < word_count; i++) {
        char *w = words[i];
        while (*w != '\0') {
            putchar(*w);
            w++;
        }
        if (i < word_count - 1) {
            putchar(' ');
        }
    }
    putchar('\n');

    return 0;
}
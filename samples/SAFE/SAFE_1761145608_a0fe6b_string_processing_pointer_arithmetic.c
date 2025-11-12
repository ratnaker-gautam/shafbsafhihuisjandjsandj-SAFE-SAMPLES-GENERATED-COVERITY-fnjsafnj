//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int is_valid_input(const char *str, size_t len) {
    if (str == NULL || len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    const char *p = str;
    while (*p != '\0') {
        if (!isprint(*p)) {
            return 0;
        }
        p++;
    }
    return 1;
}

void reverse_string(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }
    char *start = str;
    char *end = str;
    while (*end != '\0') {
        end++;
    }
    end--;
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void remove_vowels(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }
    char *read_ptr = str;
    char *write_ptr = str;
    while (*read_ptr != '\0') {
        char c = *read_ptr;
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' &&
            c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') {
            *write_ptr = c;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

void count_characters(const char *str, int *counts) {
    if (str == NULL || counts == NULL) {
        return;
    }
    for (int i = 0; i < 26; i++) {
        counts[i] = 0;
    }
    const char *p = str;
    while (*p != '\0') {
        char c = *p;
        if (isalpha(c)) {
            if (c >= 'a' && c <= 'z') {
                counts[c - 'a']++;
            } else if (c >= 'A' && c <= 'Z') {
                counts[c - 'A']++;
            }
        }
        p++;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    if (!is_valid_input(input, len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    char processed[MAX_INPUT_LEN];
    if (strlen(input) >= sizeof(processed)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    strcpy(processed, input);
    printf("Original: %s\n", processed);
    reverse_string(processed);
    printf("Reversed: %s\n", processed);
    strcpy(processed, input);
    remove_vowels(processed);
    printf("Without vowels: %s\n", processed);
    int char_counts[26];
    count_characters(input, char_counts);
    printf("Character counts:\n");
    for (int i = 0; i < 26; i++) {
        if (char_counts[i] > 0) {
            printf("%c: %d\n", 'a' + i, char_counts[i]);
        }
    }
    return 0;
}
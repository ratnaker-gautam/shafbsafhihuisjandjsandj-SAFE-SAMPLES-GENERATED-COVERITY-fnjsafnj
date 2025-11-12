//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    const char *p = str;
    while (*p != '\0') {
        if (!isalnum((unsigned char)*p) && *p != ' ') return 0;
        p++;
    }
    return 1;
}

void reverse_string(char *str) {
    if (str == NULL) return;
    char *start = str;
    char *end = str;
    while (*end != '\0') end++;
    if (end == str) return;
    end--;
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void process_string(char *str) {
    if (str == NULL) return;
    char *read = str;
    char *write = str;
    while (*read != '\0') {
        if (isalpha((unsigned char)*read)) {
            *write = toupper((unsigned char)*read);
            write++;
        }
        read++;
    }
    *write = '\0';
}

int main() {
    char input[MAX_INPUT + 1];
    printf("Enter a string (alphanumeric and spaces only, max %d chars): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    if (len >= MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid characters in input\n");
        return 1;
    }
    char *buffer = malloc(len + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    char *src = input;
    char *dest = buffer;
    while (*src != '\0') {
        *dest = *src;
        src++;
        dest++;
    }
    *dest = '\0';
    printf("Original: %s\n", buffer);
    reverse_string(buffer);
    printf("Reversed: %s\n", buffer);
    process_string(buffer);
    printf("Processed: %s\n", buffer);
    free(buffer);
    return 0;
}
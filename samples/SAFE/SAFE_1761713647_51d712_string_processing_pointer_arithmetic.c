//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    const char *p = str;
    while (*p) {
        if (!isprint(*p)) return 0;
        p++;
    }
    return 1;
}

void reverse_string(char *str) {
    if (str == NULL || *str == '\0') return;
    
    char *start = str;
    char *end = str;
    while (*(end + 1)) end++;
    
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void remove_whitespace(char *str) {
    if (str == NULL) return;
    
    char *read = str;
    char *write = str;
    
    while (*read) {
        if (!isspace(*read)) {
            *write = *read;
            write++;
        }
        read++;
    }
    *write = '\0';
}

void count_characters(const char *str, int *letters, int *digits, int *others) {
    if (str == NULL || letters == NULL || digits == NULL || others == NULL) return;
    
    *letters = 0;
    *digits = 0;
    *others = 0;
    
    const char *p = str;
    while (*p) {
        if (isalpha(*p)) (*letters)++;
        else if (isdigit(*p)) (*digits)++;
        else (*others)++;
        p++;
    }
}

int main() {
    char input[MAX_INPUT + 1];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline) *newline = '\0';
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input: contains non-printable characters\n");
        return 1;
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input string\n");
        return 1;
    }
    
    printf("Original string: \"%s\"\n", input);
    printf("Length: %zu\n", strlen(input));
    
    int letters, digits, others;
    count_characters(input, &letters, &digits, &others);
    printf("Letters: %d, Digits: %d, Others: %d\n", letters, digits, others);
    
    char processed[MAX_INPUT + 1];
    if (strlen(input) >= sizeof(processed)) {
        fprintf(stderr, "Input too long for processing\n");
        return 1;
    }
    strcpy(processed, input);
    
    remove_whitespace(processed);
    printf("Without whitespace: \"%s\"\n", processed);
    
    strcpy(processed, input);
    reverse_string(processed);
    printf("Reversed: \"%s\"\n", processed);
    
    return 0;
}
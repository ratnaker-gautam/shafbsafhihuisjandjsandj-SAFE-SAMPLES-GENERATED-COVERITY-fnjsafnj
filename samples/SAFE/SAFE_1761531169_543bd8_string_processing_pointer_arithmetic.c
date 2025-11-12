//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    const char *p = str;
    while (*p != '\0') {
        if (!isprint(*p)) return 0;
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

void remove_non_alpha(char *str) {
    if (str == NULL) return;
    char *read = str;
    char *write = str;
    while (*read != '\0') {
        if (isalpha(*read)) {
            *write = *read;
            write++;
        }
        read++;
    }
    *write = '\0';
}

int count_vowels(const char *str) {
    if (str == NULL) return 0;
    int count = 0;
    const char *p = str;
    while (*p != '\0') {
        char c = tolower(*p);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
        p++;
    }
    return count;
}

int main() {
    char input[MAX_LENGTH + 1];
    printf("Enter a string (max %d characters): ", MAX_LENGTH);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline != NULL) *newline = '\0';
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input: contains non-printable characters\n");
        return 1;
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty string provided\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    
    int vowels = count_vowels(input);
    printf("Vowel count: %d\n", vowels);
    
    char processed[MAX_LENGTH + 1];
    if (strlen(input) >= sizeof(processed)) {
        fprintf(stderr, "Input too long for processing\n");
        return 1;
    }
    strcpy(processed, input);
    
    remove_non_alpha(processed);
    printf("Alpha only: %s\n", processed);
    
    reverse_string(processed);
    printf("Reversed: %s\n", processed);
    
    return 0;
}
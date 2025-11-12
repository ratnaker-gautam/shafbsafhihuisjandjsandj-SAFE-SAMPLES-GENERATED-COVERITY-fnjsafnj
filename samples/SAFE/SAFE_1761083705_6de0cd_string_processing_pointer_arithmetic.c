//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT) return 0;
    return 1;
}

void reverse_string(char *str) {
    if (str == NULL) return;
    char *start = str;
    char *end = str + strlen(str) - 1;
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void remove_non_alnum(char *str) {
    if (str == NULL) return;
    char *read = str;
    char *write = str;
    while (*read != '\0') {
        if (isalnum((unsigned char)*read)) {
            *write = *read;
            write++;
        }
        read++;
    }
    *write = '\0';
}

int count_words(const char *str) {
    if (str == NULL) return 0;
    int count = 0;
    int in_word = 0;
    const char *ptr = str;
    while (*ptr != '\0') {
        if (isspace((unsigned char)*ptr)) {
            in_word = 0;
        } else if (!in_word) {
            count++;
            in_word = 1;
        }
        ptr++;
    }
    return count;
}

void process_string(char *str) {
    if (str == NULL) return;
    char buffer[MAX_INPUT];
    if (strlen(str) >= MAX_INPUT) return;
    strcpy(buffer, str);
    
    printf("Original: %s\n", buffer);
    
    reverse_string(buffer);
    printf("Reversed: %s\n", buffer);
    
    reverse_string(buffer);
    remove_non_alnum(buffer);
    printf("Alphanumeric only: %s\n", buffer);
    
    int word_count = count_words(buffer);
    printf("Word count: %d\n", word_count);
}

int main() {
    char input[MAX_INPUT];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    process_string(input);
    
    return 0;
}
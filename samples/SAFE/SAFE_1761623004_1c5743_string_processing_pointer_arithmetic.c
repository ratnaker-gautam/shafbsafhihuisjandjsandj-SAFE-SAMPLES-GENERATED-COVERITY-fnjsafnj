//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
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

void to_uppercase(char *str) {
    if (str == NULL) return;
    while (*str != '\0') {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

int count_words(const char *str) {
    if (str == NULL) return 0;
    int count = 0;
    int in_word = 0;
    while (*str != '\0') {
        if (isspace((unsigned char)*str)) {
            in_word = 0;
        } else if (!in_word) {
            count++;
            in_word = 1;
        }
        str++;
    }
    return count;
}

void process_string(char *str) {
    if (str == NULL) return;
    printf("Original: %s\n", str);
    
    char *copy = malloc(strlen(str) + 1);
    if (copy == NULL) return;
    char *copy_ptr = copy;
    const char *src_ptr = str;
    while ((*copy_ptr++ = *src_ptr++) != '\0');
    
    reverse_string(copy);
    printf("Reversed: %s\n", copy);
    
    to_uppercase(copy);
    printf("Uppercase: %s\n", copy);
    
    int word_count = count_words(copy);
    printf("Word count: %d\n", word_count);
    
    free(copy);
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
//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
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

void convert_case(char *str) {
    if (str == NULL) return;
    char *ptr = str;
    while (*ptr != '\0') {
        if (isalpha((unsigned char)*ptr)) {
            if (islower((unsigned char)*ptr)) {
                *ptr = toupper((unsigned char)*ptr);
            } else {
                *ptr = tolower((unsigned char)*ptr);
            }
        }
        ptr++;
    }
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

int main(void) {
    char input[MAX_INPUT_LEN];
    char buffer[MAX_INPUT_LEN];
    
    printf("Enter a string: ");
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
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    printf("Original: %s\n", buffer);
    
    reverse_string(buffer);
    printf("Reversed: %s\n", buffer);
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    convert_case(buffer);
    printf("Case swapped: %s\n", buffer);
    
    int word_count = count_words(input);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
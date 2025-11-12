//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
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
    if (str == NULL || *str == '\0') return;
    
    char *start = str;
    char *end = str;
    while (*(end + 1) != '\0') end++;
    
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
    
    char *read_ptr = str;
    char *write_ptr = str;
    
    while (*read_ptr != '\0') {
        if (isalnum((unsigned char)*read_ptr)) {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

int count_words(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    
    int count = 0;
    int in_word = 0;
    const char *ptr = str;
    
    while (*ptr != '\0') {
        if (isalnum((unsigned char)*ptr)) {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        ptr++;
    }
    
    return count;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline != NULL) *newline = '\0';
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original: \"%s\"\n", input);
    
    int word_count = count_words(input);
    printf("Word count: %d\n", word_count);
    
    char processed[MAX_INPUT_LEN];
    strncpy(processed, input, sizeof(processed) - 1);
    processed[sizeof(processed) - 1] = '\0';
    
    process_string(processed);
    printf("Alphanumeric only: \"%s\"\n", processed);
    
    char reversed[MAX_INPUT_LEN];
    strncpy(reversed, input, sizeof(reversed) - 1);
    reversed[sizeof(reversed) - 1] = '\0';
    
    reverse_string(reversed);
    printf("Reversed: \"%s\"\n", reversed);
    
    return 0;
}
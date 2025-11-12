//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

void reverse_string(char *str) {
    if (str == NULL) return;
    
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

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
    
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
        printf("Empty input string\n");
        return 0;
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char processed[MAX_INPUT_LEN + 1];
    strncpy(processed, input, sizeof(processed));
    processed[sizeof(processed) - 1] = '\0';
    
    printf("Original string: %s\n", input);
    printf("Length: %zu\n", len);
    
    int word_count = count_words(input);
    printf("Word count: %d\n", word_count);
    
    remove_non_alnum(processed);
    printf("Alphanumeric only: %s\n", processed);
    
    reverse_string(processed);
    printf("Reversed: %s\n", processed);
    
    return 0;
}
//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

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
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH];
    
    printf("Enter a string (max %d characters): ", MAX_LENGTH - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline_ptr = strchr(input, '\n');
    if (newline_ptr != NULL) {
        *newline_ptr = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty string provided\n");
        return 1;
    }
    
    strncpy(processed, input, sizeof(processed) - 1);
    processed[sizeof(processed) - 1] = '\0';
    
    printf("Original string: %s\n", input);
    printf("Length: %zu\n", strlen(input));
    printf("Word count: %d\n", count_words(input));
    
    remove_non_alnum(processed);
    printf("Alphanumeric only: %s\n", processed);
    
    reverse_string(processed);
    printf("Reversed: %s\n", processed);
    
    return 0;
}
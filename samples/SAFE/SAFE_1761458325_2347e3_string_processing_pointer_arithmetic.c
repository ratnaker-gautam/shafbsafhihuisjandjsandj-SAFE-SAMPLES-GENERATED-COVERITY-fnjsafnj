//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256

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

void remove_whitespace(char *str) {
    if (str == NULL) return;
    
    char *read_ptr = str;
    char *write_ptr = str;
    
    while (*read_ptr != '\0') {
        if (!isspace((unsigned char)*read_ptr)) {
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
        if (isspace((unsigned char)*ptr)) {
            in_word = 0;
        } else {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        }
        ptr++;
    }
    
    return count;
}

int main() {
    char input[MAX_INPUT];
    char processed[MAX_INPUT];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input string\n");
        return 1;
    }
    
    strncpy(processed, input, sizeof(processed) - 1);
    processed[sizeof(processed) - 1] = '\0';
    
    printf("Original: \"%s\"\n", input);
    printf("Length: %zu\n", strlen(input));
    
    int word_count = count_words(input);
    printf("Word count: %d\n", word_count);
    
    reverse_string(processed);
    printf("Reversed: \"%s\"\n", processed);
    
    strncpy(processed, input, sizeof(processed) - 1);
    processed[sizeof(processed) - 1] = '\0';
    
    remove_whitespace(processed);
    printf("Without whitespace: \"%s\"\n", processed);
    
    return 0;
}
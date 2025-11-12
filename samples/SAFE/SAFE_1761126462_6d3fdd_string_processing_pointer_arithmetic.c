//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

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
    char input[MAX_LENGTH + 1];
    char processed[MAX_LENGTH + 1];
    
    printf("Enter a string (max %d characters): ", MAX_LENGTH);
    
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
        printf("Empty string entered\n");
        return 1;
    }
    
    if (len > MAX_LENGTH) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    strncpy(processed, input, MAX_LENGTH);
    processed[MAX_LENGTH] = '\0';
    
    printf("Original: \"%s\"\n", input);
    printf("Length: %zu\n", len);
    printf("Word count: %d\n", count_words(input));
    
    remove_non_alnum(processed);
    printf("Alphanumeric only: \"%s\"\n", processed);
    
    reverse_string(processed);
    printf("Reversed alphanumeric: \"%s\"\n", processed);
    
    return 0;
}
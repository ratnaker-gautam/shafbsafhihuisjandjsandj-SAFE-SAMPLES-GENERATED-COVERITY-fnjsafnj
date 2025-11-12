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
    if (str == NULL || *str == '\0') return;
    
    char *start = str;
    char *end = str;
    while (*end != '\0') end++;
    end--;
    
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
    
    char *ptr = str;
    while (*ptr != '\0') {
        if (isalpha(*ptr)) {
            if (islower(*ptr)) {
                *ptr = toupper(*ptr);
            } else {
                *ptr = tolower(*ptr);
            }
        }
        ptr++;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN - 1);
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
    
    char *original = malloc(MAX_INPUT_LEN);
    if (original == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char *ptr1 = input;
    char *ptr2 = original;
    while (*ptr1 != '\0') {
        *ptr2 = *ptr1;
        ptr1++;
        ptr2++;
    }
    *ptr2 = '\0';
    
    printf("Original: %s\n", original);
    
    process_string(input);
    printf("Processed: %s\n", input);
    
    reverse_string(input);
    printf("Reversed: %s\n", input);
    
    free(original);
    return 0;
}
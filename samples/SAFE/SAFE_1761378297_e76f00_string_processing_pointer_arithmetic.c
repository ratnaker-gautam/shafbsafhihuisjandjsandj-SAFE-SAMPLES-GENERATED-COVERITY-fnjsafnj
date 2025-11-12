//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
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

void process_string(char *input) {
    if (!validate_input(input)) {
        printf("Invalid input\n");
        return;
    }
    
    char *ptr = input;
    while (*ptr) {
        if (islower(*ptr)) {
            *ptr = toupper(*ptr);
        } else if (isupper(*ptr)) {
            *ptr = tolower(*ptr);
        }
        ptr++;
    }
    
    reverse_string(input);
    
    ptr = input;
    int word_count = 0;
    while (*ptr) {
        while (*ptr && isspace(*ptr)) ptr++;
        if (*ptr) word_count++;
        while (*ptr && !isspace(*ptr)) ptr++;
    }
    
    printf("Processed: %s\n", input);
    printf("Word count: %d\n", word_count);
}

int main() {
    char input[MAX_INPUT_LEN];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    process_string(input);
    
    return 0;
}
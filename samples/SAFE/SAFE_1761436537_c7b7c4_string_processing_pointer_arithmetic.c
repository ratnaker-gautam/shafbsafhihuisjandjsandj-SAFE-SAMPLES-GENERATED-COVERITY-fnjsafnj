//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    const char *p = str;
    while (*p != '\0') {
        if (!isalnum(*p) && *p != ' ') return 0;
        p++;
    }
    return 1;
}

void reverse_string(char *str) {
    if (str == NULL) return;
    char *start = str;
    char *end = str;
    while (*end != '\0') end++;
    if (end == str) return;
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
    char *p = str;
    while (*p != '\0') {
        if (islower(*p)) *p = toupper(*p);
        else if (isupper(*p)) *p = tolower(*p);
        p++;
    }
}

int count_words(const char *str) {
    if (str == NULL) return 0;
    int count = 0;
    int in_word = 0;
    const char *p = str;
    while (*p != '\0') {
        if (isalnum(*p)) {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        p++;
    }
    return count;
}

int main() {
    char input[MAX_INPUT + 1];
    
    printf("Enter a string (alphanumeric and spaces only, max %d chars): ", MAX_INPUT);
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (len > MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid characters in input\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    
    int word_count = count_words(input);
    printf("Word count: %d\n", word_count);
    
    char processed[MAX_INPUT + 1];
    if (strlen(input) >= sizeof(processed)) {
        fprintf(stderr, "Buffer size error\n");
        return 1;
    }
    strcpy(processed, input);
    
    process_string(processed);
    printf("Case swapped: %s\n", processed);
    
    reverse_string(processed);
    printf("Reversed: %s\n", processed);
    
    return 0;
}
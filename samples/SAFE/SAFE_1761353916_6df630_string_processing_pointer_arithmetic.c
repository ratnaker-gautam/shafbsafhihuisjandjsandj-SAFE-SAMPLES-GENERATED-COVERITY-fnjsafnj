//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    const char *p = str;
    while (*p) {
        if (!isprint(*p)) return 0;
        p++;
    }
    return 1;
}

void reverse_string(char *str) {
    if (str == NULL || *str == '\0') return;
    
    char *start = str;
    char *end = str;
    while (*(end + 1)) end++;
    
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
    
    char *read = str;
    char *write = str;
    
    while (*read) {
        if (!isspace(*read)) {
            *write = *read;
            write++;
        }
        read++;
    }
    *write = '\0';
}

int count_words(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    
    int count = 0;
    int in_word = 0;
    const char *p = str;
    
    while (*p) {
        if (isspace(*p)) {
            in_word = 0;
        } else if (!in_word) {
            count++;
            in_word = 1;
        }
        p++;
    }
    return count;
}

int main(void) {
    char input[MAX_INPUT + 1];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input: contains non-printable characters\n");
        return 1;
    }
    
    printf("Original: \"%s\"\n", input);
    printf("Length: %zu\n", strlen(input));
    printf("Word count: %d\n", count_words(input));
    
    char processed[MAX_INPUT + 1];
    if (strlen(input) >= sizeof(processed)) {
        fprintf(stderr, "Input too long for processing\n");
        return 1;
    }
    strcpy(processed, input);
    
    remove_whitespace(processed);
    printf("Without whitespace: \"%s\"\n", processed);
    
    strcpy(processed, input);
    reverse_string(processed);
    printf("Reversed: \"%s\"\n", processed);
    
    return 0;
}
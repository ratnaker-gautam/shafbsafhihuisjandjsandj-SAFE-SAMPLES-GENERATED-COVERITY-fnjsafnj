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

void process_string(char *str) {
    if (str == NULL) return;
    
    char *read_ptr = str;
    char *write_ptr = str;
    
    while (*read_ptr) {
        if (isalpha(*read_ptr)) {
            *write_ptr = toupper(*read_ptr);
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

int main(void) {
    char input[MAX_INPUT + 1];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline != NULL) *newline = '\0';
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input: contains non-printable characters\n");
        return 1;
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input string\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    
    reverse_string(input);
    printf("Reversed: %s\n", input);
    
    process_string(input);
    printf("Processed: %s\n", input);
    
    return 0;
}
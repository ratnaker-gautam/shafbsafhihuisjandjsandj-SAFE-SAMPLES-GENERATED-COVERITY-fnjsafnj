//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int is_valid_input(const char *str, size_t len) {
    if (str == NULL || len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    const char *p = str;
    while (*p) {
        if (!isprint(*p)) {
            return 0;
        }
        p++;
    }
    return 1;
}

void reverse_string(char *str, size_t len) {
    if (str == NULL || len < 2) {
        return;
    }
    char *start = str;
    char *end = str + len - 1;
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void remove_whitespace(char *str) {
    if (str == NULL) {
        return;
    }
    char *read_ptr = str;
    char *write_ptr = str;
    while (*read_ptr) {
        if (!isspace((unsigned char)*read_ptr)) {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

int count_words(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }
    int count = 0;
    int in_word = 0;
    const char *p = str;
    while (*p) {
        if (isspace((unsigned char)*p)) {
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
    char input[MAX_INPUT_LEN];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!is_valid_input(input, len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Original string: \"%s\"\n", input);
    printf("Length: %zu\n", len);
    
    int word_count = count_words(input);
    printf("Word count: %d\n", word_count);
    
    char processed[MAX_INPUT_LEN];
    if (strlen(input) >= sizeof(processed)) {
        fprintf(stderr, "Input too long for processing\n");
        return EXIT_FAILURE;
    }
    strcpy(processed, input);
    
    remove_whitespace(processed);
    printf("Without whitespace: \"%s\"\n", processed);
    
    strcpy(processed, input);
    reverse_string(processed, strlen(processed));
    printf("Reversed: \"%s\"\n", processed);
    
    return EXIT_SUCCESS;
}
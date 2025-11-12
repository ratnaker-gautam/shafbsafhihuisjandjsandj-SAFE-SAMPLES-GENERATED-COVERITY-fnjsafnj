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

void reverse_string(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }
    char *start = str;
    char *end = str;
    while (*end) {
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

void process_string(char *input) {
    if (input == NULL) {
        return;
    }
    char *p = input;
    while (*p) {
        if (islower(*p)) {
            *p = toupper(*p);
        } else if (isupper(*p)) {
            *p = tolower(*p);
        }
        p++;
    }
    reverse_string(input);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    if (!is_valid_input(input, len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    process_string(input);
    printf("Processed string: %s\n", input);
    return 0;
}
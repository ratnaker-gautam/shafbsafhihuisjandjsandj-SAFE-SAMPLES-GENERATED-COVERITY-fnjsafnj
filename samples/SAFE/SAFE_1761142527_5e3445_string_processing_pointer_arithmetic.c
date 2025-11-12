//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024

int validate_input(const char *input) {
    if (input == NULL) return 0;
    const char *p = input;
    while (*p != '\0') {
        if (!isprint(*p)) return 0;
        p++;
    }
    return 1;
}

void reverse_string(char *str) {
    if (str == NULL || *str == '\0') return;
    char *start = str;
    char *end = str;
    while (*(end + 1) != '\0') end++;
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void process_string(char *input) {
    if (input == NULL) return;
    char *p = input;
    while (*p != '\0') {
        if (islower(*p)) *p = toupper(*p);
        else if (isupper(*p)) *p = tolower(*p);
        p++;
    }
    reverse_string(input);
}

int main() {
    char input[MAX_INPUT + 1];
    printf("Enter a string (max %d characters): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    char *newline = strchr(input, '\n');
    if (newline != NULL) *newline = '\0';
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input: contains non-printable characters\n");
        return 1;
    }
    if (strlen(input) == 0) {
        fprintf(stderr, "Error: empty string\n");
        return 1;
    }
    process_string(input);
    printf("Processed string: %s\n", input);
    return 0;
}
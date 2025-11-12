//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

typedef char* (*string_processor)(char*);

char* to_uppercase(char* str) {
    if (str == NULL) return NULL;
    for (char* p = str; *p; p++) {
        *p = toupper((unsigned char)*p);
    }
    return str;
}

char* to_lowercase(char* str) {
    if (str == NULL) return NULL;
    for (char* p = str; *p; p++) {
        *p = tolower((unsigned char)*p);
    }
    return str;
}

char* reverse_string(char* str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
    return str;
}

char* remove_whitespace(char* str) {
    if (str == NULL) return NULL;
    char* dst = str;
    char* src = str;
    while (*src) {
        if (!isspace((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
    return str;
}

void process_string(char* input, string_processor processor) {
    if (input == NULL || processor == NULL) return;
    char* result = processor(input);
    if (result != NULL) {
        printf("Result: %s\n", result);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    const char* names[] = {
        "Uppercase",
        "Lowercase", 
        "Reversed",
        "No whitespace"
    };
    int num_processors = sizeof(processors) / sizeof(processors[0]);

    printf("Enter a string to process: ");
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

    char working_copy[MAX_INPUT_LEN];
    for (int i = 0; i < num_processors; i++) {
        strncpy(working_copy, input, sizeof(working_copy) - 1);
        working_copy[sizeof(working_copy) - 1] = '\0';
        
        printf("\n%s: ", names[i]);
        process_string(working_copy, processors[i]);
    }

    return 0;
}
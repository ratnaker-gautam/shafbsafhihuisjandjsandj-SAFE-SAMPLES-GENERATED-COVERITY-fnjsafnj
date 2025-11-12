//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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
            *dst = *src;
            dst++;
        }
        src++;
    }
    *dst = '\0';
    return str;
}

void process_string(char* input, string_processor processor, char* operation_name) {
    if (input == NULL || processor == NULL || operation_name == NULL) {
        printf("Invalid input parameters\n");
        return;
    }
    
    char buffer[256];
    if (strlen(input) >= sizeof(buffer)) {
        printf("Input string too long\n");
        return;
    }
    
    strcpy(buffer, input);
    char* result = processor(buffer);
    if (result != NULL) {
        printf("%s: %s\n", operation_name, result);
    } else {
        printf("Processing failed\n");
    }
}

int main() {
    char input[256];
    printf("Enter a string to process (max 255 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input string\n");
        return 1;
    }
    
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    char* operation_names[] = {
        "Uppercase",
        "Lowercase",
        "Reversed",
        "No whitespace"
    };
    
    int num_operations = sizeof(processors) / sizeof(processors[0]);
    
    printf("\nProcessing results:\n");
    for (int i = 0; i < num_operations; i++) {
        process_string(input, processors[i], operation_names[i]);
    }
    
    return 0;
}
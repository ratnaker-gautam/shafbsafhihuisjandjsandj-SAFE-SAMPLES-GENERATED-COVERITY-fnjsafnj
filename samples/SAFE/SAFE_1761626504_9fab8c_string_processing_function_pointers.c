//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef char* (*string_processor)(char*);

char* to_uppercase(char* str) {
    if (str == NULL) return NULL;
    char* result = malloc(strlen(str) + 1);
    if (result == NULL) return NULL;
    strcpy(result, str);
    for (char* p = result; *p; p++) {
        *p = toupper((unsigned char)*p);
    }
    return result;
}

char* to_lowercase(char* str) {
    if (str == NULL) return NULL;
    char* result = malloc(strlen(str) + 1);
    if (result == NULL) return NULL;
    strcpy(result, str);
    for (char* p = result; *p; p++) {
        *p = tolower((unsigned char)*p);
    }
    return result;
}

char* reverse_string(char* str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char* result = malloc(len + 1);
    if (result == NULL) return NULL;
    strcpy(result, str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = result[i];
        result[i] = result[len - 1 - i];
        result[len - 1 - i] = temp;
    }
    return result;
}

char* remove_spaces(char* str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char* result = malloc(len + 1);
    if (result == NULL) return NULL;
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (!isspace((unsigned char)str[i])) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';
    return result;
}

void process_string(char* input, string_processor processor) {
    if (input == NULL || processor == NULL) {
        printf("Invalid input or processor\n");
        return;
    }
    char* result = processor(input);
    if (result == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    printf("Result: %s\n", result);
    free(result);
}

int main() {
    char input[256];
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    if (strlen(input) == 0) {
        printf("Empty string entered\n");
        return 1;
    }
    string_processor processors[] = {to_uppercase, to_lowercase, reverse_string, remove_spaces};
    char* names[] = {"Uppercase", "Lowercase", "Reversed", "Spaces removed"};
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    for (int i = 0; i < num_processors; i++) {
        printf("\n%s:\n", names[i]);
        process_string(input, processors[i]);
    }
    return 0;
}
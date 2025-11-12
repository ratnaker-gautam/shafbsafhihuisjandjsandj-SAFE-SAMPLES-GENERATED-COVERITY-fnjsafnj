//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef char* (*string_operation)(char*);

char* to_uppercase(char* str) {
    if (str == NULL) return NULL;
    char* result = malloc(strlen(str) + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        result[i] = toupper((unsigned char)str[i]);
    }
    result[strlen(str)] = '\0';
    return result;
}

char* reverse_string(char* str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char* result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = str[len - 1 - i];
    }
    result[len] = '\0';
    return result;
}

char* remove_vowels(char* str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char* result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    size_t j = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        char c = tolower((unsigned char)str[i]);
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';
    return result;
}

char* process_string(char* input, string_operation operation) {
    if (input == NULL || operation == NULL) return NULL;
    return operation(input);
}

int main() {
    char input[256];
    int choice;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty string provided\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Reverse string\n");
    printf("3. Remove vowels\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    string_operation operations[] = {to_uppercase, reverse_string, remove_vowels};
    const char* operation_names[] = {"Uppercase", "Reversed", "Vowels removed"};
    
    if (choice < 1 || choice > 3) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    char* result = process_string(input, operations[choice - 1]);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("%s: %s\n", operation_names[choice - 1], result);
    
    free(result);
    return 0;
}
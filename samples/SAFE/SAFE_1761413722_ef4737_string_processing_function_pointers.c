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
    char* result = malloc(strlen(str) + 1);
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

void process_string(char* input, string_operation op) {
    if (input == NULL || op == NULL) {
        printf("Invalid input or operation\n");
        return;
    }
    
    char* result = op(input);
    if (result == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    printf("Result: %s\n", result);
    free(result);
}

int main() {
    char input[256];
    string_operation operations[] = {to_uppercase, reverse_string, remove_vowels};
    char* operation_names[] = {"Uppercase", "Reverse", "Remove vowels"};
    int num_operations = 3;
    
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
        printf("Empty string provided\n");
        return 1;
    }
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("Choose operation (1-%d): ", num_operations);
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > num_operations) {
        printf("Invalid choice\n");
        return 1;
    }
    
    process_string(input, operations[choice - 1]);
    
    return 0;
}
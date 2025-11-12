//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

typedef char* (*string_operation)(char*);

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
    for (size_t i = 0; i < len; i++) {
        result[i] = str[len - 1 - i];
    }
    result[len] = '\0';
    return result;
}

char* remove_spaces(char* str) {
    if (str == NULL) return NULL;
    char* result = malloc(strlen(str) + 1);
    if (result == NULL) return NULL;
    char* dest = result;
    for (char* src = str; *src; src++) {
        if (!isspace((unsigned char)*src)) {
            *dest++ = *src;
        }
    }
    *dest = '\0';
    return result;
}

void process_string(char* input, string_operation op) {
    if (input == NULL || op == NULL) return;
    char* result = op(input);
    if (result != NULL) {
        printf("Result: %s\n", result);
        free(result);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    string_operation operations[] = {to_uppercase, to_lowercase, reverse_string, remove_spaces};
    const char* operation_names[] = {"Uppercase", "Lowercase", "Reverse", "Remove spaces"};
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN - 1);
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
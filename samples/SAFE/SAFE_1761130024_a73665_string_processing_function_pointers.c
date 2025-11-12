//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef char* (*string_operation)(char*);

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

char* remove_spaces(char* str) {
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

void process_string(char* input, string_operation op) {
    if (input == NULL || op == NULL) {
        printf("Invalid input or operation\n");
        return;
    }
    char buffer[256];
    if (strlen(input) >= sizeof(buffer)) {
        printf("Input too long\n");
        return;
    }
    strcpy(buffer, input);
    char* result = op(buffer);
    if (result != NULL) {
        printf("Result: %s\n", result);
    }
}

int main() {
    char input[256];
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    string_operation operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_spaces
    };
    
    const char* operation_names[] = {
        "Uppercase",
        "Lowercase", 
        "Reversed",
        "Without spaces"
    };
    
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("Choose operation (1-%d): ", num_operations);
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice < 1 || choice > num_operations) {
        printf("Invalid choice\n");
        return 1;
    }
    
    process_string(input, operations[choice - 1]);
    
    return 0;
}
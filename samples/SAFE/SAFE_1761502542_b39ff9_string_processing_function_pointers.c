//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*string_operation_t)(char *str);

void to_uppercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

void remove_whitespace(char *str) {
    if (str == NULL) return;
    size_t write_pos = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[write_pos++] = str[i];
        }
    }
    str[write_pos] = '\0';
}

int get_valid_choice(void) {
    int choice;
    char buffer[100];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(buffer, "%d", &choice) != 1) {
        return -1;
    }
    
    return choice;
}

void process_string_with_function(char *str, string_operation_t operation) {
    if (str == NULL || operation == NULL) return;
    
    char original[256];
    if (strlen(str) >= sizeof(original)) {
        printf("Input string too long for processing.\n");
        return;
    }
    
    strncpy(original, str, sizeof(original) - 1);
    original[sizeof(original) - 1] = '\0';
    
    operation(str);
    
    printf("Original: \"%s\"\n", original);
    printf("Result:   \"%s\"\n", str);
}

int main(void) {
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    const char *operation_names[] = {
        "Convert to UPPERCASE",
        "Convert to lowercase",
        "Reverse string",
        "Remove whitespace"
    };
    
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    char input[256];
    
    printf("String Processing with Function Pointers\n");
    printf("=======================================\n\n");
    
    while (1) {
        printf("Enter a string (or 'quit' to exit): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            printf("Empty string entered.\n\n");
            continue;
        }
        
        printf("\nAvailable operations:\n");
        for (int i = 0; i < num_operations; i++) {
            printf("%d. %s\n", i + 1, operation_names[i]);
        }
        printf("0. Enter new string\n");
        
        printf("\nSelect operation (1-%d): ", num_operations);
        
        int choice = get_valid_choice();
        
        if (choice == 0) {
            printf("\n");
            continue;
        }
        
        if (choice < 1 || choice > num_operations) {
            printf("Invalid choice. Please try again.\n\n");
            continue;
        }
        
        char working_copy[256];
        if (strlen(input) >= sizeof(working_copy)) {
            printf("String too long for processing.\n\n");
            continue;
        }
        
        strncpy(working_copy, input, sizeof(working_copy) - 1);
        working_copy[sizeof(working_copy) - 1] = '\0';
        
        printf("\n");
        process_string_with_function(working_copy, operations[choice - 1]);
        printf("\n");
    }
    
    printf("Goodbye!\n");
    return 0;
}
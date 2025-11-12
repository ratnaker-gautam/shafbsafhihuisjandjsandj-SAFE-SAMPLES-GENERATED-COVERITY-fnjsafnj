//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef void (*string_operation_t)(char *str, size_t len);

void to_uppercase(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str, size_t len) {
    if (str == NULL || len < 2) return;
    size_t actual_len = strlen(str);
    if (actual_len > len - 1) actual_len = len - 1;
    
    for (size_t i = 0, j = actual_len - 1; i < j; i++, j--) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

void remove_whitespace(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    
    size_t write_pos = 0;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[write_pos++] = str[i];
        }
    }
    if (write_pos < len) {
        str[write_pos] = '\0';
    }
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

void get_input_string(char *buffer, size_t size) {
    if (buffer == NULL || size == 0) return;
    
    printf("Enter a string (max %zu characters): ", size - 1);
    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

int main(void) {
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    const char *operation_names[] = {
        "Convert to uppercase",
        "Convert to lowercase", 
        "Reverse string",
        "Remove whitespace"
    };
    
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    char input[256];
    char working_buffer[256];
    
    while (1) {
        printf("\nString Processing Menu:\n");
        for (int i = 0; i < num_operations; i++) {
            printf("%d. %s\n", i + 1, operation_names[i]);
        }
        printf("0. Exit\n");
        printf("Choose an operation: ");
        
        int choice = get_valid_choice();
        if (choice == -1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (choice == 0) {
            printf("Goodbye!\n");
            break;
        }
        
        if (choice < 1 || choice > num_operations) {
            printf("Invalid choice. Please select 0-%d.\n", num_operations);
            continue;
        }
        
        get_input_string(input, sizeof(input));
        if (strlen(input) == 0) {
            printf("Empty string entered.\n");
            continue;
        }
        
        strncpy(working_buffer, input, sizeof(working_buffer) - 1);
        working_buffer[sizeof(working_buffer) - 1] = '\0';
        
        printf("Original: '%s'\n", input);
        
        operations[choice - 1](working_buffer, sizeof(working_buffer));
        
        printf("Result: '%s'\n", working_buffer);
    }
    
    return 0;
}
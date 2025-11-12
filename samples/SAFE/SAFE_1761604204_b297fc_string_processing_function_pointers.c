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
    if (str == NULL || len == 0) return;
    size_t actual_len = strnlen(str, len);
    for (size_t i = 0; i < actual_len / 2; i++) {
        char temp = str[i];
        str[i] = str[actual_len - 1 - i];
        str[actual_len - 1 - i] = temp;
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
    char buffer[256];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(buffer, "%d", &choice) != 1) {
        return -1;
    }
    
    if (choice < 1 || choice > 4) {
        return -1;
    }
    
    return choice;
}

int get_input_string(char *buffer, size_t size) {
    if (buffer == NULL || size == 0) return 0;
    
    printf("Enter a string (max %zu characters): ", size - 1);
    
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return len > 0 ? 1 : 0;
}

int main(void) {
    char input[256];
    char working_buffer[256];
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
    const int num_operations = 4;
    
    while (1) {
        printf("\nString Processing Menu:\n");
        for (int i = 0; i < num_operations; i++) {
            printf("%d. %s\n", i + 1, operation_names[i]);
        }
        printf("5. Exit\n");
        printf("Choose an operation (1-5): ");
        
        int choice = get_valid_choice();
        if (choice == -1) {
            printf("Invalid choice. Please try again.\n");
            continue;
        }
        
        if (choice == 5) {
            printf("Goodbye!\n");
            break;
        }
        
        if (!get_input_string(input, sizeof(input))) {
            printf("Invalid input or empty string. Please try again.\n");
            continue;
        }
        
        size_t input_len = strlen(input);
        if (input_len >= sizeof(working_buffer)) {
            printf("Input too long. Please try again.\n");
            continue;
        }
        
        strncpy(working_buffer, input, sizeof(working_buffer) - 1);
        working_buffer[sizeof(working_buffer) - 1] = '\0';
        
        operations[choice - 1](working_buffer, sizeof(working_buffer));
        
        printf("Original: %s\n", input);
        printf("Result:   %s\n", working_buffer);
    }
    
    return 0;
}
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

int perform_operation(string_operation_t operation, char *buffer, size_t buffer_size) {
    if (operation == NULL || buffer == NULL || buffer_size == 0) {
        return 0;
    }
    operation(buffer, buffer_size - 1);
    return 1;
}

int main(void) {
    char input_buffer[256];
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
    const size_t num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = strnlen(input_buffer, sizeof(input_buffer));
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty string entered.\n");
        return 1;
    }
    
    printf("\nAvailable operations:\n");
    for (size_t i = 0; i < num_operations; i++) {
        printf("%zu. %s\n", i + 1, operation_names[i]);
    }
    
    printf("Select operation (1-%zu): ", num_operations);
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice < 1 || (size_t)choice > num_operations) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    char working_buffer[256];
    if (strlcpy(working_buffer, input_buffer, sizeof(working_buffer)) >= sizeof(working_buffer)) {
        printf("String too long for processing.\n");
        return 1;
    }
    
    if (!perform_operation(operations[choice - 1], working_buffer, sizeof(working_buffer))) {
        printf("Operation failed.\n");
        return 1;
    }
    
    printf("\nOriginal: %s\n", input_buffer);
    printf("Result: %s\n", working_buffer);
    
    return 0;
}
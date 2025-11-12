//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef void (*string_operation_t)(char *str, size_t len);

void to_uppercase(char *str, size_t len) {
    if (str == NULL) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str, size_t len) {
    if (str == NULL) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    size_t actual_len = 0;
    while (actual_len < len && str[actual_len] != '\0') {
        actual_len++;
    }
    for (size_t i = 0; i < actual_len / 2; i++) {
        char temp = str[i];
        str[i] = str[actual_len - 1 - i];
        str[actual_len - 1 - i] = temp;
    }
}

void remove_non_alnum(char *str, size_t len) {
    if (str == NULL) return;
    size_t write_pos = 0;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        if (isalnum((unsigned char)str[i])) {
            str[write_pos++] = str[i];
        }
    }
    if (write_pos < len) {
        str[write_pos] = '\0';
    }
}

int apply_string_operation(char *input, size_t input_size, string_operation_t operation) {
    if (input == NULL || operation == NULL || input_size == 0) {
        return 0;
    }
    size_t len = strnlen(input, input_size - 1);
    if (len >= input_size - 1) {
        return 0;
    }
    operation(input, input_size);
    return 1;
}

int main(void) {
    char input_buffer[256];
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_non_alnum
    };
    const char *operation_names[] = {
        "Uppercase",
        "Lowercase", 
        "Reverse",
        "Remove non-alphanumeric"
    };
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("Enter a string to process (max 255 characters): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strnlen(input_buffer, sizeof(input_buffer) - 1);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (input_buffer[0] == '\0') {
        printf("Empty input string.\n");
        return 1;
    }
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("Select operation (1-%d): ", num_operations);
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice < 1 || choice > num_operations) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    char working_copy[256];
    if (strlen(input_buffer) >= sizeof(working_copy)) {
        printf("Input too long for processing.\n");
        return 1;
    }
    strncpy(working_copy, input_buffer, sizeof(working_copy) - 1);
    working_copy[sizeof(working_copy) - 1] = '\0';
    
    if (!apply_string_operation(working_copy, sizeof(working_copy), operations[choice - 1])) {
        printf("Operation failed.\n");
        return 1;
    }
    
    printf("Original: %s\n", input_buffer);
    printf("Result: %s\n", working_copy);
    
    return 0;
}
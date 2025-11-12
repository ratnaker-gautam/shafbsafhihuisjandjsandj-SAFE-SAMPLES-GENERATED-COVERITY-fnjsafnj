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
    size_t actual_len = strlen(str);
    if (actual_len > len - 1) actual_len = len - 1;
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

int perform_operation(char *input, size_t buf_size, string_operation_t operation) {
    if (input == NULL || operation == NULL || buf_size == 0) return 0;
    size_t input_len = strlen(input);
    if (input_len >= buf_size) return 0;
    operation(input, buf_size);
    return 1;
}

int main(void) {
    char buffer[256];
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    const char *operation_names[] = {
        "Uppercase",
        "Lowercase", 
        "Reverse",
        "Remove whitespace"
    };
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("Enter a string to process (max 255 characters): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input string.\n");
        return 1;
    }
    
    printf("\nAvailable operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("\nSelect operation (1-%d): ", num_operations);
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
    if (strlen(buffer) >= sizeof(working_copy)) {
        printf("Input too long for processing.\n");
        return 1;
    }
    strncpy(working_copy, buffer, sizeof(working_copy) - 1);
    working_copy[sizeof(working_copy) - 1] = '\0';
    
    if (!perform_operation(working_copy, sizeof(working_copy), operations[choice - 1])) {
        printf("Operation failed.\n");
        return 1;
    }
    
    printf("\nOriginal: %s\n", buffer);
    printf("Result: %s\n", working_copy);
    
    return 0;
}
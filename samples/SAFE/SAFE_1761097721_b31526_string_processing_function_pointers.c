//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: string_processing
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

int perform_operation(string_operation_t operation, char *buffer, size_t buffer_size) {
    if (operation == NULL || buffer == NULL || buffer_size == 0) return 0;
    size_t len = strlen(buffer);
    if (len >= buffer_size) return 0;
    operation(buffer, buffer_size);
    return 1;
}

int main(void) {
    char input_buffer[256];
    string_operation_t operations[] = {to_uppercase, to_lowercase, reverse_string, remove_whitespace};
    const char *operation_names[] = {"Uppercase", "Lowercase", "Reverse", "Remove whitespace"};
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("Enter a string to process (max 255 characters): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
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
        printf("Invalid operation selection.\n");
        return 1;
    }
    
    char working_buffer[256];
    if (strlen(input_buffer) >= sizeof(working_buffer)) {
        printf("Input too long for processing.\n");
        return 1;
    }
    strncpy(working_buffer, input_buffer, sizeof(working_buffer) - 1);
    working_buffer[sizeof(working_buffer) - 1] = '\0';
    
    printf("\nOriginal: %s\n", input_buffer);
    
    if (perform_operation(operations[choice - 1], working_buffer, sizeof(working_buffer))) {
        printf("Result: %s\n", working_buffer);
    } else {
        printf("Operation failed.\n");
        return 1;
    }
    
    return 0;
}
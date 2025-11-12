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

int get_user_input(char *buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) return 0;
    printf("Enter a string (max %zu characters): ", buffer_size - 1);
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) return 0;
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    return 1;
}

void apply_operation(char *str, size_t len, string_operation_t operation) {
    if (str == NULL || len == 0 || operation == NULL) return;
    operation(str, len);
}

int main(void) {
    char input[256];
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
    
    if (!get_user_input(input, sizeof(input))) {
        printf("Error reading input.\n");
        return 1;
    }
    
    printf("\nOriginal string: \"%s\"\n", input);
    printf("\nAvailable operations:\n");
    for (size_t i = 0; i < num_operations; i++) {
        printf("%zu. %s\n", i + 1, operation_names[i]);
    }
    
    printf("\nSelect operation (1-%zu): ", num_operations);
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice < 1 || (size_t)choice > num_operations) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    char working_copy[256];
    size_t copy_size = sizeof(working_copy);
    if (strlen(input) >= copy_size) {
        printf("Input too long for processing.\n");
        return 1;
    }
    strncpy(working_copy, input, copy_size - 1);
    working_copy[copy_size - 1] = '\0';
    
    apply_operation(working_copy, sizeof(working_copy), operations[choice - 1]);
    
    printf("\nResult: \"%s\"\n", working_copy);
    
    return 0;
}
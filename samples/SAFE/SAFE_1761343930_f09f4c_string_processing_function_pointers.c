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

void remove_whitespace(char *str, size_t len) {
    if (str == NULL) return;
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
    char work_buffer[256];
    int choice;
    
    printf("Enter a string (max 255 characters): ");
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
        printf("Empty string entered.\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Remove whitespace\n");
    printf("Enter choice (1-4): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    if (choice < 1 || choice > 4) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    if (strlen(input_buffer) >= sizeof(work_buffer)) {
        printf("Input too long for processing.\n");
        return 1;
    }
    
    strncpy(work_buffer, input_buffer, sizeof(work_buffer) - 1);
    work_buffer[sizeof(work_buffer) - 1] = '\0';
    
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    if (!perform_operation(operations[choice - 1], work_buffer, sizeof(work_buffer))) {
        printf("Operation failed.\n");
        return 1;
    }
    
    printf("Original: %s\n", input_buffer);
    printf("Result: %s\n", work_buffer);
    
    return 0;
}
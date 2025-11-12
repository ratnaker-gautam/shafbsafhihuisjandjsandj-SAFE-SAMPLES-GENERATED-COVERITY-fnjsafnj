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

void remove_non_alpha(char *str, size_t len) {
    if (str == NULL) return;
    size_t write_pos = 0;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        if (isalpha((unsigned char)str[i])) {
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
    
    if (input_len >= sizeof(work_buffer)) {
        printf("Input too long for processing.\n");
        return 1;
    }
    
    strncpy(work_buffer, input_buffer, sizeof(work_buffer) - 1);
    work_buffer[sizeof(work_buffer) - 1] = '\0';
    
    printf("Original: %s\n", input_buffer);
    
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_non_alpha
    };
    
    const char *operation_names[] = {
        "Uppercase",
        "Lowercase", 
        "Reversed",
        "Alpha-only"
    };
    
    size_t num_operations = sizeof(operations) / sizeof(operations[0]);
    
    for (size_t i = 0; i < num_operations; i++) {
        char temp_buffer[256];
        strncpy(temp_buffer, input_buffer, sizeof(temp_buffer) - 1);
        temp_buffer[sizeof(temp_buffer) - 1] = '\0';
        
        if (perform_operation(operations[i], temp_buffer, sizeof(temp_buffer))) {
            printf("%s: %s\n", operation_names[i], temp_buffer);
        } else {
            printf("Failed to perform %s operation.\n", operation_names[i]);
        }
    }
    
    return 0;
}
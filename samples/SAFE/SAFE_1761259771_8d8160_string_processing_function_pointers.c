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

int apply_string_operation(char *input, size_t input_len, string_operation_t operation) {
    if (input == NULL || operation == NULL || input_len == 0) {
        return 0;
    }
    if (input_len > 1024) {
        return 0;
    }
    operation(input, input_len);
    return 1;
}

int main(void) {
    char buffer[1024];
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
    const size_t num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("Enter a string (max 1023 characters): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string entered.\n");
        return 1;
    }
    
    printf("\nOriginal: '%s'\n\n", buffer);
    
    for (size_t i = 0; i < num_operations; i++) {
        char working_copy[1024];
        if (strlen(buffer) >= sizeof(working_copy)) {
            printf("String too long for processing.\n");
            return 1;
        }
        strcpy(working_copy, buffer);
        
        if (apply_string_operation(working_copy, sizeof(working_copy), operations[i])) {
            printf("%s: '%s'\n", operation_names[i], working_copy);
        } else {
            printf("Failed to apply %s operation.\n", operation_names[i]);
        }
    }
    
    return 0;
}
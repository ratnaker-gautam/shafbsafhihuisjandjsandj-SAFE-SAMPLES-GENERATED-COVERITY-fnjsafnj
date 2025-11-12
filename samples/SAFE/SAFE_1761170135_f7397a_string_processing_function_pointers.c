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
    size_t i = 0;
    size_t j = len - 1;
    while (i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

void remove_whitespace(char *str, size_t len) {
    if (str == NULL) return;
    size_t write_pos = 0;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[write_pos] = str[i];
            write_pos++;
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
    if (str == NULL || operation == NULL) return;
    operation(str, len);
}

int main(void) {
    const size_t BUFFER_SIZE = 256;
    char input_buffer[BUFFER_SIZE];
    char working_buffer[BUFFER_SIZE];
    
    if (!get_user_input(input_buffer, BUFFER_SIZE)) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len >= BUFFER_SIZE) {
        printf("Input too long.\n");
        return 1;
    }
    
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    const char *operation_names[] = {
        "Uppercase",
        "Lowercase", 
        "Reversed",
        "Without whitespace"
    };
    
    const size_t num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("\nOriginal: %s\n", input_buffer);
    printf("Processing results:\n");
    
    for (size_t i = 0; i < num_operations; i++) {
        strncpy(working_buffer, input_buffer, BUFFER_SIZE - 1);
        working_buffer[BUFFER_SIZE - 1] = '\0';
        
        apply_operation(working_buffer, strlen(working_buffer), operations[i]);
        
        printf("%s: %s\n", operation_names[i], working_buffer);
    }
    
    return 0;
}
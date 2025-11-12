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

int safe_strcpy(char *dest, size_t dest_size, const char *src, size_t src_len) {
    if (dest == NULL || src == NULL || dest_size == 0) return -1;
    size_t copy_len = src_len;
    if (copy_len >= dest_size) copy_len = dest_size - 1;
    for (size_t i = 0; i < copy_len; i++) {
        dest[i] = src[i];
        if (src[i] == '\0') break;
    }
    dest[copy_len] = '\0';
    return 0;
}

void process_string(char *input, size_t input_len, string_operation_t operation) {
    if (input == NULL || operation == NULL || input_len == 0) return;
    operation(input, input_len);
}

int main() {
    char input_buffer[256];
    char work_buffer[256];
    int choice;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strnlen(input_buffer, sizeof(input_buffer));
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty string entered\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Remove whitespace\n");
    printf("Enter choice (1-4): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    string_operation_t operation = NULL;
    
    switch (choice) {
        case 1: operation = to_uppercase; break;
        case 2: operation = to_lowercase; break;
        case 3: operation = reverse_string; break;
        case 4: operation = remove_whitespace; break;
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    if (safe_strcpy(work_buffer, sizeof(work_buffer), input_buffer, input_len) != 0) {
        printf("Error copying string\n");
        return 1;
    }
    
    printf("Original: %s\n", input_buffer);
    process_string(work_buffer, sizeof(work_buffer), operation);
    printf("Result: %s\n", work_buffer);
    
    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*string_operation)(char *str, int len);

void to_uppercase(char *str, int len) {
    if (str == NULL || len <= 0) return;
    for (int i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str, int len) {
    if (str == NULL || len <= 0) return;
    for (int i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str, int len) {
    if (str == NULL || len <= 0) return;
    int actual_len = 0;
    while (actual_len < len && str[actual_len] != '\0') {
        actual_len++;
    }
    for (int i = 0; i < actual_len / 2; i++) {
        char temp = str[i];
        str[i] = str[actual_len - 1 - i];
        str[actual_len - 1 - i] = temp;
    }
}

void remove_whitespace(char *str, int len) {
    if (str == NULL || len <= 0) return;
    int write_pos = 0;
    for (int i = 0; i < len && str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[write_pos++] = str[i];
        }
    }
    if (write_pos < len) {
        str[write_pos] = '\0';
    }
}

int get_user_input(char *buffer, int max_len) {
    if (buffer == NULL || max_len <= 1) return 0;
    printf("Enter a string (max %d characters): ", max_len - 1);
    if (fgets(buffer, max_len, stdin) == NULL) return 0;
    size_t input_len = strlen(buffer);
    if (input_len > 0 && buffer[input_len - 1] == '\n') {
        buffer[input_len - 1] = '\0';
    }
    return 1;
}

void apply_operation(char *str, int len, string_operation op) {
    if (str == NULL || len <= 0 || op == NULL) return;
    op(str, len);
}

int main(void) {
    const int BUFFER_SIZE = 256;
    char input_buffer[BUFFER_SIZE];
    char working_buffer[BUFFER_SIZE];
    
    if (!get_user_input(input_buffer, BUFFER_SIZE)) {
        printf("Error reading input.\n");
        return 1;
    }
    
    if (strlen(input_buffer) == 0) {
        printf("Empty string provided.\n");
        return 1;
    }
    
    string_operation operations[] = {
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
    
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("\nOriginal string: \"%s\"\n", input_buffer);
    printf("\nProcessing results:\n");
    
    for (int i = 0; i < num_operations; i++) {
        strncpy(working_buffer, input_buffer, BUFFER_SIZE - 1);
        working_buffer[BUFFER_SIZE - 1] = '\0';
        
        apply_operation(working_buffer, BUFFER_SIZE, operations[i]);
        
        printf("%s: \"%s\"\n", operation_names[i], working_buffer);
    }
    
    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*string_operation_t)(char *str, int len);

void to_uppercase(char *str, int len) {
    if (str == NULL) return;
    for (int i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str, int len) {
    if (str == NULL) return;
    for (int i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str, int len) {
    if (str == NULL) return;
    int actual_len = 0;
    while (actual_len < len && str[actual_len] != '\0') {
        actual_len++;
    }
    for (int i = 0; i < actual_len / 2; i++) {
        char temp = str[i];
        str[i] = str[actual_len - i - 1];
        str[actual_len - i - 1] = temp;
    }
}

void remove_whitespace(char *str, int len) {
    if (str == NULL) return;
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

int get_valid_choice(void) {
    int choice;
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    if (sscanf(buffer, "%d", &choice) != 1) {
        return -1;
    }
    return choice;
}

void get_input_string(char *buffer, int max_len) {
    if (fgets(buffer, max_len, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

int main(void) {
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
    
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    char input[256];
    char working_copy[256];
    
    printf("Available string operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("Select operation (1-%d): ", num_operations);
    int choice = get_valid_choice();
    
    if (choice < 1 || choice > num_operations) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    printf("Enter a string (max 255 characters): ");
    get_input_string(input, sizeof(input));
    
    if (input[0] == '\0') {
        printf("Empty string provided.\n");
        return 1;
    }
    
    strncpy(working_copy, input, sizeof(working_copy) - 1);
    working_copy[sizeof(working_copy) - 1] = '\0';
    
    string_operation_t selected_op = operations[choice - 1];
    selected_op(working_copy, sizeof(working_copy));
    
    printf("Original: \"%s\"\n", input);
    printf("Result:   \"%s\"\n", working_copy);
    
    return 0;
}
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

int get_valid_choice(void) {
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    char *endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n' || choice < 1 || choice > 5) {
        return -1;
    }
    return (int)choice;
}

void process_string_with_function(string_operation op, char *str, int max_len) {
    if (op == NULL || str == NULL || max_len <= 0) return;
    op(str, max_len);
}

int main(void) {
    string_operation operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    const char *operation_names[] = {
        "Convert to uppercase",
        "Convert to lowercase", 
        "Reverse string",
        "Remove whitespace",
        "Exit"
    };
    
    char input_buffer[256];
    
    printf("Enter a string to process (max 255 characters): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
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
    
    while (1) {
        printf("\nAvailable operations:\n");
        for (int i = 0; i < 5; i++) {
            printf("%d. %s\n", i + 1, operation_names[i]);
        }
        printf("Select operation (1-5): ");
        
        int choice = get_valid_choice();
        if (choice == -1) {
            printf("Invalid choice. Please enter a number between 1 and 5.\n");
            continue;
        }
        
        if (choice == 5) {
            printf("Exiting.\n");
            break;
        }
        
        char working_copy[256];
        if (strlen(input_buffer) >= sizeof(working_copy)) {
            printf("String too long for processing.\n");
            return 1;
        }
        strcpy(working_copy, input_buffer);
        
        process_string_with_function(operations[choice - 1], working_copy, sizeof(working_copy));
        
        printf("Original: %s\n", input_buffer);
        printf("Result:   %s\n", working_copy);
        
        printf("\nProcess another operation on the same string? (y/n): ");
        char response[16];
        if (fgets(response, sizeof(response), stdin) == NULL) {
            break;
        }
        if (response[0] != 'y' && response[0] != 'Y') {
            break;
        }
    }
    
    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*string_operation_t)(char *str);

void to_uppercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

void remove_whitespace(char *str) {
    if (str == NULL) return;
    size_t read_pos = 0;
    size_t write_pos = 0;
    while (str[read_pos] != '\0') {
        if (!isspace((unsigned char)str[read_pos])) {
            str[write_pos] = str[read_pos];
            write_pos++;
        }
        read_pos++;
    }
    str[write_pos] = '\0';
}

int get_valid_choice(void) {
    int choice;
    char input[100];
    
    while (1) {
        printf("Choose operation (1-5):\n");
        printf("1. Convert to uppercase\n");
        printf("2. Convert to lowercase\n");
        printf("3. Reverse string\n");
        printf("4. Remove whitespace\n");
        printf("5. Exit\n");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return 5;
        }
        
        if (sscanf(input, "%d", &choice) == 1 && choice >= 1 && choice <= 5) {
            return choice;
        }
        
        printf("Invalid choice. Please enter 1-5.\n");
    }
}

int main(void) {
    string_operation_t operations[] = {
        NULL,
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    char input_string[1024];
    
    while (1) {
        printf("Enter a string (or 'quit' to exit): ");
        if (fgets(input_string, sizeof(input_string), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_string);
        if (len > 0 && input_string[len - 1] == '\n') {
            input_string[len - 1] = '\0';
        }
        
        if (strcmp(input_string, "quit") == 0) {
            break;
        }
        
        if (strlen(input_string) == 0) {
            printf("Empty string entered.\n");
            continue;
        }
        
        int choice = get_valid_choice();
        if (choice == 5) {
            break;
        }
        
        char working_string[1024];
        strncpy(working_string, input_string, sizeof(working_string) - 1);
        working_string[sizeof(working_string) - 1] = '\0';
        
        operations[choice](working_string);
        
        printf("Original: %s\n", input_string);
        printf("Result:   %s\n\n", working_string);
    }
    
    return 0;
}
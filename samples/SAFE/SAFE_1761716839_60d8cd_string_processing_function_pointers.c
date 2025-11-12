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
    char input[16];
    
    while (1) {
        printf("Enter operation (1-4): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(input, "%d", &choice) == 1 && choice >= 1 && choice <= 4) {
            return choice;
        }
        printf("Invalid choice. Please enter 1-4.\n");
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
    
    char input[256];
    
    printf("Available string operations:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    int choice = get_valid_choice();
    if (choice == -1) {
        return 1;
    }
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty string provided.\n");
        return 1;
    }
    
    char working_copy[256];
    if (strlen(input) >= sizeof(working_copy)) {
        printf("Input too long.\n");
        return 1;
    }
    strcpy(working_copy, input);
    
    operations[choice - 1](working_copy);
    
    printf("Original: '%s'\n", input);
    printf("Result:   '%s'\n", working_copy);
    
    return 0;
}
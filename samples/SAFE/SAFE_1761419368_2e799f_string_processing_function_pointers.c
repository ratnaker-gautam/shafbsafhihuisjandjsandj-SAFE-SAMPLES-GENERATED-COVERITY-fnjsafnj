//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*string_operation_t)(char *);

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
    size_t j = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

int get_valid_choice(void) {
    int choice;
    char input[16];
    
    while (1) {
        printf("Choose operation (1-4): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(input, "%d", &choice) == 1 && choice >= 1 && choice <= 4) {
            return choice;
        }
        printf("Invalid choice. Please enter 1-4.\n");
    }
}

int get_input_string(char *buffer, size_t size) {
    printf("Enter a string (max %zu chars): ", size - 1);
    if (fgets(buffer, size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (len == 1 && buffer[0] == '\0') {
        printf("Empty string not allowed.\n");
        return 0;
    }
    
    return 1;
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
    char working_copy[256];
    
    printf("String Processing Tool\n");
    printf("Available operations:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    int choice = get_valid_choice();
    if (choice == -1) {
        return 1;
    }
    
    if (!get_input_string(input, sizeof(input))) {
        return 1;
    }
    
    if (strlen(input) >= sizeof(working_copy)) {
        printf("Input string too long.\n");
        return 1;
    }
    
    strcpy(working_copy, input);
    
    printf("\nOriginal: '%s'\n", input);
    
    operations[choice - 1](working_copy);
    
    printf("Result: '%s'\n", working_copy);
    
    return 0;
}
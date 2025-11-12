//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef void (*string_processor)(char *str);

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
    size_t write_pos = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[write_pos++] = str[i];
        }
    }
    str[write_pos] = '\0';
}

int get_valid_choice(void) {
    int choice;
    char buffer[100];
    
    while (1) {
        printf("Choose operation (1-5):\n");
        printf("1. Convert to uppercase\n");
        printf("2. Convert to lowercase\n");
        printf("3. Reverse string\n");
        printf("4. Remove whitespace\n");
        printf("5. Exit\n");
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 5;
        }
        
        if (sscanf(buffer, "%d", &choice) == 1 && choice >= 1 && choice <= 5) {
            return choice;
        }
        printf("Invalid choice. Please enter 1-5.\n");
    }
}

int get_input_string(char *buffer, size_t size) {
    printf("Enter a string (max %zu characters): ", size - 1);
    
    if (fgets(buffer, size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && buffer[0] == '\0')) {
        printf("Empty string not allowed.\n");
        return 0;
    }
    
    return 1;
}

int main(void) {
    char input[256];
    string_processor processors[] = {
        NULL,
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    printf("String Processing with Function Pointers\n");
    printf("=======================================\n");
    
    while (1) {
        int choice = get_valid_choice();
        
        if (choice == 5) {
            printf("Goodbye!\n");
            break;
        }
        
        if (!get_input_string(input, sizeof(input))) {
            continue;
        }
        
        char original[256];
        strncpy(original, input, sizeof(original) - 1);
        original[sizeof(original) - 1] = '\0';
        
        processors[choice](input);
        
        printf("Original: '%s'\n", original);
        printf("Result:   '%s'\n", input);
        printf("\n");
    }
    
    return 0;
}
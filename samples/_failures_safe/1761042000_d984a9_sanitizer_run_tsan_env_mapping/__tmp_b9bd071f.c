//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: string_processing
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
    char buffer[100];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(buffer, "%d", &choice) != 1) {
        return -1;
    }
    
    if (choice < 1 || choice > 5) {
        return -1;
    }
    
    return choice;
}

int get_input_string(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else if (len == size - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return 0;
    }
    
    return 1;
}

int main(void) {
    string_operation_t operations[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace,
        NULL
    };
    
    const char *operation_names[] = {
        "Convert to uppercase",
        "Convert to lowercase", 
        "Reverse string",
        "Remove whitespace",
        "Exit"
    };
    
    char input[256];
    char working_copy[256];
    
    printf("String Processing Tool\n");
    printf("=====================\n");
    
    while (1) {
        printf("\nAvailable operations:\n");
        for (int i = 0; i < 5; i++) {
            printf("%d. %s\n", i + 1, operation_names[i]);
        }
        
        printf("\nEnter your choice (1-5): ");
        int choice = get_valid_choice();
        
        if (choice == -1) {
            printf("Invalid choice. Please enter a number between 1 and 5.\n");
            continue;
        }
        
        if (choice == 5) {
            printf("Goodbye!\n");
            break;
        }
        
        printf("Enter a string (max 255 characters): ");
        if (!get_input_string(input, sizeof(input))) {
            printf("Invalid input or input too long.\n");
            continue;
        }
        
        if (strlen(input) == 0) {
            printf("Empty string provided.\n");
            continue;
        }
        
        strncpy(working_copy, input, sizeof(working_copy) - 1);
        working_copy[sizeof(working_copy) - 1] = '\0';
        
        operations[choice - 1](working_copy);
        
        printf("\nOriginal: \"%s\"\n", input);
        printf("Result:   \"%s\"\n", working_copy);
    }
    
    return 0;
}
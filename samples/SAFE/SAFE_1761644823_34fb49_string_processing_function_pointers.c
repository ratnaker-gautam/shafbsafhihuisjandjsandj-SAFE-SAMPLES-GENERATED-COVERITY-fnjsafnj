//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*string_processor)(char *);

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
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
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
    char buffer[100];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(buffer, "%d", &choice) != 1) {
        return -1;
    }
    
    return choice;
}

void process_string_with_function(string_processor processor, char *str) {
    if (processor == NULL || str == NULL) return;
    
    char temp[256];
    if (strlen(str) >= sizeof(temp)) {
        printf("Input string too long for processing.\n");
        return;
    }
    
    strcpy(temp, str);
    processor(temp);
    printf("Result: %s\n", temp);
}

int main(void) {
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    
    const char *names[] = {
        "Convert to UPPERCASE",
        "Convert to lowercase",
        "Reverse string",
        "Remove whitespace"
    };
    
    const int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    char input[256];
    
    printf("String Processing Menu:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    printf("0. Exit\n");
    
    while (1) {
        printf("\nEnter your choice (0-%d): ", num_processors);
        int choice = get_valid_choice();
        
        if (choice == 0) {
            printf("Goodbye!\n");
            break;
        }
        
        if (choice < 1 || choice > num_processors) {
            printf("Invalid choice. Please try again.\n");
            continue;
        }
        
        printf("Enter a string to process: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            printf("Empty string provided.\n");
            continue;
        }
        
        process_string_with_function(processors[choice - 1], input);
    }
    
    return 0;
}
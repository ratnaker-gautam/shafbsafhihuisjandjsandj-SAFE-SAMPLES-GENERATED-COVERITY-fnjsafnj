//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*StringProcessor)(char *);

void to_uppercase(char *str) {
    if (str == NULL) return;
    for (char *p = str; *p; p++) {
        *p = toupper((unsigned char)*p);
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (char *p = str; *p; p++) {
        *p = tolower((unsigned char)*p);
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
    char *dst = str;
    for (char *src = str; *src; src++) {
        if (!isspace((unsigned char)*src)) {
            *dst++ = *src;
        }
    }
    *dst = '\0';
}

int get_valid_choice(void) {
    int choice;
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
    if (sscanf(buffer, "%d", &choice) != 1) return -1;
    return choice;
}

int main(void) {
    StringProcessor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_whitespace
    };
    const char *names[] = {
        "Convert to uppercase",
        "Convert to lowercase", 
        "Reverse string",
        "Remove whitespace"
    };
    const int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    char input[256];
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty string provided\n");
        return 1;
    }
    
    printf("\nAvailable string processors:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    
    printf("Select operation (1-%d): ", num_processors);
    int choice = get_valid_choice();
    
    if (choice < 1 || choice > num_processors) {
        printf("Invalid choice\n");
        return 1;
    }
    
    char working_copy[256];
    if (strlen(input) >= sizeof(working_copy)) {
        printf("Input too long for processing\n");
        return 1;
    }
    strcpy(working_copy, input);
    
    processors[choice - 1](working_copy);
    
    printf("Original: \"%s\"\n", input);
    printf("Result:   \"%s\"\n", working_copy);
    
    return 0;
}
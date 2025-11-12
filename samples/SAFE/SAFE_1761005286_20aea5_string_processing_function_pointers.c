//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef void (*string_processor)(char *);

void to_uppercase(char *str) {
    if (str == NULL) return;
    for (char *p = str; *p != '\0'; p++) {
        *p = toupper((unsigned char)*p);
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (char *p = str; *p != '\0'; p++) {
        *p = tolower((unsigned char)*p);
    }
}

void reverse_string(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    if (len == 0) return;
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

void remove_non_alnum(char *str) {
    if (str == NULL) return;
    char *read = str;
    char *write = str;
    while (*read != '\0') {
        if (isalnum((unsigned char)*read)) {
            *write = *read;
            write++;
        }
        read++;
    }
    *write = '\0';
}

int get_valid_choice(void) {
    int choice;
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    if (sscanf(input, "%d", &choice) != 1) {
        return -1;
    }
    return choice;
}

void process_string_with_function(char *str, string_processor processor) {
    if (str == NULL || processor == NULL) return;
    char buffer[256];
    if (strlen(str) >= sizeof(buffer)) {
        printf("Input string too long.\n");
        return;
    }
    strcpy(buffer, str);
    processor(buffer);
    printf("Result: %s\n", buffer);
}

int main(void) {
    char input[256];
    string_processor processors[] = {
        to_uppercase,
        to_lowercase,
        reverse_string,
        remove_non_alnum
    };
    const char *names[] = {
        "Convert to uppercase",
        "Convert to lowercase",
        "Reverse string",
        "Remove non-alphanumeric characters"
    };
    int num_processors = sizeof(processors) / sizeof(processors[0]);

    printf("Enter a string to process: ");
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

    printf("Available operations:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    printf("Select operation (1-%d): ", num_processors);

    int choice = get_valid_choice();
    if (choice < 1 || choice > num_processors) {
        printf("Invalid choice.\n");
        return 1;
    }

    process_string_with_function(input, processors[choice - 1]);
    return 0;
}
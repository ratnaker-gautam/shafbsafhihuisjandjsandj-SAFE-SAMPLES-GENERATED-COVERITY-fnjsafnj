//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READING_NAME,
    READING_AGE,
    PROCESSING,
    DONE,
    ERROR
};

struct person {
    char name[32];
    int age;
};

int read_line(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return -1;
    }
    
    return 0;
}

int validate_name(const char *name) {
    if (name == NULL || strlen(name) == 0 || strlen(name) >= 32) {
        return 0;
    }
    
    for (size_t i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            return 0;
        }
    }
    
    return 1;
}

int validate_age(const char *input) {
    if (input == NULL || strlen(input) == 0) {
        return 0;
    }
    
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (!isdigit(input[i])) {
            return 0;
        }
    }
    
    char *endptr;
    long age = strtol(input, &endptr, 10);
    if (*endptr != '\0' || age < 1 || age > 150) {
        return 0;
    }
    
    return 1;
}

int main(void) {
    enum state current_state = INIT;
    struct person p;
    char input[64];
    int result;
    
    printf("Person Data Entry System\n");
    printf("Enter 'quit' at any time to exit\n\n");
    
    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Enter person's name: ");
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (validate_name(input)) {
                    strncpy(p.name, input, sizeof(p.name) - 1);
                    p.name[sizeof(p.name) - 1] = '\0';
                    current_state = READING_AGE;
                } else {
                    printf("Invalid name. Use only letters and spaces, max 31 characters.\n");
                }
                break;
                
            case READING_AGE:
                printf("Enter person's age: ");
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (validate_age(input)) {
                    p.age = atoi(input);
                    current_state = PROCESSING;
                } else {
                    printf("Invalid age. Use numbers only, between 1 and 150.\n");
                }
                break;
                
            case PROCESSING:
                printf("\nProcessing complete:\n");
                printf("Name: %s\n", p.name);
                printf("Age: %d\n", p.age);
                
                if (p.age >= 18) {
                    printf("Status: Adult\n");
                } else {
                    printf("Status: Minor\n");
                }
                
                printf("\nEnter another person? (y/n): ");
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (input[0] == 'y' || input[0] == 'Y') {
                    current_state = INIT;
                } else if (input[0] == 'n' || input[0] == 'N') {
                    current_state = DONE;
                } else {
                    printf("Invalid choice. Please enter 'y' or 'n'.\n");
                }
                break;
                
            case ERROR:
                printf("An error occurred during input.\n");
                current_state = DONE;
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}
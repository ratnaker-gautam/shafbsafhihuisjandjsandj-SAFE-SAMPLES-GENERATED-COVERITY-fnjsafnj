//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NAME,
    READING_AGE,
    CONFIRMING,
    DONE,
    ERROR
};

struct Person {
    char name[64];
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

int is_valid_name(const char *name) {
    if (name == NULL || strlen(name) == 0 || strlen(name) >= 63) {
        return 0;
    }
    
    for (size_t i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ' && name[i] != '-') {
            return 0;
        }
    }
    
    return 1;
}

int is_valid_age(const char *input) {
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
    enum State current_state = INIT;
    struct Person person;
    char input[256];
    int running = 1;
    
    printf("Person Registration System\n");
    printf("Enter 'quit' at any time to exit\n\n");
    
    while (running) {
        switch (current_state) {
            case INIT:
                printf("Please enter your name: ");
                current_state = READING_NAME;
                break;
                
            case READING_NAME:
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    running = 0;
                    break;
                }
                
                if (!is_valid_name(input)) {
                    printf("Invalid name. Please use only letters, spaces, and hyphens.\n");
                    current_state = READING_NAME;
                    break;
                }
                
                strncpy(person.name, input, sizeof(person.name) - 1);
                person.name[sizeof(person.name) - 1] = '\0';
                current_state = READING_AGE;
                break;
                
            case READING_AGE:
                printf("Please enter your age: ");
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    running = 0;
                    break;
                }
                
                if (!is_valid_age(input)) {
                    printf("Invalid age. Please enter a number between 1 and 150.\n");
                    current_state = READING_AGE;
                    break;
                }
                
                person.age = atoi(input);
                current_state = CONFIRMING;
                break;
                
            case CONFIRMING:
                printf("\nPlease confirm the following information:\n");
                printf("Name: %s\n", person.name);
                printf("Age: %d\n", person.age);
                printf("Is this correct? (yes/no): ");
                
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    running = 0;
                    break;
                }
                
                if (strcmp(input, "yes") == 0) {
                    printf("\nRegistration completed successfully!\n");
                    printf("Welcome, %s (age %d)!\n\n", person.name, person.age);
                    current_state = DONE;
                } else if (strcmp(input, "no") == 0) {
                    printf("Let's start over.\n\n");
                    current_state = INIT;
                } else {
                    printf("Please answer 'yes' or 'no'.\n");
                    current_state = CONFIRMING;
                }
                break;
                
            case DONE:
                printf("Would you like to register another person? (yes/no): ");
                
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    running = 0;
                    break;
                }
                
                if (strcmp(input, "yes") == 0) {
                    printf("\n");
                    current_state = INIT;
                } else if (strcmp(input, "no") == 0) {
                    running = 0;
                } else {
                    printf("Please answer 'yes' or 'no'.\n");
                    current_state = DONE;
                }
                break;
                
            case ERROR:
                printf("An error occurred. Please try again.\n");
                current_state = INIT;
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}
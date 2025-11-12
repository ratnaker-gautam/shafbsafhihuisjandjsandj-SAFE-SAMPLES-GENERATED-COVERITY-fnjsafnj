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
    
    printf("Person Registration System\n");
    printf("Enter 'quit' at any time to exit\n\n");
    
    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Please enter the person's name: ");
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (is_valid_name(input)) {
                    strncpy(person.name, input, sizeof(person.name) - 1);
                    person.name[sizeof(person.name) - 1] = '\0';
                    current_state = READING_AGE;
                } else {
                    printf("Invalid name. Names can only contain letters, spaces, and hyphens.\n");
                }
                break;
                
            case READING_AGE:
                printf("Please enter the person's age: ");
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (is_valid_age(input)) {
                    char *endptr;
                    person.age = (int)strtol(input, &endptr, 10);
                    current_state = CONFIRMING;
                } else {
                    printf("Invalid age. Please enter a number between 1 and 150.\n");
                }
                break;
                
            case CONFIRMING:
                printf("\nRegistration Summary:\n");
                printf("Name: %s\n", person.name);
                printf("Age: %d\n", person.age);
                printf("Is this correct? (yes/no): ");
                
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (strcmp(input, "yes") == 0) {
                    printf("\nRegistration completed successfully!\n");
                    printf("Thank you for registering: %s, age %d\n", person.name, person.age);
                    current_state = DONE;
                } else if (strcmp(input, "no") == 0) {
                    printf("Let's start over...\n\n");
                    current_state = INIT;
                } else {
                    printf("Please answer 'yes' or 'no'.\n");
                }
                break;
                
            case ERROR:
                printf("An error occurred. Exiting.\n");
                current_state = DONE;
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    return 0;
}
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
    
    long age = strtol(input, NULL, 10);
    if (age < 1 || age > 150) {
        return 0;
    }
    
    return 1;
}

int main(void) {
    enum State current_state = INIT;
    struct Person person;
    char input[256];
    int running = 1;
    
    memset(&person, 0, sizeof(person));
    
    while (running) {
        switch (current_state) {
            case INIT:
                printf("Welcome to the person registration system.\n");
                printf("Please enter the person's name: ");
                current_state = READING_NAME;
                break;
                
            case READING_NAME:
                if (read_line(input, sizeof(input)) != 0) {
                    printf("Error reading input.\n");
                    current_state = ERROR;
                    break;
                }
                
                if (!is_valid_name(input)) {
                    printf("Invalid name. Please use only letters, spaces, and hyphens: ");
                    break;
                }
                
                strncpy(person.name, input, sizeof(person.name) - 1);
                person.name[sizeof(person.name) - 1] = '\0';
                printf("Please enter the person's age: ");
                current_state = READING_AGE;
                break;
                
            case READING_AGE:
                if (read_line(input, sizeof(input)) != 0) {
                    printf("Error reading input.\n");
                    current_state = ERROR;
                    break;
                }
                
                if (!is_valid_age(input)) {
                    printf("Invalid age. Please enter a number between 1 and 150: ");
                    break;
                }
                
                person.age = (int)strtol(input, NULL, 10);
                printf("Name: %s, Age: %d\n", person.name, person.age);
                printf("Is this correct? (yes/no): ");
                current_state = CONFIRMING;
                break;
                
            case CONFIRMING:
                if (read_line(input, sizeof(input)) != 0) {
                    printf("Error reading input.\n");
                    current_state = ERROR;
                    break;
                }
                
                for (size_t i = 0; input[i] != '\0'; i++) {
                    input[i] = tolower(input[i]);
                }
                
                if (strcmp(input, "yes") == 0 || strcmp(input, "y") == 0) {
                    printf("Registration completed successfully!\n");
                    current_state = DONE;
                } else if (strcmp(input, "no") == 0 || strcmp(input, "n") == 0) {
                    printf("Let's start over.\n");
                    memset(&person, 0, sizeof(person));
                    current_state = INIT;
                } else {
                    printf("Please answer 'yes' or 'no': ");
                }
                break;
                
            case DONE:
                running = 0;
                break;
                
            case ERROR:
                printf("An error occurred. Exiting.\n");
                running = 0;
                break;
        }
    }
    
    return 0;
}
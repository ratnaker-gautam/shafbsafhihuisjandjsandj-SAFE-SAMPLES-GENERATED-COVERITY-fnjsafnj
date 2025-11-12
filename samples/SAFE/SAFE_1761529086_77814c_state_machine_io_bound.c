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

int validate_name(const char *name) {
    if (name == NULL) return 0;
    
    size_t len = strlen(name);
    if (len < 1 || len > 63) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!isalpha(name[i]) && name[i] != ' ' && name[i] != '-') {
            return 0;
        }
    }
    
    return 1;
}

int validate_age(const char *input) {
    if (input == NULL) return 0;
    
    char *endptr;
    long age = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') {
        return 0;
    }
    
    if (age < 0 || age > 150) {
        return 0;
    }
    
    return 1;
}

int get_age_value(const char *input) {
    char *endptr;
    long age = strtol(input, &endptr, 10);
    return (int)age;
}

int main(void) {
    enum State current_state = INIT;
    struct Person person;
    char input[256];
    int attempts = 0;
    
    printf("Person Registration System\n");
    printf("==========================\n\n");
    
    while (current_state != DONE && current_state != ERROR && attempts < 10) {
        switch (current_state) {
            case INIT:
                printf("Please enter the person's name: ");
                if (read_line(input, sizeof(input)) == 0) {
                    current_state = READING_NAME;
                } else {
                    printf("Error reading input.\n");
                    current_state = ERROR;
                }
                break;
                
            case READING_NAME:
                if (validate_name(input)) {
                    strncpy(person.name, input, sizeof(person.name) - 1);
                    person.name[sizeof(person.name) - 1] = '\0';
                    printf("Please enter the person's age: ");
                    if (read_line(input, sizeof(input)) == 0) {
                        current_state = READING_AGE;
                    } else {
                        printf("Error reading input.\n");
                        current_state = ERROR;
                    }
                } else {
                    printf("Invalid name. Names must contain only letters, spaces, and hyphens (1-63 characters).\n");
                    printf("Please enter the person's name: ");
                    if (read_line(input, sizeof(input)) == 0) {
                        current_state = READING_NAME;
                    } else {
                        printf("Error reading input.\n");
                        current_state = ERROR;
                    }
                    attempts++;
                }
                break;
                
            case READING_AGE:
                if (validate_age(input)) {
                    person.age = get_age_value(input);
                    printf("Confirm registration:\n");
                    printf("Name: %s\n", person.name);
                    printf("Age: %d\n", person.age);
                    printf("Is this correct? (y/n): ");
                    if (read_line(input, sizeof(input)) == 0) {
                        current_state = CONFIRMING;
                    } else {
                        printf("Error reading input.\n");
                        current_state = ERROR;
                    }
                } else {
                    printf("Invalid age. Please enter a number between 0 and 150.\n");
                    printf("Please enter the person's age: ");
                    if (read_line(input, sizeof(input)) == 0) {
                        current_state = READING_AGE;
                    } else {
                        printf("Error reading input.\n");
                        current_state = ERROR;
                    }
                    attempts++;
                }
                break;
                
            case CONFIRMING:
                if (strlen(input) == 1 && (input[0] == 'y' || input[0] == 'Y')) {
                    printf("\nRegistration completed successfully!\n");
                    printf("Registered person: %s, %d years old\n", person.name, person.age);
                    current_state = DONE;
                } else if (strlen(input) == 1 && (input[0] == 'n' || input[0] == 'N')) {
                    printf("Restarting registration process...\n\n");
                    current_state = INIT;
                    attempts = 0;
                } else {
                    printf("Please enter 'y' for yes or 'n' for no: ");
                    if (read_line(input, sizeof(input)) == 0) {
                        current_state = CONFIRMING;
                    } else {
                        printf("Error reading input.\n");
                        current_state = ERROR;
                    }
                    attempts++;
                }
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    if (current_state == ERROR || attempts >= 10) {
        printf("Too many failed attempts. Exiting.\n");
        return 1;
    }
    
    return 0;
}
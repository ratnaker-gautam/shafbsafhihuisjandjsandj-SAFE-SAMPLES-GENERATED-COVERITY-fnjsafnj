//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NAME,
    READING_AGE,
    PROCESSING,
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
    char input_buffer[256];
    
    printf("Person Data Entry System\n");
    printf("Enter 'quit' at any time to exit\n\n");
    
    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Enter person's name: ");
                if (read_line(input_buffer, sizeof(input_buffer)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (is_valid_name(input_buffer)) {
                    strncpy(person.name, input_buffer, sizeof(person.name) - 1);
                    person.name[sizeof(person.name) - 1] = '\0';
                    current_state = READING_AGE;
                } else {
                    printf("Error: Invalid name. Use only letters, spaces, and hyphens.\n");
                    current_state = INIT;
                }
                break;
                
            case READING_AGE:
                printf("Enter person's age: ");
                if (read_line(input_buffer, sizeof(input_buffer)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (is_valid_age(input_buffer)) {
                    char *endptr;
                    long age_val = strtol(input_buffer, &endptr, 10);
                    if (age_val >= 1 && age_val <= 150) {
                        person.age = (int)age_val;
                        current_state = PROCESSING;
                    } else {
                        printf("Error: Invalid age. Must be a number between 1 and 150.\n");
                        current_state = READING_AGE;
                    }
                } else {
                    printf("Error: Invalid age. Must be a number between 1 and 150.\n");
                    current_state = READING_AGE;
                }
                break;
                
            case PROCESSING:
                printf("\nProcessing complete:\n");
                printf("Name: %s\n", person.name);
                printf("Age: %d\n", person.age);
                printf("\n");
                
                printf("Process another person? (yes/no): ");
                if (read_line(input_buffer, sizeof(input_buffer)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input_buffer, "yes") == 0) {
                    current_state = INIT;
                } else if (strcmp(input_buffer, "no") == 0) {
                    current_state = DONE;
                } else {
                    printf("Invalid response. Please enter 'yes' or 'no'.\n");
                    current_state = PROCESSING;
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
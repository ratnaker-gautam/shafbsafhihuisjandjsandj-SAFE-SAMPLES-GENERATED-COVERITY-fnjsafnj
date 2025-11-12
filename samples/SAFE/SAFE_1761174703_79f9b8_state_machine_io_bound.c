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
    FINISHED,
    ERROR
};

struct Person {
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

int validate_age(const char *input, int *age) {
    if (input == NULL || strlen(input) == 0) {
        return 0;
    }
    
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (!isdigit(input[i])) {
            return 0;
        }
    }
    
    long val = strtol(input, NULL, 10);
    if (val < 1 || val > 150) {
        return 0;
    }
    
    *age = (int)val;
    return 1;
}

int main(void) {
    enum State current_state = INIT;
    struct Person person;
    char input[64];
    int valid_inputs = 0;
    
    printf("Person Data Entry System\n");
    printf("Enter 'quit' at any time to exit\n\n");
    
    while (current_state != FINISHED && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Enter person's name: ");
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (validate_name(input)) {
                    strncpy(person.name, input, sizeof(person.name) - 1);
                    person.name[sizeof(person.name) - 1] = '\0';
                    current_state = READING_AGE;
                } else {
                    printf("Invalid name. Use only letters and spaces (max 31 chars).\n");
                    current_state = INIT;
                }
                break;
                
            case READING_AGE:
                printf("Enter person's age: ");
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (validate_age(input, &person.age)) {
                    current_state = PROCESSING;
                } else {
                    printf("Invalid age. Use numbers only (1-150).\n");
                    current_state = READING_AGE;
                }
                break;
                
            case PROCESSING:
                printf("\nProcessing data...\n");
                printf("Name: %s\n", person.name);
                printf("Age: %d\n", person.age);
                
                if (person.age >= 18) {
                    printf("Status: Adult\n");
                } else {
                    printf("Status: Minor\n");
                }
                
                valid_inputs++;
                printf("\nData entry %d completed.\n\n", valid_inputs);
                current_state = INIT;
                break;
                
            case ERROR:
                printf("An error occurred. Exiting.\n");
                current_state = FINISHED;
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    if (valid_inputs > 0) {
        printf("\nTotal valid entries processed: %d\n", valid_inputs);
    }
    printf("Goodbye!\n");
    
    return 0;
}
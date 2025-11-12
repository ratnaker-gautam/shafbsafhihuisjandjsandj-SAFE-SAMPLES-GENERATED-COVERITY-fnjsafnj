//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
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
    
    return len;
}

int is_valid_name(const char *name) {
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

int is_valid_age(int age) {
    return age >= 0 && age <= 150;
}

int main(void) {
    enum State current_state = INIT;
    struct Person person;
    char input_buffer[64];
    int temp_age;
    
    printf("Person Data Entry System\n");
    printf("Enter 'quit' at any time to exit\n\n");
    
    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Enter person's name: ");
                if (read_line(input_buffer, sizeof(input_buffer)) < 0) {
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
                    printf("Error: Invalid name. Use only letters and spaces.\n");
                    current_state = INIT;
                }
                break;
                
            case READING_AGE:
                printf("Enter person's age: ");
                if (read_line(input_buffer, sizeof(input_buffer)) < 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (sscanf(input_buffer, "%d", &temp_age) == 1 && is_valid_age(temp_age)) {
                    person.age = temp_age;
                    current_state = PROCESSING;
                } else {
                    printf("Error: Invalid age. Must be 0-150.\n");
                    current_state = READING_AGE;
                }
                break;
                
            case PROCESSING:
                printf("\nProcessing data...\n");
                printf("Name: %s\n", person.name);
                printf("Age: %d\n", person.age);
                
                if (person.age < 18) {
                    printf("Category: Minor\n");
                } else if (person.age < 65) {
                    printf("Category: Adult\n");
                } else {
                    printf("Category: Senior\n");
                }
                
                printf("\nWould you like to enter another person? (yes/no): ");
                if (read_line(input_buffer, sizeof(input_buffer)) < 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input_buffer, "yes") == 0) {
                    current_state = INIT;
                } else if (strcmp(input_buffer, "no") == 0) {
                    current_state = DONE;
                } else {
                    printf("Invalid response. Exiting.\n");
                    current_state = DONE;
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
    
    printf("Goodbye!\n");
    return 0;
}
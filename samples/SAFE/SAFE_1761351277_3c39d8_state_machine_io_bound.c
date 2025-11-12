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

int validate_name(const char* name) {
    if (name == NULL || strlen(name) == 0 || strlen(name) >= 32) {
        return 0;
    }
    for (size_t i = 0; i < strlen(name); i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

int validate_age(int age) {
    return age >= 0 && age <= 150;
}

int main(void) {
    enum State current_state = INIT;
    struct Person person;
    char input_buffer[64];
    int temp_age;
    
    printf("Person Data Processor\n");
    printf("Enter 'quit' to exit\n\n");
    
    while (current_state != FINISHED && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Enter name: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (validate_name(input_buffer)) {
                    strncpy(person.name, input_buffer, sizeof(person.name) - 1);
                    person.name[sizeof(person.name) - 1] = '\0';
                    current_state = READING_AGE;
                } else {
                    printf("Error: Invalid name. Use only letters and spaces.\n");
                    current_state = INIT;
                }
                break;
                
            case READING_AGE:
                printf("Enter age: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                if (sscanf(input_buffer, "%d", &temp_age) == 1) {
                    if (validate_age(temp_age)) {
                        person.age = temp_age;
                        current_state = PROCESSING;
                    } else {
                        printf("Error: Age must be between 0 and 150.\n");
                        current_state = READING_AGE;
                    }
                } else {
                    printf("Error: Invalid age format.\n");
                    current_state = READING_AGE;
                }
                break;
                
            case PROCESSING:
                printf("\nProcessing complete:\n");
                printf("Name: %s\n", person.name);
                printf("Age: %d\n", person.age);
                printf("Status: %s\n", person.age >= 18 ? "Adult" : "Minor");
                printf("\n");
                current_state = INIT;
                break;
                
            case ERROR:
                printf("Fatal error occurred. Exiting.\n");
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    printf("Program terminated.\n");
    return current_state == ERROR ? 1 : 0;
}
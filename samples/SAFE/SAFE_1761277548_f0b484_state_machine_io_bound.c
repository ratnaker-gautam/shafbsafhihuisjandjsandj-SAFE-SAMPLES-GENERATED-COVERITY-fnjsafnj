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

int validate_name(const char *name) {
    if (name == NULL) return 0;
    if (strlen(name) < 1 || strlen(name) > 63) return 0;
    for (size_t i = 0; i < strlen(name); i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
    }
    return 1;
}

int validate_age(int age) {
    return age >= 0 && age <= 150;
}

int main(void) {
    enum State current_state = INIT;
    struct Person person;
    char input_buffer[256];
    int temp_age;
    
    printf("Person Data Entry System\n");
    printf("Enter 'quit' at any time to exit\n\n");
    
    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Enter person's name: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (validate_name(input_buffer)) {
                    strncpy(person.name, input_buffer, sizeof(person.name) - 1);
                    person.name[sizeof(person.name) - 1] = '\0';
                    current_state = READING_AGE;
                } else {
                    printf("Error: Invalid name. Use only letters and spaces (1-63 characters).\n");
                    current_state = INIT;
                }
                break;
                
            case READING_AGE:
                printf("Enter person's age: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (sscanf(input_buffer, "%d", &temp_age) == 1 && validate_age(temp_age)) {
                    person.age = temp_age;
                    current_state = PROCESSING;
                } else {
                    printf("Error: Invalid age. Must be 0-150.\n");
                    current_state = READING_AGE;
                }
                break;
                
            case PROCESSING:
                printf("\nProcessing complete:\n");
                printf("Name: %s\n", person.name);
                printf("Age: %d\n", person.age);
                printf("\n");
                current_state = INIT;
                break;
                
            case ERROR:
                printf("Fatal error occurred. Exiting.\n");
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}
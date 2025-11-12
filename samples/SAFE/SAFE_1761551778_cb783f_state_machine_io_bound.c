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
    FINISHED,
    ERROR
};

struct person {
    char name[32];
    int age;
};

int validate_name(const char *name) {
    if (name == NULL) return 0;
    if (strlen(name) < 1 || strlen(name) >= 32) return 0;
    for (size_t i = 0; i < strlen(name); i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
    }
    return 1;
}

int validate_age(int age) {
    return age >= 0 && age <= 150;
}

int main(void) {
    enum state current_state = INIT;
    struct person current_person;
    char input_buffer[64];
    int temp_age;
    
    printf("Person Data Processor\n");
    printf("Enter 'quit' to exit\n\n");
    
    while (current_state != FINISHED) {
        switch (current_state) {
            case INIT:
                printf("Enter person's name: ");
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
                    strncpy(current_person.name, input_buffer, sizeof(current_person.name) - 1);
                    current_person.name[sizeof(current_person.name) - 1] = '\0';
                    current_state = READING_AGE;
                } else {
                    printf("Error: Invalid name. Use only letters and spaces (1-31 characters).\n");
                    current_state = INIT;
                }
                break;
                
            case READING_AGE:
                printf("Enter person's age: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                if (sscanf(input_buffer, "%d", &temp_age) == 1) {
                    if (validate_age(temp_age)) {
                        current_person.age = temp_age;
                        current_state = PROCESSING;
                    } else {
                        printf("Error: Invalid age. Must be between 0 and 150.\n");
                        current_state = READING_AGE;
                    }
                } else {
                    printf("Error: Please enter a valid integer for age.\n");
                    current_state = READING_AGE;
                }
                break;
                
            case PROCESSING:
                printf("Processing: %s, %d years old\n", current_person.name, current_person.age);
                
                if (current_person.age < 18) {
                    printf("Category: Minor\n");
                } else if (current_person.age < 65) {
                    printf("Category: Adult\n");
                } else {
                    printf("Category: Senior\n");
                }
                printf("\n");
                current_state = INIT;
                break;
                
            case ERROR:
                printf("Fatal error occurred. Exiting.\n");
                current_state = FINISHED;
                break;
                
            case FINISHED:
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}
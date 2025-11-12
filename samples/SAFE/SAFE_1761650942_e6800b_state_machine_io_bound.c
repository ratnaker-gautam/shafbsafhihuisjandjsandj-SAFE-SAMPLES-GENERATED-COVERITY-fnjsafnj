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
    enum state current_state = INIT;
    struct person current_person;
    char input_buffer[64];
    int valid_records = 0;
    
    printf("Person Data Entry System\n");
    printf("Enter 'quit' at any prompt to exit\n\n");
    
    while (current_state != FINISHED && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Enter person's name: ");
                if (read_line(input_buffer, sizeof(input_buffer)) != 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (validate_name(input_buffer)) {
                    strncpy(current_person.name, input_buffer, sizeof(current_person.name) - 1);
                    current_person.name[sizeof(current_person.name) - 1] = '\0';
                    current_state = READING_AGE;
                } else {
                    printf("Error: Invalid name. Use only letters and spaces, max 31 characters.\n");
                    current_state = INIT;
                }
                break;
                
            case READING_AGE:
                printf("Enter person's age: ");
                if (read_line(input_buffer, sizeof(input_buffer)) != 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (validate_age(input_buffer, &current_person.age)) {
                    current_state = PROCESSING;
                } else {
                    printf("Error: Invalid age. Use numbers only, range 1-150.\n");
                    current_state = READING_AGE;
                }
                break;
                
            case PROCESSING:
                printf("Record processed: %s, %d years old\n\n", current_person.name, current_person.age);
                valid_records++;
                current_state = INIT;
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                printf("System error occurred\n");
                current_state = FINISHED;
                break;
        }
    }
    
    printf("\nTotal valid records processed: %d\n", valid_records);
    printf("Goodbye!\n");
    
    return 0;
}
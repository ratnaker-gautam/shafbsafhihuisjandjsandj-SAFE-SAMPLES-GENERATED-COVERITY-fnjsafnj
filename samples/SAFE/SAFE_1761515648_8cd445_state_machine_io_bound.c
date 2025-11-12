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
    DONE,
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

int validate_age(const char *input) {
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
    enum state current_state = INIT;
    struct person current_person;
    char input_buffer[64];
    int person_count = 0;
    
    printf("Person Data Entry System\n");
    printf("Enter 'quit' at any time to exit\n\n");
    
    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Enter person's name: ");
                if (read_line(input_buffer, sizeof(input_buffer)) != 0) {
                    current_state = DONE;
                    break;
                }
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (validate_name(input_buffer)) {
                    strncpy(current_person.name, input_buffer, sizeof(current_person.name) - 1);
                    current_person.name[sizeof(current_person.name) - 1] = '\0';
                    current_state = READING_AGE;
                } else {
                    printf("Invalid name. Use only letters and spaces, max 31 characters.\n");
                    current_state = INIT;
                }
                break;
                
            case READING_AGE:
                printf("Enter person's age: ");
                if (read_line(input_buffer, sizeof(input_buffer)) != 0) {
                    current_state = DONE;
                    break;
                }
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (validate_age(input_buffer)) {
                    char *endptr;
                    current_person.age = (int)strtol(input_buffer, &endptr, 10);
                    current_state = PROCESSING;
                } else {
                    printf("Invalid age. Use numbers only, 1-150.\n");
                    current_state = READING_AGE;
                }
                break;
                
            case PROCESSING:
                printf("Person %d: %s, %d years old\n", person_count + 1, current_person.name, current_person.age);
                person_count++;
                current_state = INIT;
                break;
                
            case ERROR:
                printf("An error occurred. Exiting.\n");
                current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("\nTotal persons entered: %d\n", person_count);
    printf("Goodbye!\n");
    
    return 0;
}
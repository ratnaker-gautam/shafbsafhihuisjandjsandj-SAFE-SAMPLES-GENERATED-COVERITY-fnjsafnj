//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READING_NAME,
    READING_AGE,
    READING_CITY,
    PROCESSING,
    FINISHED,
    ERROR
};

struct person {
    char name[64];
    int age;
    char city[64];
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
    if (name == NULL || strlen(name) == 0 || strlen(name) >= 64) {
        return 0;
    }
    
    for (size_t i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ' && name[i] != '-') {
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

int validate_city(const char *city) {
    if (city == NULL || strlen(city) == 0 || strlen(city) >= 64) {
        return 0;
    }
    
    for (size_t i = 0; city[i] != '\0'; i++) {
        if (!isalpha(city[i]) && city[i] != ' ' && city[i] != '-') {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    enum state current_state = INIT;
    struct person current_person;
    char input_buffer[256];
    int person_count = 0;
    
    printf("Person Data Entry System\n");
    printf("Enter 'quit' at any prompt to exit\n\n");
    
    while (current_state != FINISHED && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                memset(&current_person, 0, sizeof(current_person));
                printf("Enter person's first name: ");
                current_state = READING_NAME;
                break;
                
            case READING_NAME:
                if (read_line(input_buffer, sizeof(input_buffer)) != 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (!validate_name(input_buffer)) {
                    printf("Invalid name. Please enter letters, spaces, and hyphens only.\n");
                    break;
                }
                
                strncpy(current_person.name, input_buffer, sizeof(current_person.name) - 1);
                current_person.name[sizeof(current_person.name) - 1] = '\0';
                printf("Enter age: ");
                current_state = READING_AGE;
                break;
                
            case READING_AGE:
                if (read_line(input_buffer, sizeof(input_buffer)) != 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (!validate_age(input_buffer)) {
                    printf("Invalid age. Please enter a number between 1 and 150.\n");
                    break;
                }
                
                current_person.age = atoi(input_buffer);
                printf("Enter city: ");
                current_state = READING_CITY;
                break;
                
            case READING_CITY:
                if (read_line(input_buffer, sizeof(input_buffer)) != 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (!validate_city(input_buffer)) {
                    printf("Invalid city name. Please enter letters, spaces, and hyphens only.\n");
                    break;
                }
                
                strncpy(current_person.city, input_buffer, sizeof(current_person.city) - 1);
                current_person.city[sizeof(current_person.city) - 1] = '\0';
                current_state = PROCESSING;
                break;
                
            case PROCESSING:
                printf("\nPerson #%d:\n", ++person_count);
                printf("  Name: %s\n", current_person.name);
                printf("  Age: %d\n", current_person.age);
                printf("  City: %s\n\n", current_person.city);
                current_state = INIT;
                break;
                
            case ERROR:
                printf("An error occurred. Exiting.\n");
                current_state = FINISHED;
                break;
                
            case FINISHED:
                break;
        }
    }
    
    if (person_count > 0
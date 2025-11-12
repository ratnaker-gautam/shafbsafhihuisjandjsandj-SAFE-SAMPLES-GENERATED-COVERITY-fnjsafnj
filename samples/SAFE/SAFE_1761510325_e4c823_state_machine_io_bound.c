//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NAME,
    READING_AGE,
    READING_CITY,
    PROCESSING,
    DONE,
    ERROR
};

struct Person {
    char name[32];
    int age;
    char city[32];
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
    if (strlen(name) == 0 || strlen(name) >= 32) {
        return 0;
    }
    
    for (size_t i = 0; i < strlen(name); i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            return 0;
        }
    }
    
    return 1;
}

int validate_age(const char *input) {
    for (size_t i = 0; i < strlen(input); i++) {
        if (!isdigit(input[i])) {
            return 0;
        }
    }
    
    long age = strtol(input, NULL, 10);
    if (age < 0 || age > 150) {
        return 0;
    }
    
    return 1;
}

int validate_city(const char *city) {
    if (strlen(city) == 0 || strlen(city) >= 32) {
        return 0;
    }
    
    for (size_t i = 0; i < strlen(city); i++) {
        if (!isalpha(city[i]) && city[i] != ' ') {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    enum State current_state = INIT;
    struct Person person;
    char input[64];
    int valid_input = 0;
    
    printf("Person Information System\n");
    printf("Enter 'quit' at any time to exit\n\n");
    
    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Enter person's name: ");
                current_state = READING_NAME;
                break;
                
            case READING_NAME:
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (validate_name(input)) {
                    strncpy(person.name, input, sizeof(person.name) - 1);
                    person.name[sizeof(person.name) - 1] = '\0';
                    current_state = READING_AGE;
                } else {
                    printf("Invalid name. Please enter letters and spaces only: ");
                }
                break;
                
            case READING_AGE:
                printf("Enter person's age: ");
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (validate_age(input)) {
                    person.age = (int)strtol(input, NULL, 10);
                    current_state = READING_CITY;
                } else {
                    printf("Invalid age. Please enter a number between 0 and 150: ");
                }
                break;
                
            case READING_CITY:
                printf("Enter person's city: ");
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (validate_city(input)) {
                    strncpy(person.city, input, sizeof(person.city) - 1);
                    person.city[sizeof(person.city) - 1] = '\0';
                    current_state = PROCESSING;
                } else {
                    printf("Invalid city. Please enter letters and spaces only: ");
                }
                break;
                
            case PROCESSING:
                printf("\nPerson Information:\n");
                printf("Name: %s\n", person.name);
                printf("Age: %d\n", person.age);
                printf("City: %s\n", person.city);
                printf("\nWould you like to enter another person? (yes/no): ");
                
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strcmp(input, "yes") == 0) {
                    current_state = INIT;
                } else if (strcmp(input, "no") == 0) {
                    current_state = DONE;
                } else {
                    printf("Please enter 'yes' or 'no': ");
                }
                break;
                
            case ERROR:
                printf("An error occurred. Exiting.\n");
                current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}
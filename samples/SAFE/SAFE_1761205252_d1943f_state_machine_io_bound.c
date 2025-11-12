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
    if (strlen(name) == 0 || strlen(name) >= 64) {
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
    char *endptr;
    long age = strtol(input, &endptr, 10);
    if (*endptr != '\0') {
        return 0;
    }
    if (age < 0 || age > 150) {
        return 0;
    }
    return 1;
}

int validate_city(const char *city) {
    if (strlen(city) == 0 || strlen(city) >= 64) {
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
    enum state current_state = INIT;
    struct person p;
    char input[128];
    int valid_input = 0;
    
    printf("Person Data Entry System\n");
    printf("Enter 'quit' at any time to exit\n\n");
    
    while (current_state != FINISHED && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Starting data entry...\n");
                memset(&p, 0, sizeof(p));
                current_state = READING_NAME;
                break;
                
            case READING_NAME:
                printf("Enter name: ");
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                if (strcmp(input, "quit") == 0) {
                    current_state = FINISHED;
                    break;
                }
                if (validate_name(input)) {
                    strncpy(p.name, input, sizeof(p.name) - 1);
                    current_state = READING_AGE;
                } else {
                    printf("Invalid name. Use only letters and spaces.\n");
                }
                break;
                
            case READING_AGE:
                printf("Enter age: ");
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                if (strcmp(input, "quit") == 0) {
                    current_state = FINISHED;
                    break;
                }
                if (validate_age(input)) {
                    p.age = atoi(input);
                    current_state = READING_CITY;
                } else {
                    printf("Invalid age. Enter a number between 0 and 150.\n");
                }
                break;
                
            case READING_CITY:
                printf("Enter city: ");
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                if (strcmp(input, "quit") == 0) {
                    current_state = FINISHED;
                    break;
                }
                if (validate_city(input)) {
                    strncpy(p.city, input, sizeof(p.city) - 1);
                    current_state = PROCESSING;
                } else {
                    printf("Invalid city. Use only letters and spaces.\n");
                }
                break;
                
            case PROCESSING:
                printf("\nProcessing complete:\n");
                printf("Name: %s\n", p.name);
                printf("Age: %d\n", p.age);
                printf("City: %s\n", p.city);
                printf("\nWould you like to enter another person? (y/n): ");
                
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = ERROR;
                    break;
                }
                
                if (strlen(input) == 1 && (input[0] == 'y' || input[0] == 'Y')) {
                    current_state = INIT;
                } else if (strlen(input) == 1 && (input[0] == 'n' || input[0] == 'N')) {
                    current_state = FINISHED;
                } else {
                    printf("Invalid choice. Please enter 'y' or 'n'.\n");
                }
                break;
                
            case ERROR:
                printf("An error occurred during input.\n");
                current_state = FINISHED;
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}
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
    FINISHED,
    ERROR
};

struct Person {
    char name[64];
    int age;
    char city[64];
};

int validate_name(const char* name) {
    if (name == NULL) return 0;
    size_t len = strlen(name);
    if (len == 0 || len >= 64) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
    }
    return 1;
}

int validate_age(int age) {
    return age >= 0 && age <= 150;
}

int validate_city(const char* city) {
    if (city == NULL) return 0;
    size_t len = strlen(city);
    if (len == 0 || len >= 64) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalpha(city[i]) && city[i] != ' ') return 0;
    }
    return 1;
}

int main(void) {
    enum State current_state = INIT;
    struct Person person;
    char input[256];
    int temp_age;
    
    printf("Person Data Entry System\n");
    printf("Enter 'quit' at any time to exit\n\n");
    
    while (current_state != FINISHED && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                memset(&person, 0, sizeof(person));
                printf("Starting new person entry...\n");
                current_state = READING_NAME;
                break;
                
            case READING_NAME:
                printf("Enter name: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                input[strcspn(input, "\n")] = '\0';
                
                if (strcmp(input, "quit") == 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (validate_name(input)) {
                    strncpy(person.name, input, sizeof(person.name) - 1);
                    person.name[sizeof(person.name) - 1] = '\0';
                    current_state = READING_AGE;
                } else {
                    printf("Invalid name. Use only letters and spaces.\n");
                }
                break;
                
            case READING_AGE:
                printf("Enter age: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                input[strcspn(input, "\n")] = '\0';
                
                if (strcmp(input, "quit") == 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (sscanf(input, "%d", &temp_age) == 1 && validate_age(temp_age)) {
                    person.age = temp_age;
                    current_state = READING_CITY;
                } else {
                    printf("Invalid age. Enter a number between 0 and 150.\n");
                }
                break;
                
            case READING_CITY:
                printf("Enter city: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                input[strcspn(input, "\n")] = '\0';
                
                if (strcmp(input, "quit") == 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (validate_city(input)) {
                    strncpy(person.city, input, sizeof(person.city) - 1);
                    person.city[sizeof(person.city) - 1] = '\0';
                    current_state = PROCESSING;
                } else {
                    printf("Invalid city. Use only letters and spaces.\n");
                }
                break;
                
            case PROCESSING:
                printf("\nProcessing person data...\n");
                printf("Name: %s\n", person.name);
                printf("Age: %d\n", person.age);
                printf("City: %s\n", person.city);
                printf("Person data processed successfully.\n\n");
                
                printf("Enter another person? (y/n): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                input[strcspn(input, "\n")] = '\0';
                
                if (input[0] == 'y' || input[0] == 'Y') {
                    current_state = INIT;
                } else if (input[0] == 'n' || input[0] == 'N') {
                    current_state = FINISHED;
                } else {
                    printf("Invalid response. Please enter 'y' or 'n'.\n");
                }
                break;
                
            case ERROR:
                printf("An error occurred during input.\n");
                current_state = FINISHED;
                break;
                
            case FINISHED:
                break;
        }
    }
    
    printf("Program terminated.\n");
    return 0;
}
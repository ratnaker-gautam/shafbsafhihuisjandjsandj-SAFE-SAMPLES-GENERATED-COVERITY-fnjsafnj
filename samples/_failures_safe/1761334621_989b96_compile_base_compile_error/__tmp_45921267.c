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

int is_valid_name(const char *name) {
    if (name == NULL || strlen(name) == 0 || strlen(name) >= 63) {
        return 0;
    }
    
    for (size_t i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ' && name[i] != '-') {
            return 0;
        }
    }
    
    return 1;
}

int is_valid_city(const char *city) {
    if (city == NULL || strlen(city) == 0 || strlen(city) >= 63) {
        return 0;
    }
    
    for (size_t i = 0; city[i] != '\0'; i++) {
        if (!isalpha(city[i]) && city[i] != ' ' && city[i] != '-') {
            return 0;
        }
    }
    
    return 1;
}

int is_valid_age(const char *age_str) {
    if (age_str == NULL || strlen(age_str) == 0) {
        return 0;
    }
    
    for (size_t i = 0; age_str[i] != '\0'; i++) {
        if (!isdigit(age_str[i])) {
            return 0;
        }
    }
    
    char *endptr;
    long age = strtol(age_str, &endptr, 10);
    if (*endptr != '\0' || age < 0 || age > 150) {
        return 0;
    }
    
    return 1;
}

int main(void) {
    enum State current_state = INIT;
    struct Person person;
    char input[128];
    int person_count = 0;
    
    printf("Person Data Entry System\n");
    printf("Enter 'quit' at any time to exit\n\n");
    
    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                memset(&person, 0, sizeof(person));
                printf("Starting new person entry...\n");
                current_state = READING_NAME;
                break;
                
            case READING_NAME:
                printf("Enter name: ");
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = DONE;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (is_valid_name(input)) {
                    strncpy(person.name, input, sizeof(person.name) - 1);
                    person.name[sizeof(person.name) - 1] = '\0';
                    current_state = READING_AGE;
                } else {
                    printf("Invalid name. Use only letters, spaces, and hyphens.\n");
                }
                break;
                
            case READING_AGE:
                printf("Enter age: ");
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = DONE;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (is_valid_age(input)) {
                    char *endptr;
                    long age_val = strtol(input, &endptr, 10);
                    if (age_val >= 0 && age_val <= 150) {
                        person.age = (int)age_val;
                        current_state = READING_CITY;
                    } else {
                        printf("Invalid age. Enter a number between 0 and 150.\n");
                    }
                } else {
                    printf("Invalid age. Enter a number between 0 and 150.\n");
                }
                break;
                
            case READING_CITY:
                printf("Enter city: ");
                if (read_line(input, sizeof(input)) != 0) {
                    current_state = DONE;
                    break;
                }
                
                if (strcmp(input, "quit") == 0) {
                    current_state = DONE;
                    break;
                }
                
                if (is_valid_city(input)) {
                    strncpy(person.city, input, sizeof(person.city) - 1);
                    person.city[sizeof(person.city) - 1] = '\0';
                    current_state = PROCESSING;
                } else {
                    printf("Invalid city. Use only letters, spaces, and hyphens.\n");
                }
                break;
                
            case PROCESSING:
                printf("\nPerson %d Summary:\n", person_count + 1);
                printf("Name: %s\n", person.name);
                printf("Age: %d\n", person.age);
                printf("City: %s\n\n", person.city);
                
                person_count++;
                
                printf("Enter another person? (y/n): ");
                if (read_line(input, sizeof(input)) != 0)
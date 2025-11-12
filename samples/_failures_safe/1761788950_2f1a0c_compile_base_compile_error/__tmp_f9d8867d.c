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
    CONFIRMING,
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

int validate_name(const char *name) {
    size_t name_len = strlen(name);
    if (name_len == 0 || name_len >= 63) {
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
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (!isdigit(input[i])) {
            return 0;
        }
    }
    
    long age = strtol(input, NULL, 10);
    if (age <= 0 || age > 150) {
        return 0;
    }
    
    return 1;
}

int validate_city(const char *city) {
    size_t city_len = strlen(city);
    if (city_len == 0 || city_len >= 63) {
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
    enum State state = INIT;
    struct Person person;
    char input[128];
    int confirmed = 0;
    
    memset(&person, 0, sizeof(person));
    
    while (state != DONE && state != ERROR) {
        switch (state) {
            case INIT:
                printf("Welcome to the person registration system.\n");
                state = READING_NAME;
                break;
                
            case READING_NAME:
                printf("Please enter your name: ");
                if (read_line(input, sizeof(input)) != 0) {
                    state = ERROR;
                    break;
                }
                
                if (!validate_name(input)) {
                    printf("Invalid name. Please use only letters, spaces, and hyphens.\n");
                    break;
                }
                
                strncpy(person.name, input, sizeof(person.name) - 1);
                person.name[sizeof(person.name) - 1] = '\0';
                state = READING_AGE;
                break;
                
            case READING_AGE:
                printf("Please enter your age: ");
                if (read_line(input, sizeof(input)) != 0) {
                    state = ERROR;
                    break;
                }
                
                if (!validate_age(input)) {
                    printf("Invalid age. Please enter a number between 1 and 150.\n");
                    break;
                }
                
                person.age = (int)strtol(input, NULL, 10);
                state = READING_CITY;
                break;
                
            case READING_CITY:
                printf("Please enter your city: ");
                if (read_line(input, sizeof(input)) != 0) {
                    state = ERROR;
                    break;
                }
                
                if (!validate_city(input)) {
                    printf("Invalid city name. Please use only letters, spaces, and hyphens.\n");
                    break;
                }
                
                strncpy(person.city, input, sizeof(person.city) - 1);
                person.city[sizeof(person.city) - 1] = '\0';
                state = CONFIRMING;
                break;
                
            case CONFIRMING:
                printf("\nPlease confirm your information:\n");
                printf("Name: %s\n", person.name);
                printf("Age: %d\n", person.age);
                printf("City: %s\n", person.city);
                printf("Is this correct? (yes/no): ");
                
                if (read_line(input, sizeof(input)) != 0) {
                    state = ERROR;
                    break;
                }
                
                for (size_t i = 0; input[i] != '\0'; i++) {
                    input[i] = tolower(input[i]);
                }
                
                if (strcmp(input, "yes") == 0 || strcmp(input, "y") == 0) {
                    confirmed = 1;
                    state = DONE;
                } else if (strcmp(input, "no") == 0 || strcmp(input, "n") == 0) {
                    printf("Let's start over.\n");
                    memset(&person, 0, sizeof(person));
                    state = READING_NAME;
                } else {
                    printf("Please answer 'yes' or 'no'.\n");
                }
                break;
                
            case ERROR:
                printf("An error occurred. Exiting.\n");
                return 1;
                
            default:
                state = ERROR;
                break;
        }
    }
    
    if (confirmed) {
        printf("\nRegistration complete! Thank you, %s, age %
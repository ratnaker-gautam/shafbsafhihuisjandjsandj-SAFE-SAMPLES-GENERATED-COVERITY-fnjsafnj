//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READING_NAME,
    READING_AGE,
    READING_SCORE,
    PROCESSING,
    FINISHED,
    ERROR
};

struct person {
    char name[32];
    int age;
    double score;
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

int validate_age(int age) {
    return age >= 0 && age <= 150;
}

int validate_score(double score) {
    return score >= 0.0 && score <= 100.0;
}

int main(void) {
    enum state current_state = INIT;
    struct person current_person;
    char input_buffer[256];
    int temp_int;
    double temp_double;
    int person_count = 0;
    
    printf("Person Data Entry System\n");
    printf("Enter 'quit' at any prompt to exit\n\n");
    
    while (current_state != FINISHED && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Enter person name: ");
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
                    printf("Invalid name. Use only letters and spaces, max 31 characters.\n");
                }
                break;
                
            case READING_AGE:
                printf("Enter age (0-150): ");
                if (read_line(input_buffer, sizeof(input_buffer)) != 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (sscanf(input_buffer, "%d", &temp_int) == 1 && validate_age(temp_int)) {
                    current_person.age = temp_int;
                    current_state = READING_SCORE;
                } else {
                    printf("Invalid age. Enter a number between 0 and 150.\n");
                }
                break;
                
            case READING_SCORE:
                printf("Enter score (0.0-100.0): ");
                if (read_line(input_buffer, sizeof(input_buffer)) != 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (strcmp(input_buffer, "quit") == 0) {
                    current_state = FINISHED;
                    break;
                }
                
                if (sscanf(input_buffer, "%lf", &temp_double) == 1 && validate_score(temp_double)) {
                    current_person.score = temp_double;
                    current_state = PROCESSING;
                } else {
                    printf("Invalid score. Enter a number between 0.0 and 100.0.\n");
                }
                break;
                
            case PROCESSING:
                printf("\nPerson %d recorded:\n", ++person_count);
                printf("  Name: %s\n", current_person.name);
                printf("  Age: %d\n", current_person.age);
                printf("  Score: %.2f\n", current_person.score);
                printf("\n");
                current_state = INIT;
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                printf("An error occurred. Exiting.\n");
                break;
        }
    }
    
    if (person_count > 0) {
        printf("Total persons recorded: %d\n", person_count);
    } else {
        printf("No persons recorded.\n");
    }
    
    return 0;
}
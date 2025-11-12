//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READING_NAME,
    READING_AGE,
    READING_EMAIL,
    VALIDATING,
    COMPLETE,
    ERROR
};

struct person {
    char name[64];
    int age;
    char email[128];
};

int validate_name(const char *name) {
    if (name[0] == '\0') return 0;
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
    }
    return 1;
}

int validate_age(int age) {
    return age >= 0 && age <= 150;
}

int validate_email(const char *email) {
    int at_count = 0;
    int dot_after_at = 0;
    
    if (email[0] == '\0') return 0;
    
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            at_count++;
            if (at_count > 1) return 0;
        } else if (at_count == 1 && email[i] == '.') {
            dot_after_at = 1;
        }
    }
    
    return at_count == 1 && dot_after_at;
}

int main(void) {
    enum state current_state = INIT;
    struct person p;
    char input[256];
    int valid_input;
    
    memset(&p, 0, sizeof(p));
    
    while (current_state != COMPLETE && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Enter person data (name, age, email)\n");
                current_state = READING_NAME;
                break;
                
            case READING_NAME:
                printf("Name: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                input[strcspn(input, "\n")] = '\0';
                if (strlen(input) >= sizeof(p.name)) {
                    printf("Name too long\n");
                    current_state = ERROR;
                    break;
                }
                strcpy(p.name, input);
                current_state = READING_AGE;
                break;
                
            case READING_AGE:
                printf("Age: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                valid_input = 0;
                if (sscanf(input, "%d", &p.age) == 1) {
                    valid_input = 1;
                }
                if (!valid_input) {
                    printf("Invalid age format\n");
                    current_state = ERROR;
                    break;
                }
                current_state = READING_EMAIL;
                break;
                
            case READING_EMAIL:
                printf("Email: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                input[strcspn(input, "\n")] = '\0';
                if (strlen(input) >= sizeof(p.email)) {
                    printf("Email too long\n");
                    current_state = ERROR;
                    break;
                }
                strcpy(p.email, input);
                current_state = VALIDATING;
                break;
                
            case VALIDATING:
                if (!validate_name(p.name)) {
                    printf("Invalid name\n");
                    current_state = ERROR;
                } else if (!validate_age(p.age)) {
                    printf("Invalid age\n");
                    current_state = ERROR;
                } else if (!validate_email(p.email)) {
                    printf("Invalid email\n");
                    current_state = ERROR;
                } else {
                    current_state = COMPLETE;
                }
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    if (current_state == COMPLETE) {
        printf("\nPerson data validated successfully:\n");
        printf("Name: %s\n", p.name);
        printf("Age: %d\n", p.age);
        printf("Email: %s\n", p.email);
    } else {
        printf("Data entry failed\n");
        return 1;
    }
    
    return 0;
}
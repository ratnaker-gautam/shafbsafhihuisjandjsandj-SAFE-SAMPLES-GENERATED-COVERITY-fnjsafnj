//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READING_NAME,
    READING_EMAIL,
    VALIDATING,
    COMPLETE,
    ERROR
};

struct user_data {
    char name[64];
    char email[128];
    int name_len;
    int email_len;
};

int is_valid_email(const char *email) {
    int at_count = 0;
    int dot_after_at = 0;
    int len = strlen(email);
    
    if (len < 3 || len > 127) return 0;
    
    for (int i = 0; i < len; i++) {
        if (email[i] == '@') {
            at_count++;
            if (i == 0 || i == len - 1) return 0;
        } else if (email[i] == '.' && at_count > 0) {
            if (i > 0 && i < len - 1) dot_after_at = 1;
        }
    }
    
    return (at_count == 1 && dot_after_at == 1);
}

int is_valid_name(const char *name) {
    int len = strlen(name);
    if (len < 1 || len > 63) return 0;
    
    for (int i = 0; i < len; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
    }
    
    return 1;
}

int main(void) {
    enum state current_state = INIT;
    struct user_data data;
    char input[256];
    int input_len;
    
    memset(&data, 0, sizeof(data));
    
    printf("User Registration System\n");
    printf("Enter 'quit' at any time to exit\n\n");
    
    while (current_state != COMPLETE && current_state != ERROR) {
        switch (current_state) {
            case INIT:
                printf("Please enter your name: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                input_len = strlen(input);
                if (input_len > 0 && input[input_len - 1] == '\n') {
                    input[input_len - 1] = '\0';
                    input_len--;
                }
                
                if (strcmp(input, "quit") == 0) {
                    printf("Goodbye!\n");
                    return 0;
                }
                
                if (input_len > 63) {
                    printf("Error: Name too long (max 63 characters)\n");
                    current_state = ERROR;
                    break;
                }
                
                strncpy(data.name, input, sizeof(data.name) - 1);
                data.name[sizeof(data.name) - 1] = '\0';
                data.name_len = input_len;
                current_state = READING_NAME;
                break;
                
            case READING_NAME:
                if (!is_valid_name(data.name)) {
                    printf("Error: Invalid name (letters and spaces only)\n");
                    current_state = ERROR;
                    break;
                }
                
                printf("Please enter your email: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    current_state = ERROR;
                    break;
                }
                
                input_len = strlen(input);
                if (input_len > 0 && input[input_len - 1] == '\n') {
                    input[input_len - 1] = '\0';
                    input_len--;
                }
                
                if (strcmp(input, "quit") == 0) {
                    printf("Goodbye!\n");
                    return 0;
                }
                
                if (input_len > 127) {
                    printf("Error: Email too long (max 127 characters)\n");
                    current_state = ERROR;
                    break;
                }
                
                strncpy(data.email, input, sizeof(data.email) - 1);
                data.email[sizeof(data.email) - 1] = '\0';
                data.email_len = input_len;
                current_state = READING_EMAIL;
                break;
                
            case READING_EMAIL:
                current_state = VALIDATING;
                break;
                
            case VALIDATING:
                if (!is_valid_email(data.email)) {
                    printf("Error: Invalid email format\n");
                    current_state = ERROR;
                    break;
                }
                
                printf("\nRegistration Complete!\n");
                printf("Name: %s\n", data.name);
                printf("Email: %s\n", data.email);
                current_state = COMPLETE;
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Registration failed due to errors.\n");
        return 1;
    }
    
    return 0;
}
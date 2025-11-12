//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    PROCESSING,
    VALIDATING,
    COMPLETED,
    ERROR
};

enum State process_state(enum State current, int input, int depth) {
    if (depth > 100) {
        return ERROR;
    }
    
    switch (current) {
        case START:
            if (input >= 0 && input <= 100) {
                return process_state(PROCESSING, input, depth + 1);
            } else {
                return ERROR;
            }
            
        case PROCESSING:
            if (input % 2 == 0) {
                return process_state(VALIDATING, input / 2, depth + 1);
            } else {
                return process_state(VALIDATING, input * 3 + 1, depth + 1);
            }
            
        case VALIDATING:
            if (input > 0 && input < 1000) {
                return process_state(COMPLETED, input, depth + 1);
            } else {
                return ERROR;
            }
            
        case COMPLETED:
            return COMPLETED;
            
        case ERROR:
            return ERROR;
    }
    
    return ERROR;
}

void print_state(enum State state) {
    switch (state) {
        case START:
            printf("START\n");
            break;
        case PROCESSING:
            printf("PROCESSING\n");
            break;
        case VALIDATING:
            printf("VALIDATING\n");
            break;
        case COMPLETED:
            printf("COMPLETED\n");
            break;
        case ERROR:
            printf("ERROR\n");
            break;
    }
}

int main() {
    char input_buffer[32];
    int user_input;
    
    printf("Enter a number between 0 and 100: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("ERROR\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%d", &user_input) != 1) {
        printf("ERROR\n");
        return 1;
    }
    
    if (user_input < 0 || user_input > 100) {
        printf("ERROR\n");
        return 1;
    }
    
    enum State result = process_state(START, user_input, 0);
    print_state(result);
    
    if (result == COMPLETED) {
        printf("Final value: %d\n", user_input);
    }
    
    return 0;
}
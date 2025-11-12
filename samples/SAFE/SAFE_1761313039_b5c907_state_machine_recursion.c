//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

enum State process_input(char input, int depth) {
    if (depth > 100) return ERROR;
    
    switch (input) {
        case 'B':
            return START;
        case 'R':
            return READING;
        case 'P':
            return PROCESSING;
        case 'F':
            return FINISHED;
        case 'E':
            return ERROR;
        default:
            return ERROR;
    }
}

enum State state_transition(enum State current, char input, int depth) {
    if (depth > 100) return ERROR;
    
    switch (current) {
        case START:
            if (input == 'R') return READING;
            break;
        case READING:
            if (input == 'P') return PROCESSING;
            if (input == 'E') return ERROR;
            break;
        case PROCESSING:
            if (input == 'F') return FINISHED;
            if (input == 'R') return READING;
            if (input == 'E') return ERROR;
            break;
        case FINISHED:
            if (input == 'B') return START;
            break;
        case ERROR:
            if (input == 'B') return START;
            break;
        default:
            return ERROR;
    }
    return current;
}

void execute_state_action(enum State state, int depth) {
    if (depth > 100) return;
    
    switch (state) {
        case START:
            printf("System started.\n");
            break;
        case READING:
            printf("Reading data...\n");
            break;
        case PROCESSING:
            printf("Processing data...\n");
            break;
        case FINISHED:
            printf("Operation completed successfully.\n");
            break;
        case ERROR:
            printf("Error occurred.\n");
            break;
    }
}

enum State run_state_machine(enum State current_state, const char* input_sequence, int position, int depth) {
    if (depth > 100 || input_sequence == NULL || position < 0) return ERROR;
    
    size_t len = strlen(input_sequence);
    if (position >= (int)len) {
        execute_state_action(current_state, depth + 1);
        return current_state;
    }
    
    char current_input = input_sequence[position];
    if (current_input == '\0') {
        execute_state_action(current_state, depth + 1);
        return current_state;
    }
    
    enum State new_state = state_transition(current_state, current_input, depth + 1);
    execute_state_action(new_state, depth + 1);
    
    return run_state_machine(new_state, input_sequence, position + 1, depth + 1);
}

int main(void) {
    char input_buffer[1024];
    printf("Enter state sequence (B=Start, R=Read, P=Process, F=Finish, E=Error): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error.\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 1 && input_buffer[0] == '\n') {
        printf("Empty input.\n");
        return 1;
    }
    
    for (size_t i = 0; i < strlen(input_buffer); i++) {
        char c = input_buffer[i];
        if (c != 'B' && c != 'R' && c != 'P' && c != 'F' && c != 'E') {
            printf("Invalid input character: %c\n", c);
            return 1;
        }
    }
    
    printf("State machine execution:\n");
    enum State final_state = run_state_machine(START, input_buffer, 0, 0);
    
    printf("Final state: ");
    switch (final_state) {
        case START: printf("START\n"); break;
        case READING: printf("READING\n"); break;
        case PROCESSING: printf("PROCESSING\n"); break;
        case FINISHED: printf("FINISHED\n"); break;
        case ERROR: printf("ERROR\n"); break;
    }
    
    return 0;
}
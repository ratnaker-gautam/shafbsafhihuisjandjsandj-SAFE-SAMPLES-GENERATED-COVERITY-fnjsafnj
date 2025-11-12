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
    
    enum State next = process_input(input, depth + 1);
    
    switch (current) {
        case START:
            if (next == READING || next == PROCESSING) return next;
            return ERROR;
        case READING:
            if (next == PROCESSING || next == FINISHED) return next;
            return ERROR;
        case PROCESSING:
            if (next == FINISHED || next == READING) return next;
            return ERROR;
        case FINISHED:
            if (next == START) return next;
            return ERROR;
        case ERROR:
            return ERROR;
    }
    return ERROR;
}

void print_state(enum State s) {
    switch (s) {
        case START: printf("START"); break;
        case READING: printf("READING"); break;
        case PROCESSING: printf("PROCESSING"); break;
        case FINISHED: printf("FINISHED"); break;
        case ERROR: printf("ERROR"); break;
    }
}

enum State process_sequence(const char* sequence, int index, enum State current, int depth) {
    if (depth > 100) return ERROR;
    if (sequence == NULL) return ERROR;
    if (index < 0) return ERROR;
    
    size_t len = strlen(sequence);
    if (index >= (int)len) return current;
    if (len > 1000) return ERROR;
    
    char input = sequence[index];
    if (input == '\0') return current;
    
    enum State next = state_transition(current, input, depth + 1);
    if (next == ERROR) return ERROR;
    
    return process_sequence(sequence, index + 1, next, depth + 1);
}

int main(void) {
    char buffer[1024];
    printf("Enter state sequence (B=START, R=READING, P=PROCESSING, F=FINISHED, E=ERROR): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("ERROR\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (strlen(buffer) == 0) {
        printf("ERROR\n");
        return 1;
    }
    
    enum State result = process_sequence(buffer, 0, START, 0);
    printf("Final state: ");
    print_state(result);
    printf("\n");
    
    return 0;
}
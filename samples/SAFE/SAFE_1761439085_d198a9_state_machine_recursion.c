//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

enum State process_input(char input, enum State current) {
    switch(current) {
        case START:
            if (input == 'B') return PROCESSING;
            if (input == 'E') return ERROR;
            break;
        case PROCESSING:
            if (input == 'C') return FINISHED;
            if (input == 'E') return ERROR;
            break;
        case FINISHED:
            if (input == 'R') return START;
            break;
        case ERROR:
            if (input == 'R') return START;
            break;
    }
    return current;
}

void execute_state_machine(const char* sequence, int pos, enum State current) {
    if (sequence == NULL) return;
    
    if (pos >= (int)strlen(sequence)) {
        printf("Final state: %d\n", current);
        return;
    }
    
    char input = sequence[pos];
    if (input < 32 || input > 126) {
        printf("Invalid character at position %d\n", pos);
        return;
    }
    
    enum State next = process_input(input, current);
    
    printf("Position %d: Input '%c' -> State %d\n", pos, input, next);
    
    execute_state_machine(sequence, pos + 1, next);
}

int main(void) {
    char buffer[256];
    printf("Enter state machine input sequence: ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (strlen(buffer) == 0) {
        fprintf(stderr, "Empty input sequence\n");
        return 1;
    }
    
    if (strlen(buffer) >= 255) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    printf("Starting state machine...\n");
    execute_state_machine(buffer, 0, START);
    
    return 0;
}
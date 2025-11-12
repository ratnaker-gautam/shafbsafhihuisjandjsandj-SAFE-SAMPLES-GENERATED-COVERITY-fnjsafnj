//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR };

struct StateMachine {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int data_count;
};

typedef void (*StateHandler)(struct StateMachine*);

void handle_idle(struct StateMachine* sm) {
    printf("Enter command (read/quit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current_state = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "read") == 0) {
        sm->buffer_pos = 0;
        sm->data_count = 0;
        sm->current_state = READING;
    } else if (strcmp(input, "quit") == 0) {
        exit(0);
    } else {
        printf("Unknown command\n");
    }
}

void handle_reading(struct StateMachine* sm) {
    printf("Enter data (max 255 chars, 'end' to finish): ");
    char input[256];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current_state = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "end") == 0) {
        if (sm->buffer_pos > 0) {
            sm->current_state = PROCESSING;
        } else {
            printf("No data entered\n");
            sm->current_state = IDLE;
        }
        return;
    }
    
    size_t input_len = strlen(input);
    if (sm->buffer_pos + input_len >= sizeof(sm->buffer) - 1) {
        printf("Buffer full\n");
        sm->current_state = PROCESSING;
        return;
    }
    
    memcpy(sm->buffer + sm->buffer_pos, input, input_len);
    sm->buffer_pos += input_len;
    sm->buffer[sm->buffer_pos] = ' ';
    sm->buffer_pos++;
    sm->data_count++;
}

void handle_processing(struct StateMachine* sm) {
    if (sm->buffer_pos > 0 && sm->buffer[sm->buffer_pos - 1] == ' ') {
        sm->buffer[sm->buffer_pos - 1] = '\0';
    }
    
    printf("Processing %d data items...\n", sm->data_count);
    
    for (size_t i = 0; i < sm->buffer_pos; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    sm->current_state = WRITING;
}

void handle_writing(struct StateMachine* sm) {
    printf("Processed data: %s\n", sm->buffer);
    printf("Total items processed: %d\n", sm->data_count);
    sm->current_state = IDLE;
}

void handle_error(struct StateMachine* sm) {
    printf("Error occurred. Resetting state machine.\n");
    sm->current_state = IDLE;
    sm->buffer_pos = 0;
    sm->data_count = 0;
}

int main() {
    struct StateMachine sm;
    sm.current_state = IDLE;
    sm.buffer_pos = 0;
    sm.data_count = 0;
    
    StateHandler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    while (1) {
        if (sm.current_state >= 0 && sm.current_state <= ERROR) {
            handlers[sm.current_state](&sm);
        } else {
            sm.current_state = ERROR;
        }
    }
    
    return 0;
}
//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define EVENT_START 0
#define EVENT_DATA 1
#define EVENT_END 2
#define EVENT_RESET 3
#define EVENT_INVALID 4

#define MAX_INPUT_LEN 100
#define MAX_DATA_LEN 50

typedef struct {
    int state;
    char data[MAX_DATA_LEN];
    size_t data_len;
} StateMachine;

const char* state_names[] = {
    "IDLE", "READING", "PROCESSING", "DONE", "ERROR"
};

const char* event_names[] = {
    "START", "DATA", "END", "RESET", "INVALID"
};

void state_machine_init(StateMachine* sm) {
    sm->state = STATE_IDLE;
    sm->data_len = 0;
    memset(sm->data, 0, sizeof(sm->data));
}

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_LEN) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) {
            return 0;
        }
    }
    return 1;
}

int process_event(StateMachine* sm, int event, const char* input, size_t len) {
    if (sm == NULL || !validate_input(input, len)) {
        return STATE_ERROR;
    }

    switch (sm->state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->state = STATE_READING;
                sm->data_len = 0;
            } else if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
            } else {
                sm->state = STATE_ERROR;
            }
            break;

        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->data_len + len < MAX_DATA_LEN) {
                    memcpy(sm->data + sm->data_len, input, len);
                    sm->data_len += len;
                } else {
                    sm->state = STATE_ERROR;
                }
            } else if (event == EVENT_END) {
                sm->state = STATE_PROCESSING;
            } else if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
            } else {
                sm->state = STATE_ERROR;
            }
            break;

        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
            } else if (event == EVENT_END) {
                if (sm->data_len > 0) {
                    sm->state = STATE_DONE;
                } else {
                    sm->state = STATE_ERROR;
                }
            } else {
                sm->state = STATE_ERROR;
            }
            break;

        case STATE_DONE:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
            } else {
                sm->state = STATE_ERROR;
            }
            break;

        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
            }
            break;

        default:
            sm->state = STATE_ERROR;
            break;
    }

    return sm->state;
}

void print_state_info(const StateMachine* sm) {
    if (sm->state >= STATE_IDLE && sm->state <= STATE_ERROR) {
        printf("Current state: %s\n", state_names[sm->state]);
        if (sm->state == STATE_DONE && sm->data_len > 0) {
            printf("Processed data: %.*s\n", (int)sm->data_len, sm->data);
        }
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input_buffer[MAX_INPUT_LEN];
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Commands: start, data <text>, end, reset, quit\n");
    
    while (running) {
        printf("\n> ");
        fflush(stdout);
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        int event = EVENT_INVALID;
        const char* data_ptr = NULL;
        size_t data_len = 0;
        
        if (strcmp(input_buffer, "start") == 0) {
            event = EVENT_START;
        } else if (strcmp(input_buffer, "end") == 0) {
            event = EVENT_END;
        } else if (strcmp(input_buffer, "reset") == 0) {
            event = EVENT_RESET;
        } else if (strcmp(input_buffer, "quit") == 0) {
            running = 0;
            continue;
        } else if
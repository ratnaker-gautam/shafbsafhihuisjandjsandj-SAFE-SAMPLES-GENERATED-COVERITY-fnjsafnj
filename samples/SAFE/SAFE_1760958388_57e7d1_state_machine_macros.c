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
    int current_state;
    char data_buffer[MAX_DATA_LEN + 1];
    size_t data_len;
} state_machine_t;

static int get_next_event(const char* input, size_t* pos) {
    if (*pos >= strlen(input)) return EVENT_END;
    
    char c = input[*pos];
    (*pos)++;
    
    if (c == 'S') return EVENT_START;
    if (c == 'D') return EVENT_DATA;
    if (c == 'E') return EVENT_END;
    if (c == 'R') return EVENT_RESET;
    
    return EVENT_INVALID;
}

static void state_machine_init(state_machine_t* sm) {
    sm->current_state = STATE_IDLE;
    sm->data_buffer[0] = '\0';
    sm->data_len = 0;
}

static int state_machine_step(state_machine_t* sm, int event, char data_char) {
    int next_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->data_buffer[0] = '\0';
                sm->data_len = 0;
                next_state = STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->data_len < MAX_DATA_LEN && isalnum(data_char)) {
                    sm->data_buffer[sm->data_len] = data_char;
                    sm->data_len++;
                    sm->data_buffer[sm->data_len] = '\0';
                }
                next_state = STATE_READING;
            } else if (event == EVENT_END) {
                if (sm->data_len > 0) {
                    next_state = STATE_PROCESSING;
                } else {
                    next_state = STATE_ERROR;
                }
            } else if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
            } else {
                next_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
            } else {
                next_state = STATE_DONE;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
            }
            break;
            
        default:
            next_state = STATE_ERROR;
            break;
    }
    
    return next_state;
}

static void process_data(const char* data) {
    printf("Processing: ");
    for (size_t i = 0; i < strlen(data); i++) {
        printf("%c", toupper(data[i]));
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    state_machine_t sm;
    
    printf("Enter sequence (S=Start, Dx=Data, E=End, R=Reset): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    state_machine_init(&sm);
    
    size_t pos = 0;
    while (pos < strlen(input) && sm.current_state != STATE_ERROR && sm.current_state != STATE_DONE) {
        int event = get_next_event(input, &pos);
        char data_char = '\0';
        
        if (event == EVENT_DATA && pos < strlen(input)) {
            data_char = input[pos];
            pos++;
        }
        
        int next_state = state_machine_step(&sm, event, data_char);
        
        if (next_state == STATE_PROCESSING) {
            process_data(sm.data_buffer);
            next_state = STATE_DONE;
        }
        
        sm.current_state = next_state;
        
        if (sm.current_state == STATE_ERROR) {
            fprintf(stderr, "State machine error at position %zu\n", pos);
            break;
        }
    }
    
    if (sm.current_state == STATE_DONE) {
        printf("Sequence completed successfully\n");
    } else if (sm.current_state != STATE_ERROR) {
        fprintf(stderr, "Unexpected termination in state %d\n", sm.current_state);
    }
    
    return 0;
}
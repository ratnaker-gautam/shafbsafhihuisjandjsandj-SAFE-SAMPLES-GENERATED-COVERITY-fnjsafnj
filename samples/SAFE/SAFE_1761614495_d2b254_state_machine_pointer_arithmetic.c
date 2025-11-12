//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_END
} State;

typedef struct {
    State current;
    char buffer[MAX_INPUT];
    size_t position;
    int result;
} StateMachine;

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current = STATE_START;
    sm->position = 0;
    sm->result = 0;
    memset(sm->buffer, 0, MAX_INPUT);
}

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT) return 0;
    
    const char *ptr = input;
    while (*ptr != '\0') {
        if (!isdigit(*ptr) && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') {
            return 0;
        }
        ptr++;
    }
    return 1;
}

State process_state_start(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return STATE_END;
    
    if (!is_valid_input(input)) {
        return STATE_END;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT) {
        return STATE_END;
    }
    
    memcpy(sm->buffer, input, input_len);
    *(sm->buffer + input_len) = '\0';
    sm->position = 0;
    
    return STATE_READING;
}

State process_state_reading(StateMachine *sm) {
    if (sm == NULL) return STATE_END;
    
    char *ptr = sm->buffer + sm->position;
    while (*ptr != '\0' && isspace(*ptr)) {
        ptr++;
        sm->position++;
    }
    
    if (*ptr == '\0') {
        return STATE_OUTPUT;
    }
    
    return STATE_PROCESSING;
}

State process_state_processing(StateMachine *sm) {
    if (sm == NULL) return STATE_END;
    
    char *start = sm->buffer + sm->position;
    char *end = start;
    
    while (*end != '\0' && isdigit(*end)) {
        end++;
    }
    
    if (end > start) {
        char num_buf[32];
        size_t num_len = end - start;
        if (num_len >= sizeof(num_buf)) {
            return STATE_END;
        }
        
        memcpy(num_buf, start, num_len);
        num_buf[num_len] = '\0';
        
        int num = atoi(num_buf);
        if (num > 0) {
            sm->result += num;
        }
        
        sm->position += num_len;
    }
    
    return STATE_READING;
}

State process_state_output(StateMachine *sm) {
    if (sm == NULL) return STATE_END;
    
    printf("Sum: %d\n", sm->result);
    return STATE_END;
}

void run_state_machine(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return;
    
    sm->current = STATE_START;
    
    while (sm->current != STATE_END) {
        switch (sm->current) {
            case STATE_START:
                sm->current = process_state_start(sm, input);
                break;
            case STATE_READING:
                sm->current = process_state_reading(sm);
                break;
            case STATE_PROCESSING:
                sm->current = process_state_processing(sm);
                break;
            case STATE_OUTPUT:
                sm->current = process_state_output(sm);
                break;
            case STATE_END:
                break;
        }
    }
}

int main(void) {
    char input[MAX_INPUT];
    StateMachine sm;
    
    printf("Enter numbers separated by spaces: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    state_machine_init(&sm);
    run_state_machine(&sm, input);
    
    return 0;
}
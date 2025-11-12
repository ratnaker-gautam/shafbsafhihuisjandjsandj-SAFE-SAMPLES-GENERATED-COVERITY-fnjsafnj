//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
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
    char *input;
    size_t pos;
    size_t len;
    int result;
} StateMachine;

void state_start(StateMachine *sm);
void state_reading(StateMachine *sm);
void state_processing(StateMachine *sm);
void state_output(StateMachine *sm);
void state_end(StateMachine *sm);

void (*state_handlers[STATE_COUNT])(StateMachine *) = {
    state_start,
    state_reading,
    state_processing,
    state_output,
    state_end
};

void state_start(StateMachine *sm) {
    sm->current = STATE_READING;
    sm->pos = 0;
    sm->len = 0;
    sm->result = 0;
}

void state_reading(StateMachine *sm) {
    char buffer[MAX_INPUT_LEN + 1];
    printf("Enter a string (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        sm->current = STATE_END;
        return;
    }
    
    size_t input_len = strlen(buffer);
    if (input_len > 0 && *(buffer + input_len - 1) == '\n') {
        *(buffer + input_len - 1) = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        sm->current = STATE_END;
        return;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        sm->current = STATE_END;
        return;
    }
    
    sm->input = malloc(input_len + 1);
    if (sm->input == NULL) {
        sm->current = STATE_END;
        return;
    }
    
    char *src = buffer;
    char *dst = sm->input;
    while (*src != '\0') {
        *dst = *src;
        src++;
        dst++;
    }
    *dst = '\0';
    
    sm->len = input_len;
    sm->current = STATE_PROCESSING;
}

void state_processing(StateMachine *sm) {
    if (sm->input == NULL || sm->len == 0) {
        sm->current = STATE_END;
        return;
    }
    
    int sum = 0;
    char *ptr = sm->input;
    char *end = sm->input + sm->len;
    
    while (ptr < end) {
        if (*ptr >= '0' && *ptr <= '9') {
            sum += (*ptr - '0');
        }
        ptr++;
    }
    
    sm->result = sum;
    sm->current = STATE_OUTPUT;
}

void state_output(StateMachine *sm) {
    if (sm->input != NULL) {
        printf("Input: %s\n", sm->input);
        printf("Sum of digits: %d\n", sm->result);
        free(sm->input);
        sm->input = NULL;
    }
    sm->current = STATE_END;
}

void state_end(StateMachine *sm) {
    sm->current = STATE_END;
}

int main(void) {
    StateMachine sm;
    
    sm.current = STATE_START;
    sm.input = NULL;
    sm.pos = 0;
    sm.len = 0;
    sm.result = 0;
    
    while (sm.current != STATE_END) {
        if (sm.current >= 0 && sm.current < STATE_COUNT) {
            state_handlers[sm.current](&sm);
        } else {
            break;
        }
    }
    
    if (sm.input != NULL) {
        free(sm.input);
    }
    
    return 0;
}
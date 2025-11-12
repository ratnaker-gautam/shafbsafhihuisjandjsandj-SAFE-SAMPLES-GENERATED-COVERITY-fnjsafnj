//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3

typedef struct {
    int current_state;
    int input_index;
    int data_count;
    int data_sum;
    char input_buffer[MAX_INPUT_LEN + 1];
} state_machine_t;

void state_machine_init(state_machine_t *sm) {
    sm->current_state = STATE_IDLE;
    sm->input_index = 0;
    sm->data_count = 0;
    sm->data_sum = 0;
    memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
}

int is_valid_input_char(char c) {
    return isdigit(c) || c == ' ' || c == '\n' || c == '\0';
}

int process_state_idle(state_machine_t *sm, char input) {
    if (isdigit(input)) {
        sm->input_buffer[sm->input_index++] = input;
        sm->current_state = STATE_READING;
        return 1;
    }
    return 0;
}

int process_state_reading(state_machine_t *sm, char input) {
    if (isdigit(input)) {
        if (sm->input_index < MAX_INPUT_LEN) {
            sm->input_buffer[sm->input_index++] = input;
        }
        return 1;
    } else if (input == ' ' || input == '\n') {
        sm->input_buffer[sm->input_index] = '\0';
        if (sm->input_index > 0) {
            sm->current_state = STATE_PROCESSING;
        } else {
            sm->current_state = STATE_IDLE;
        }
        return 1;
    }
    return 0;
}

int process_state_processing(state_machine_t *sm) {
    char *endptr;
    long value = strtol(sm->input_buffer, &endptr, 10);
    if (endptr != sm->input_buffer && *endptr == '\0' && value >= 0 && value <= 1000000) {
        sm->data_sum += (int)value;
        sm->data_count++;
    }
    sm->input_index = 0;
    memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
    sm->current_state = STATE_IDLE;
    return 1;
}

int state_machine_step(state_machine_t *sm, char input) {
    if (!is_valid_input_char(input)) {
        return 0;
    }
    
    switch (sm->current_state) {
        case STATE_IDLE:
            return process_state_idle(sm, input);
        case STATE_READING:
            return process_state_reading(sm, input);
        case STATE_PROCESSING:
            return process_state_processing(sm);
        default:
            return 0;
    }
}

void perform_cpu_bound_work(int iterations) {
    volatile int result = 0;
    for (int i = 0; i < iterations; i++) {
        result += i * i;
        if (result > 1000000) {
            result = result % 1000;
        }
    }
}

int main(void) {
    state_machine_t sm;
    state_machine_init(&sm);
    
    printf("Enter numbers separated by spaces (max %d chars): ", MAX_INPUT_LEN);
    fflush(stdout);
    
    char input;
    int char_count = 0;
    
    while (char_count < MAX_INPUT_LEN) {
        int read_result = read(0, &input, 1);
        if (read_result != 1) {
            break;
        }
        
        char_count++;
        
        if (!state_machine_step(&sm, input)) {
            printf("Invalid input character: %c\n", input);
            return 1;
        }
        
        if (input == '\n') {
            if (sm.current_state == STATE_READING) {
                state_machine_step(&sm, ' ');
            }
            break;
        }
        
        if (sm.current_state == STATE_PROCESSING) {
            perform_cpu_bound_work(1000);
        }
    }
    
    if (char_count >= MAX_INPUT_LEN && sm.current_state == STATE_READING) {
        state_machine_step(&sm, ' ');
    }
    
    if (sm.data_count > 0) {
        printf("Processed %d numbers. Sum: %d. Average: %.2f\n", 
               sm.data_count, sm.data_sum, (double)sm.data_sum / sm.data_count);
        perform_cpu_bound_work(5000);
    } else {
        printf("No valid numbers processed.\n");
    }
    
    return 0;
}
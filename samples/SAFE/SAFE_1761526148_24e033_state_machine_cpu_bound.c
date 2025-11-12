//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define STATE_COUNT 5
#define MAX_INPUTS 1000

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E
} state_t;

typedef enum {
    INPUT_X,
    INPUT_Y,
    INPUT_Z
} input_t;

static state_t transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static int process_state_machine(const input_t *inputs, size_t input_count) {
    state_t current_state = STATE_A;
    int cycle_count = 0;
    
    for (size_t i = 0; i < input_count; i++) {
        if (inputs[i] >= 3) {
            continue;
        }
        
        current_state = transition_table[current_state][inputs[i]];
        cycle_count++;
        
        if (current_state == STATE_E) {
            cycle_count += (int)(input_count - i - 1);
            break;
        }
    }
    
    return cycle_count;
}

static int validate_and_parse_inputs(const char *input_str, input_t *inputs, size_t max_inputs) {
    if (input_str == NULL || inputs == NULL || max_inputs == 0) {
        return -1;
    }
    
    size_t len = strlen(input_str);
    if (len == 0 || len > max_inputs) {
        return -1;
    }
    
    for (size_t i = 0; i < len; i++) {
        switch (input_str[i]) {
            case 'x': case 'X': inputs[i] = INPUT_X; break;
            case 'y': case 'Y': inputs[i] = INPUT_Y; break;
            case 'z': case 'Z': inputs[i] = INPUT_Z; break;
            default: return -1;
        }
    }
    
    return (int)len;
}

int main(void) {
    char input_buffer[1024];
    input_t inputs[MAX_INPUTS];
    
    printf("Enter state machine inputs (x/y/z, max %d): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    int input_count = validate_and_parse_inputs(input_buffer, inputs, MAX_INPUTS);
    if (input_count <= 0) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    int result = process_state_machine(inputs, (size_t)input_count);
    
    printf("State machine processed %d inputs with %d computation cycles\n", input_count, result);
    
    return 0;
}
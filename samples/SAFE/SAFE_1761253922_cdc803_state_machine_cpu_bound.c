//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define MAX_INPUTS 256

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
} state_t;

typedef struct {
    state_t current;
    char input;
    state_t next;
} transition_t;

typedef struct {
    state_t current_state;
    transition_t transitions[MAX_TRANSITIONS];
    int transition_count;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
    
    transition_t trans[] = {
        {STATE_A, '0', STATE_B}, {STATE_A, '1', STATE_C},
        {STATE_B, '0', STATE_D}, {STATE_B, '1', STATE_E},
        {STATE_C, '0', STATE_F}, {STATE_C, '1', STATE_G},
        {STATE_D, '0', STATE_H}, {STATE_D, '1', STATE_A},
        {STATE_E, '0', STATE_B}, {STATE_E, '1', STATE_C},
        {STATE_F, '0', STATE_D}, {STATE_F, '1', STATE_E},
        {STATE_G, '0', STATE_F}, {STATE_G, '1', STATE_G},
        {STATE_H, '0', STATE_H}, {STATE_H, '1', STATE_A}
    };
    
    int count = sizeof(trans) / sizeof(trans[0]);
    if (count > MAX_TRANSITIONS) count = MAX_TRANSITIONS;
    
    for (int i = 0; i < count; i++) {
        if (machine->transition_count < MAX_TRANSITIONS) {
            machine->transitions[machine->transition_count] = trans[i];
            machine->transition_count++;
        }
    }
}

static state_t process_input(state_machine_t *machine, char input) {
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state &&
            machine->transitions[i].input == input) {
            return machine->transitions[i].next;
        }
    }
    return machine->current_state;
}

static const char* state_name(state_t state) {
    switch(state) {
        case STATE_A: return "A";
        case STATE_B: return "B";
        case STATE_C: return "C";
        case STATE_D: return "D";
        case STATE_E: return "E";
        case STATE_F: return "F";
        case STATE_G: return "G";
        case STATE_H: return "H";
        default: return "UNKNOWN";
    }
}

static int validate_input_string(const char *input) {
    if (input == NULL) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUTS) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] != '0' && input[i] != '1') return 0;
    }
    
    return 1;
}

static void perform_cpu_work(int iterations) {
    volatile uint64_t result = 0;
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < 1000; j++) {
            result += (i * j) % 997;
        }
    }
}

int main(void) {
    char input_buffer[MAX_INPUTS + 2];
    state_machine_t machine;
    
    initialize_machine(&machine);
    
    printf("Enter binary input sequence (max %d chars, only 0/1): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input_string(input_buffer)) {
        fprintf(stderr, "Invalid input: must contain only 0 and 1 characters\n");
        return EXIT_FAILURE;
    }
    
    printf("Initial state: %s\n", state_name(machine.current_state));
    printf("Processing input: %s\n", input_buffer);
    
    for (size_t i = 0; i < len; i++) {
        state_t new_state = process_input(&machine, input_buffer[i]);
        printf("Input '%c': %s -> %s\n", 
               input_buffer[i], 
               state_name(machine.current_state), 
               state_name(new_state));
        machine.current_state = new_state;
        
        perform_cpu_work(1000);
    }
    
    printf("Final state: %s\n", state_name(machine.current_state));
    
    return EXIT_SUCCESS;
}
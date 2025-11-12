//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    Transition transitions[MAX_TRANSITIONS];
    size_t num_transitions;
} StateMachine;

void initialize_state_machine(StateMachine* sm) {
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->num_transitions = 0;
    
    Transition trans[] = {
        {STATE_IDLE, 'S', STATE_READING},
        {STATE_READING, 'E', STATE_PROCESSING},
        {STATE_READING, 'X', STATE_ERROR},
        {STATE_PROCESSING, 'C', STATE_DONE},
        {STATE_PROCESSING, 'X', STATE_ERROR},
        {STATE_ERROR, 'R', STATE_IDLE},
        {STATE_DONE, 'R', STATE_IDLE}
    };
    
    size_t num = sizeof(trans) / sizeof(trans[0]);
    if (num > MAX_TRANSITIONS) {
        num = MAX_TRANSITIONS;
    }
    
    for (size_t i = 0; i < num; i++) {
        sm->transitions[i] = trans[i];
        sm->num_transitions++;
    }
}

State get_next_state(const StateMachine* sm, State current, char input) {
    for (size_t i = 0; i < sm->num_transitions; i++) {
        if (sm->transitions[i].current == current && 
            sm->transitions[i].input == input) {
            return sm->transitions[i].next;
        }
    }
    return current;
}

int handle_state_action(StateMachine* sm, char input) {
    switch (sm->current_state) {
        case STATE_IDLE:
            if (input == 'S') {
                sm->buffer_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
            }
            break;
            
        case STATE_READING:
            if (sm->buffer_pos < MAX_INPUT_LEN - 1) {
                sm->buffer[sm->buffer_pos++] = input;
                sm->buffer[sm->buffer_pos] = '\0';
            }
            break;
            
        case STATE_PROCESSING:
            for (size_t i = 0; i < sm->buffer_pos; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                }
            }
            break;
            
        case STATE_DONE:
            printf("Processed: %s\n", sm->buffer);
            break;
            
        case STATE_ERROR:
            printf("Error state reached. Press 'R' to reset.\n");
            break;
            
        default:
            break;
    }
    
    return 1;
}

void print_state_info(State state) {
    const char* state_names[] = {
        "IDLE", "READING", "PROCESSING", "DONE", "ERROR"
    };
    
    if (state >= STATE_IDLE && state <= STATE_ERROR) {
        printf("Current state: %s\n", state_names[state]);
    }
}

void print_instructions(void) {
    printf("State Machine Controller\n");
    printf("Commands: S=Start, E=End input, C=Complete, R=Reset, X=Error\n");
    printf("Type 'Q' to quit\n");
    printf("Enter commands one at a time:\n");
}

int main(void) {
    StateMachine sm;
    char input;
    int running = 1;
    
    initialize_state_machine(&sm);
    print_instructions();
    
    while (running) {
        print_state_info(sm.current_state);
        printf("Enter command: ");
        
        int result = scanf(" %c", &input);
        if (result != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a single character.\n");
            continue;
        }
        
        input = toupper((unsigned char)input);
        
        if (input == 'Q') {
            running = 0;
            continue;
        }
        
        if (!isalpha((unsigned char)input)) {
            printf("Invalid command. Please enter a letter.\n");
            continue;
        }
        
        State next_state = get_next_state(&sm, sm.current_state, input);
        
        if (handle_state_action(&sm, input)) {
            sm.current_state = next_state;
        }
        
        printf("\n");
    }
    
    printf("State machine terminated.\n");
    return 0;
}
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
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_FINISH,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    Event event;
    State next_state;
} Transition;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    Transition transitions[MAX_TRANSITIONS];
    size_t num_transitions;
} StateMachine;

void initialize_state_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->num_transitions = 0;
    
    Transition transitions[] = {
        {STATE_IDLE, EVENT_START, STATE_READING},
        {STATE_READING, EVENT_DATA, STATE_READING},
        {STATE_READING, EVENT_PROCESS, STATE_PROCESSING},
        {STATE_PROCESSING, EVENT_FINISH, STATE_COMPLETE},
        {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
        {STATE_ERROR, EVENT_RESET, STATE_IDLE},
        {STATE_COMPLETE, EVENT_RESET, STATE_IDLE}
    };
    
    size_t count = sizeof(transitions) / sizeof(transitions[0]);
    if (count > MAX_TRANSITIONS) count = MAX_TRANSITIONS;
    
    for (size_t i = 0; i < count; i++) {
        sm->transitions[sm->num_transitions++] = transitions[i];
    }
}

State handle_event(StateMachine *sm, Event event) {
    for (size_t i = 0; i < sm->num_transitions; i++) {
        if (sm->transitions[i].current_state == sm->current_state &&
            sm->transitions[i].event == event) {
            return sm->transitions[i].next_state;
        }
    }
    return sm->current_state;
}

int is_valid_input_char(char c) {
    return isalnum(c) || c == ' ' || c == '.' || c == ',';
}

int process_buffer_data(StateMachine *sm) {
    if (sm->buffer_pos == 0) return 0;
    
    int digit_count = 0;
    int letter_count = 0;
    
    for (size_t i = 0; i < sm->buffer_pos; i++) {
        if (isdigit(sm->buffer[i])) digit_count++;
        else if (isalpha(sm->buffer[i])) letter_count++;
    }
    
    printf("Processing complete: %zu characters analyzed\n", sm->buffer_pos);
    printf("Digits: %d, Letters: %d\n", digit_count, letter_count);
    
    return 1;
}

void reset_state_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
}

void print_state_info(State state) {
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "COMPLETE", "ERROR"
    };
    printf("Current state: %s\n", state_names[state]);
}

int main(void) {
    StateMachine sm;
    initialize_state_machine(&sm);
    
    char input[MAX_INPUT_LEN + 2];
    int running = 1;
    
    printf("State Machine Demo - Character Analyzer\n");
    printf("Commands: start, data <text>, process, finish, error, reset, quit\n");
    
    while (running) {
        print_state_info(sm.current_state);
        printf("> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strlen(input) == 0) continue;
        
        if (strcmp(input, "quit") == 0) {
            running = 0;
            continue;
        }
        
        if (strcmp(input, "start") == 0) {
            sm.current_state = handle_event(&sm, EVENT_START);
        }
        else if (strncmp(input, "data ", 5) == 0) {
            if (sm.current_state != STATE_READING && sm.current_state != STATE_IDLE) {
                printf("Error: Cannot accept data in current state\n");
                continue;
            }
            
            const char *text = input + 5;
            size_t text_len = strlen(text);
            
            if (text_len == 0) {
                printf("Error: No data provided\n");
                continue;
            }
            
            int valid = 1;
            for (size_t i = 0; i < text_len; i++) {
                if (!is_valid_input_char(text[i])) {
                    printf("Error: Invalid character '%c'\n", text[i]);
                    valid = 0;
                    break;
                }
            }
            
            if (!valid) continue;
            
            if (sm.buffer_pos + text_len >= MAX_INPUT_LEN) {
                printf("Error: Buffer full\n");
                sm.current_state = handle_event(&sm,
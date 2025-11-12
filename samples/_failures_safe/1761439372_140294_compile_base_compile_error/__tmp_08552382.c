//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_VALIDATING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef struct {
    State current_state;
    State next_state;
    char trigger_char;
} Transition;

typedef struct {
    State current_state;
    char input_buffer[MAX_INPUT_LEN + 1];
    size_t input_len;
    Transition transitions[MAX_TRANSITIONS];
    size_t num_transitions;
    int processed_count;
} StateMachine;

void initialize_state_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->input_len = 0;
    sm->processed_count = 0;
    sm->num_transitions = 0;
    
    Transition trans[] = {
        {STATE_IDLE, STATE_PROCESSING, 'S'},
        {STATE_PROCESSING, STATE_VALIDATING, 'V'},
        {STATE_VALIDATING, STATE_COMPLETE, 'C'},
        {STATE_VALIDATING, STATE_ERROR, 'E'},
        {STATE_COMPLETE, STATE_IDLE, 'R'},
        {STATE_ERROR, STATE_IDLE, 'R'}
    };
    
    size_t num = sizeof(trans) / sizeof(trans[0]);
    if (num > MAX_TRANSITIONS) num = MAX_TRANSITIONS;
    
    for (size_t i = 0; i < num; i++) {
        sm->transitions[sm->num_transitions++] = trans[i];
    }
}

State get_next_state(StateMachine *sm, char trigger) {
    for (size_t i = 0; i < sm->num_transitions; i++) {
        if (sm->transitions[i].current_state == sm->current_state &&
            sm->transitions[i].trigger_char == trigger) {
            return sm->transitions[i].next_state;
        }
    }
    return sm->current_state;
}

void process_state(StateMachine *sm) {
    switch (sm->current_state) {
        case STATE_IDLE:
            break;
        case STATE_PROCESSING:
            for (size_t i = 0; i < sm->input_len; i++) {
                if (sm->input_buffer[i] != '\0') {
                    sm->input_buffer[i] = toupper((unsigned char)sm->input_buffer[i]);
                }
            }
            sm->processed_count++;
            break;
        case STATE_VALIDATING:
            for (size_t i = 0; i < sm->input_len; i++) {
                if (!isalnum((unsigned char)sm->input_buffer[i]) && sm->input_buffer[i] != ' ') {
                    sm->current_state = STATE_ERROR;
                    return;
                }
            }
            break;
        case STATE_COMPLETE:
            printf("Processing complete. Result: %s\n", sm->input_buffer);
            printf("Total processed: %d\n", sm->processed_count);
            break;
        case STATE_ERROR:
            printf("Error: Invalid input detected\n");
            break;
    }
}

int read_input(char *buffer, size_t max_len) {
    if (max_len == 0) return 0;
    
    printf("Enter input (max %zu chars): ", max_len - 1);
    
    if (fgets(buffer, (int)max_len, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    return (int)len;
}

int main(void) {
    StateMachine sm;
    initialize_state_machine(&sm);
    
    printf("State Machine Processor\n");
    printf("Commands: S=Start, V=Validate, C=Complete, E=Error, R=Reset, Q=Quit\n");
    
    char command;
    int running = 1;
    
    while (running) {
        printf("\nCurrent state: ");
        switch (sm.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_VALIDATING: printf("VALIDATING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\nEnter command: ");
        
        if (scanf(" %c", &command) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        
        if (command == 'Q' || command == 'q') {
            running = 0;
            continue;
        }
        
        if (sm.current_state == STATE_IDLE && (command == 'S' || command == 's')) {
            sm.input_len = read_input(sm.input_buffer, sizeof(sm.input_buffer));
            if (sm.input_len > 0) {
                command = 'S';
            } else {
                printf("No input received\n");
                continue;
            }
        }
        
        State next_state = get_next_state(&sm, command);
        if (next_state != sm.current_state) {
            sm.current_state = next_state;
            process_state(&sm);
        } else {
            printf("Invalid transition from
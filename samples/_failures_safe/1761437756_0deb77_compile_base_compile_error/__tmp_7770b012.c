//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_TOKENS 10

#define TRANSITION(from, to) ((from) * 10 + (to))

typedef struct {
    int state;
    char input[MAX_INPUT_LEN];
    char tokens[MAX_TOKENS][20];
    int token_count;
} StateMachine;

void initialize_machine(StateMachine *sm) {
    sm->state = STATE_IDLE;
    memset(sm->input, 0, MAX_INPUT_LEN);
    for (int i = 0; i < MAX_TOKENS; i++) {
        memset(sm->tokens[i], 0, 20);
    }
    sm->token_count = 0;
}

int validate_transition(int from, int to) {
    int valid_transitions[] = {
        TRANSITION(STATE_IDLE, STATE_READING),
        TRANSITION(STATE_READING, STATE_PROCESSING),
        TRANSITION(STATE_PROCESSING, STATE_DONE),
        TRANSITION(STATE_PROCESSING, STATE_ERROR),
        TRANSITION(STATE_READING, STATE_ERROR),
        TRANSITION(STATE_ERROR, STATE_IDLE),
        TRANSITION(STATE_DONE, STATE_IDLE)
    };
    
    int transition = TRANSITION(from, to);
    for (size_t i = 0; i < sizeof(valid_transitions)/sizeof(valid_transitions[0]); i++) {
        if (valid_transitions[i] == transition) {
            return 1;
        }
    }
    return 0;
}

int change_state(StateMachine *sm, int new_state) {
    if (!validate_transition(sm->state, new_state)) {
        return 0;
    }
    sm->state = new_state;
    return 1;
}

int read_input(StateMachine *sm) {
    if (sm->state != STATE_READING) {
        return 0;
    }
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    if (fgets(sm->input, MAX_INPUT_LEN, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(sm->input);
    if (len > 0 && sm->input[len - 1] == '\n') {
        sm->input[len - 1] = '\0';
    }
    
    if (strlen(sm->input) == 0) {
        return 0;
    }
    
    return 1;
}

int process_input(StateMachine *sm) {
    if (sm->state != STATE_PROCESSING) {
        return 0;
    }
    
    char *token;
    char *rest = sm->input;
    sm->token_count = 0;
    
    while ((token = strtok_r(rest, " ", &rest)) != NULL) {
        if (sm->token_count >= MAX_TOKENS) {
            break;
        }
        if (strlen(token) >= 20) {
            return 0;
        }
        strncpy(sm->tokens[sm->token_count], token, 19);
        sm->tokens[sm->token_count][19] = '\0';
        sm->token_count++;
    }
    
    return sm->token_count > 0;
}

void print_results(StateMachine *sm) {
    if (sm->state == STATE_DONE) {
        printf("Processing complete. Found %d tokens:\n", sm->token_count);
        for (int i = 0; i < sm->token_count; i++) {
            printf("  %d: '%s'\n", i + 1, sm->tokens[i]);
        }
    } else if (sm->state == STATE_ERROR) {
        printf("Error occurred during processing.\n");
    }
}

int main(void) {
    StateMachine sm;
    initialize_machine(&sm);
    char command[10];
    
    printf("State Machine Demo - Commands: start, process, reset, quit\n");
    
    while (1) {
        printf("\nCurrent state: ");
        switch (sm.state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_DONE: printf("DONE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\nEnter command: ");
        
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }
        
        command[strcspn(command, "\n")] = '\0';
        
        if (strcmp(command, "quit") == 0) {
            break;
        } else if (strcmp(command, "start") == 0) {
            if (change_state(&sm, STATE_READING)) {
                if (!read_input(&sm)) {
                    change_state(&sm, STATE_ERROR);
                } else {
                    change_state(&sm, STATE_PROCESSING);
                }
            }
        } else if (strcmp(command, "process") == 0) {
            if (sm.state == STATE_PROCESSING) {
                if (!process_input(&sm)) {
                    change_state(&sm, STATE_ERROR);
                } else {
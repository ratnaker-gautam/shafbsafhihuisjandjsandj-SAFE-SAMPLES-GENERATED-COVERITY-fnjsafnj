//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
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
#define VALID_TRANSITION(from, to) \
    (from == STATE_IDLE && to == STATE_READING) || \
    (from == STATE_READING && to == STATE_PROCESSING) || \
    (from == STATE_READING && to == STATE_ERROR) || \
    (from == STATE_PROCESSING && to == STATE_DONE) || \
    (from == STATE_PROCESSING && to == STATE_ERROR) || \
    (from == STATE_DONE && to == STATE_IDLE) || \
    (from == STATE_ERROR && to == STATE_IDLE)

#define CHECK_BOUNDS(val, max) ((val) >= 0 && (val) < (max))

typedef struct {
    int state;
    char input[MAX_INPUT_LEN];
    char tokens[MAX_TOKENS][MAX_INPUT_LEN];
    int token_count;
} StateMachine;

void initialize_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    memset(sm->input, 0, sizeof(sm->input));
    for (int i = 0; i < MAX_TOKENS; i++) {
        memset(sm->tokens[i], 0, sizeof(sm->tokens[i]));
    }
    sm->token_count = 0;
}

int change_state(StateMachine *sm, int new_state) {
    if (sm == NULL) return 0;
    if (!VALID_TRANSITION(sm->state, new_state)) return 0;
    sm->state = new_state;
    return 1;
}

int read_input(StateMachine *sm) {
    if (sm == NULL || sm->state != STATE_READING) return 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    if (fgets(sm->input, sizeof(sm->input), stdin) == NULL) {
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

int tokenize_input(StateMachine *sm) {
    if (sm == NULL || sm->state != STATE_PROCESSING) return 0;
    
    sm->token_count = 0;
    char *token = strtok(sm->input, " ");
    
    while (token != NULL && sm->token_count < MAX_TOKENS) {
        if (strlen(token) >= MAX_INPUT_LEN) {
            return 0;
        }
        strncpy(sm->tokens[sm->token_count], token, MAX_INPUT_LEN - 1);
        sm->tokens[sm->token_count][MAX_INPUT_LEN - 1] = '\0';
        sm->token_count++;
        token = strtok(NULL, " ");
    }
    
    return sm->token_count > 0;
}

void process_tokens(StateMachine *sm) {
    if (sm == NULL || sm->state != STATE_PROCESSING) return;
    
    printf("Processing %d tokens:\n", sm->token_count);
    for (int i = 0; i < sm->token_count; i++) {
        if (CHECK_BOUNDS(i, MAX_TOKENS)) {
            printf("  Token %d: '%s'\n", i + 1, sm->tokens[i]);
        }
    }
}

int main(void) {
    StateMachine sm;
    initialize_machine(&sm);
    char buffer[10];
    
    printf("State Machine Demo - Text Tokenizer\n");
    
    while (1) {
        printf("\nCurrent state: ");
        switch (sm.state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_DONE: printf("DONE"); break;
            case STATE_ERROR: printf("ERROR"); break;
            default: printf("UNKNOWN"); break;
        }
        printf("\n");
        
        switch (sm.state) {
            case STATE_IDLE:
                printf("Options: (r)ead, (q)uit: ");
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
                
                if (buffer[0] == 'r' || buffer[0] == 'R') {
                    if (!change_state(&sm, STATE_READING)) {
                        change_state(&sm, STATE_ERROR);
                    }
                } else if (buffer[0] == 'q' || buffer[0] == 'Q') {
                    return 0;
                }
                break;
                
            case STATE_READING:
                if (!read_input(&sm)) {
                    change_state(&sm, STATE_ERROR);
                } else {
                    change_state(&sm, STATE_PROCESSING);
                }
                break;
                
            case STATE
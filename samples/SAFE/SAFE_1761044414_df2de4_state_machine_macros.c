//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_WORDS 50
#define MAX_WORD_LEN 20

typedef struct {
    int state;
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int processed_count;
} StateMachine;

void state_machine_init(StateMachine *sm) {
    sm->state = STATE_IDLE;
    memset(sm->input, 0, sizeof(sm->input));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(sm->words[i], 0, sizeof(sm->words[i]));
    }
    sm->word_count = 0;
    sm->processed_count = 0;
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

void state_idle(StateMachine *sm) {
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    if (fgets(sm->input, sizeof(sm->input), stdin) == NULL) {
        sm->state = STATE_ERROR;
        return;
    }
    size_t len = strlen(sm->input);
    if (len > 0 && sm->input[len - 1] == '\n') {
        sm->input[len - 1] = '\0';
    }
    if (!validate_input(sm->input)) {
        sm->state = STATE_ERROR;
        return;
    }
    sm->state = STATE_READING;
}

void state_reading(StateMachine *sm) {
    char *token = strtok(sm->input, " \t");
    sm->word_count = 0;
    while (token != NULL && sm->word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len >= MAX_WORD_LEN) {
            sm->state = STATE_ERROR;
            return;
        }
        strncpy(sm->words[sm->word_count], token, MAX_WORD_LEN - 1);
        sm->words[sm->word_count][MAX_WORD_LEN - 1] = '\0';
        sm->word_count++;
        token = strtok(NULL, " \t");
    }
    if (sm->word_count == 0) {
        sm->state = STATE_ERROR;
        return;
    }
    sm->state = STATE_PROCESSING;
}

void state_processing(StateMachine *sm) {
    for (int i = 0; i < sm->word_count; i++) {
        for (int j = 0; sm->words[i][j] != '\0'; j++) {
            if (sm->words[i][j] >= 'a' && sm->words[i][j] <= 'z') {
                sm->words[i][j] = sm->words[i][j] - 'a' + 'A';
            }
        }
        sm->processed_count++;
    }
    sm->state = STATE_WRITING;
}

void state_writing(StateMachine *sm) {
    printf("Processed words (%d):\n", sm->processed_count);
    for (int i = 0; i < sm->word_count; i++) {
        printf("%s\n", sm->words[i]);
    }
    sm->state = STATE_IDLE;
}

void state_error(StateMachine *sm) {
    printf("Error: Invalid input or processing error\n");
    state_machine_init(sm);
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    printf("Text Processor State Machine\n");
    printf("Type 'quit' to exit\n");
    
    while (1) {
        switch (sm.state) {
            case STATE_IDLE:
                state_idle(&sm);
                break;
            case STATE_READING:
                state_reading(&sm);
                break;
            case STATE_PROCESSING:
                state_processing(&sm);
                break;
            case STATE_WRITING:
                state_writing(&sm);
                break;
            case STATE_ERROR:
                state_error(&sm);
                break;
            default:
                sm.state = STATE_ERROR;
                break;
        }
        
        if (sm.state == STATE_IDLE && sm.input[0] != '\0') {
            if (strcmp(sm.input, "quit") == 0) {
                break;
            }
        }
    }
    
    printf("Exiting text processor\n");
    return 0;
}
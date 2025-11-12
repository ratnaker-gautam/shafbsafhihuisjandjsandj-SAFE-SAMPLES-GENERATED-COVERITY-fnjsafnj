//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
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
    int current_pos;
} StateMachine;

void state_machine_init(StateMachine *sm) {
    sm->state = STATE_IDLE;
    memset(sm->input, 0, sizeof(sm->input));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(sm->words[i], 0, sizeof(sm->words[i]));
    }
    sm->word_count = 0;
    sm->current_pos = 0;
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
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN - 1);
    if (fgets(sm->input, sizeof(sm->input), stdin) != NULL) {
        size_t len = strlen(sm->input);
        if (len > 0 && sm->input[len-1] == '\n') {
            sm->input[len-1] = '\0';
        }
        if (validate_input(sm->input)) {
            sm->state = STATE_READING;
            sm->current_pos = 0;
            sm->word_count = 0;
        } else {
            sm->state = STATE_ERROR;
        }
    } else {
        sm->state = STATE_ERROR;
    }
}

void state_reading(StateMachine *sm) {
    int input_len = (int)strlen(sm->input);
    while (sm->current_pos < input_len && sm->word_count < MAX_WORDS) {
        while (sm->input[sm->current_pos] == ' ' && sm->current_pos < input_len) {
            sm->current_pos++;
        }
        if (sm->current_pos >= input_len) break;
        
        int word_start = sm->current_pos;
        while (sm->input[sm->current_pos] != ' ' && sm->current_pos < input_len) {
            sm->current_pos++;
        }
        int word_len = sm->current_pos - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            if (word_len < MAX_WORD_LEN) {
                strncpy(sm->words[sm->word_count], &sm->input[word_start], word_len);
                sm->words[sm->word_count][word_len] = '\0';
                sm->word_count++;
            }
        }
    }
    sm->state = STATE_PROCESSING;
}

void state_processing(StateMachine *sm) {
    for (int i = 0; i < sm->word_count; i++) {
        for (int j = 0; j < (int)strlen(sm->words[i]); j++) {
            if (sm->words[i][j] >= 'a' && sm->words[i][j] <= 'z') {
                sm->words[i][j] = sm->words[i][j] - 'a' + 'A';
            }
        }
    }
    sm->state = STATE_WRITING;
}

void state_writing(StateMachine *sm) {
    printf("Processed words (%d):\n", sm->word_count);
    for (int i = 0; i < sm->word_count; i++) {
        printf("%d: %s\n", i+1, sm->words[i]);
    }
    sm->state = STATE_IDLE;
}

void state_error(StateMachine *sm) {
    printf("Error: Invalid input detected\n");
    sm->state = STATE_IDLE;
}

void run_state_machine(StateMachine *sm) {
    switch (sm->state) {
        case STATE_IDLE:
            state_idle(sm);
            break;
        case STATE_READING:
            state_reading(sm);
            break;
        case STATE_PROCESSING:
            state_processing(sm);
            break;
        case STATE_WRITING:
            state_writing(sm);
            break;
        case STATE_ERROR:
            state_error(sm);
            break;
        default:
            sm->state = STATE_ERROR;
            break;
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    printf("Text Processor State Machine\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        run_state_machine(&sm);
        
        if (sm.state == STATE_IDLE && sm.input[0] != '\0') {
            if (strcmp(sm.input, "quit") == 0) {
                break;
            }
        }
    }
    
    printf("Goodbye!\n");
    return
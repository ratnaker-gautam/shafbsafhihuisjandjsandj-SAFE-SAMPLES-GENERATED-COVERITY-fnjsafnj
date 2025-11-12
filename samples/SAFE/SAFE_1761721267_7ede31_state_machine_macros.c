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

int parse_words(StateMachine *sm) {
    if (sm->state != STATE_READING) return 0;
    
    char buffer[MAX_INPUT_LEN];
    strncpy(buffer, sm->input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n");
    sm->word_count = 0;
    
    while (token != NULL && sm->word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            strncpy(sm->words[sm->word_count], token, MAX_WORD_LEN - 1);
            sm->words[sm->word_count][MAX_WORD_LEN - 1] = '\0';
            sm->word_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    return sm->word_count > 0;
}

void process_words(StateMachine *sm) {
    if (sm->state != STATE_PROCESSING) return;
    
    for (int i = 0; i < sm->word_count; i++) {
        for (int j = 0; sm->words[i][j] != '\0'; j++) {
            if (sm->words[i][j] >= 'a' && sm->words[i][j] <= 'z') {
                sm->words[i][j] = sm->words[i][j] - 'a' + 'A';
            }
        }
    }
    sm->processed_count = sm->word_count;
}

void run_state_machine(StateMachine *sm) {
    switch (sm->state) {
        case STATE_IDLE:
            printf("Enter text to process: ");
            if (fgets(sm->input, sizeof(sm->input), stdin) != NULL) {
                size_t len = strlen(sm->input);
                if (len > 0 && sm->input[len - 1] == '\n') {
                    sm->input[len - 1] = '\0';
                }
                if (validate_input(sm->input)) {
                    sm->state = STATE_READING;
                } else {
                    sm->state = STATE_ERROR;
                }
            } else {
                sm->state = STATE_ERROR;
            }
            break;
            
        case STATE_READING:
            if (parse_words(sm)) {
                sm->state = STATE_PROCESSING;
            } else {
                sm->state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            process_words(sm);
            sm->state = STATE_WRITING;
            break;
            
        case STATE_WRITING:
            printf("Processed words (%d):\n", sm->processed_count);
            for (int i = 0; i < sm->processed_count; i++) {
                printf("%s\n", sm->words[i]);
            }
            sm->state = STATE_IDLE;
            break;
            
        case STATE_ERROR:
            printf("Error occurred during processing\n");
            sm->state = STATE_IDLE;
            break;
            
        default:
            sm->state = STATE_ERROR;
            break;
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    int cycles = 0;
    while (cycles < 5) {
        run_state_machine(&sm);
        cycles++;
    }
    
    return 0;
}
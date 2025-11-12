//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define EVENT_START 0
#define EVENT_DATA 1
#define EVENT_END 2
#define EVENT_RESET 3
#define EVENT_INVALID 4

#define MAX_INPUT_LEN 100
#define MAX_WORDS 10
#define MAX_WORD_LEN 20

typedef struct {
    int state;
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int pos;
} state_machine_t;

void init_machine(state_machine_t *sm) {
    sm->state = STATE_IDLE;
    sm->input[0] = '\0';
    sm->word_count = 0;
    sm->pos = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        sm->words[i][0] = '\0';
    }
}

int get_next_event(const char *input, int pos) {
    if (pos < 0 || input == NULL) return EVENT_INVALID;
    
    if (pos == 0 && input[0] != '\0') return EVENT_START;
    if (input[pos] == '\0') return EVENT_END;
    if (input[pos] == ';') return EVENT_RESET;
    if (isalnum((unsigned char)input[pos]) || input[pos] == ' ') return EVENT_DATA;
    
    return EVENT_INVALID;
}

int process_event(state_machine_t *sm, int event) {
    switch (sm->state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->state = STATE_READING;
                return 1;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->word_count < MAX_WORDS) {
                    int word_len = 0;
                    char current_word[MAX_WORD_LEN] = {0};
                    
                    while (sm->input[sm->pos] != '\0' && 
                           sm->input[sm->pos] != ' ' && 
                           word_len < MAX_WORD_LEN - 1) {
                        if (isalnum((unsigned char)sm->input[sm->pos])) {
                            current_word[word_len++] = sm->input[sm->pos];
                        }
                        sm->pos++;
                    }
                    
                    if (word_len > 0) {
                        current_word[word_len] = '\0';
                        strncpy(sm->words[sm->word_count], current_word, MAX_WORD_LEN - 1);
                        sm->words[sm->word_count][MAX_WORD_LEN - 1] = '\0';
                        sm->word_count++;
                    }
                    
                    while (sm->input[sm->pos] == ' ') {
                        sm->pos++;
                    }
                }
                return 1;
            } else if (event == EVENT_END) {
                sm->state = STATE_PROCESSING;
                return 1;
            } else if (event == EVENT_RESET) {
                init_machine(sm);
                return 1;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                init_machine(sm);
                return 1;
            } else if (event == EVENT_END) {
                sm->state = STATE_DONE;
                return 1;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                init_machine(sm);
                return 1;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                init_machine(sm);
                return 1;
            }
            break;
    }
    
    if (event == EVENT_INVALID) {
        sm->state = STATE_ERROR;
    }
    
    return 0;
}

void print_results(const state_machine_t *sm) {
    if (sm->state == STATE_DONE) {
        printf("Processing complete. Found %d words:\n", sm->word_count);
        for (int i = 0; i < sm->word_count; i++) {
            printf("%d: '%s'\n", i + 1, sm->words[i]);
        }
    } else if (sm->state == STATE_ERROR) {
        printf("Error: Invalid input detected\n");
    } else {
        printf("State: %d - Processing not complete\n", sm->state);
    }
}

int main(void) {
    state_machine_t sm;
    char user_input[MAX_INPUT_LEN + 2];
    
    printf("Enter text to process (max %d chars, use ; to reset): ", MAX_INPUT_LEN);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n') {
        user_input[len - 1] = '\0';
    }
    
    if (strlen(user_input) > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    init_machine(&sm);
    strncpy(sm.input, user_input
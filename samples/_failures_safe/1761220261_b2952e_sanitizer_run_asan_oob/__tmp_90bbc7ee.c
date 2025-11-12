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

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ' || c == '\n' || c == '\0')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int current_word_len;
} WordData;

void initialize_word_data(WordData *data) {
    if (data == NULL) return;
    data->word_count = 0;
    data->current_word_len = 0;
    memset(data->words, 0, sizeof(data->words));
}

int process_input(const char *input, WordData *data) {
    if (input == NULL || data == NULL) {
        return STATE_ERROR;
    }
    
    int current_state = STATE_IDLE;
    int i = 0;
    char c;
    
    initialize_word_data(data);
    
    while (current_state != STATE_ERROR && current_state != STATE_WRITING) {
        c = input[i];
        
        if (!IS_VALID_CHAR(c)) {
            TRANSITION(STATE_ERROR);
            break;
        }
        
        switch (current_state) {
            case STATE_IDLE:
                if (c == ' ' || c == '\n' || c == '\0') {
                    i++;
                } else {
                    TRANSITION(STATE_READING);
                }
                break;
                
            case STATE_READING:
                if (data->word_count >= MAX_WORDS) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (c == ' ' || c == '\n' || c == '\0') {
                    if (data->current_word_len > 0) {
                        if (data->word_count < MAX_WORDS) {
                            data->words[data->word_count][data->current_word_len] = '\0';
                            data->word_count++;
                        }
                        data->current_word_len = 0;
                    }
                    TRANSITION(c == '\0' ? STATE_WRITING : STATE_IDLE);
                    i++;
                } else {
                    if (data->current_word_len >= MAX_WORD_LEN - 1) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    if (data->word_count < MAX_WORDS && data->current_word_len < MAX_WORD_LEN - 1) {
                        data->words[data->word_count][data->current_word_len] = c;
                    }
                    data->current_word_len++;
                    i++;
                }
                break;
                
            case STATE_PROCESSING:
                TRANSITION(STATE_WRITING);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return current_state;
}

void print_results(int state, const WordData *data) {
    if (data == NULL) {
        printf("Error: Invalid data\n");
        return;
    }
    
    switch (state) {
        case STATE_WRITING:
            printf("Successfully processed %d words:\n", data->word_count);
            for (int i = 0; i < data->word_count && i < MAX_WORDS; i++) {
                printf("  %d: '%s'\n", i + 1, data->words[i]);
            }
            break;
        case STATE_ERROR:
            printf("Error: Invalid input or buffer overflow\n");
            break;
        default:
            printf("Unexpected state: %d\n", state);
            break;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordData word_data;
    int result;
    
    printf("Enter text (letters and spaces only, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= MAX_INPUT_LEN) {
        printf("Error: Input too long\n");
        return 1;
    }
    
    result = process_input(input, &word_data);
    print_results(result, &word_data);
    
    return result == STATE_WRITING ? 0 : 1;
}
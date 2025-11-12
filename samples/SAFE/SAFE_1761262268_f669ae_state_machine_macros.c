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
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
#define TO_UPPER(c) ((c >= 'a' && c <= 'z') ? (c - 32) : c)

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int processed_count;
} WordData;

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

void initialize_data(WordData* data) {
    data->word_count = 0;
    data->processed_count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(data->words[i], 0, MAX_WORD_LEN);
    }
}

int process_word(WordData* data, const char* word, int word_len) {
    if (data->word_count >= MAX_WORDS || word_len >= MAX_WORD_LEN) {
        return 0;
    }
    
    for (int i = 0; i < word_len; i++) {
        data->words[data->word_count][i] = TO_UPPER(word[i]);
    }
    data->word_count++;
    return 1;
}

void state_machine(const char* input) {
    if (!validate_input(input)) {
        printf("Invalid input\n");
        return;
    }
    
    WordData data;
    initialize_data(&data);
    
    int current_state = STATE_IDLE;
    int input_pos = 0;
    char current_word[MAX_WORD_LEN] = {0};
    int word_pos = 0;
    
    while (current_state != STATE_ERROR && input[input_pos] != '\0') {
        char c = input[input_pos];
        
        switch (current_state) {
            case STATE_IDLE:
                if (IS_VALID_CHAR(c)) {
                    if (word_pos < MAX_WORD_LEN - 1) {
                        current_word[word_pos++] = c;
                    }
                    TRANSITION(STATE_READING);
                } else if (c != ' ') {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                if (IS_VALID_CHAR(c)) {
                    if (word_pos < MAX_WORD_LEN - 1) {
                        current_word[word_pos++] = c;
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else if (c == ' ') {
                    if (word_pos > 0) {
                        if (!process_word(&data, current_word, word_pos)) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        memset(current_word, 0, MAX_WORD_LEN);
                        word_pos = 0;
                    }
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (IS_VALID_CHAR(c)) {
                    if (word_pos < MAX_WORD_LEN - 1) {
                        current_word[word_pos++] = c;
                    }
                    TRANSITION(STATE_READING);
                } else if (c != ' ') {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
        
        input_pos++;
    }
    
    if (current_state == STATE_READING && word_pos > 0) {
        if (!process_word(&data, current_word, word_pos)) {
            TRANSITION(STATE_ERROR);
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error processing input\n");
        return;
    }
    
    printf("Processed words (%d):\n", data.word_count);
    for (int i = 0; i < data.word_count; i++) {
        if (strlen(data.words[i]) > 0) {
            printf("%s\n", data.words[i]);
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1] = {0};
    
    printf("Enter text to process (letters and spaces only): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    state_machine(input);
    
    return 0;
}
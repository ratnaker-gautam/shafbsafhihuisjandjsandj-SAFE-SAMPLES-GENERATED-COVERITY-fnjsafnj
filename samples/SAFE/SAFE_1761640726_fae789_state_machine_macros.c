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

void initialize_word_data(WordData* data) {
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

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordData word_data;
    int current_state = STATE_IDLE;
    int input_valid = 0;
    
    initialize_word_data(&word_data);
    
    printf("Enter text to process (letters only): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        input_valid = validate_input(input);
        if (!input_valid) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_READING);
        }
    }
    
    while (current_state != STATE_ERROR && current_state != STATE_WRITING) {
        switch (current_state) {
            case STATE_READING: {
                char current_word[MAX_WORD_LEN];
                int word_len = 0;
                int i = 0;
                
                while (input[i] != '\0' && word_data.word_count < MAX_WORDS) {
                    if (IS_VALID_CHAR(input[i])) {
                        if (word_len < MAX_WORD_LEN - 1) {
                            current_word[word_len++] = input[i];
                        }
                    } else if (word_len > 0) {
                        current_word[word_len] = '\0';
                        if (!process_word(&word_data, current_word, word_len)) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        word_len = 0;
                    }
                    i++;
                }
                
                if (word_len > 0 && current_state != STATE_ERROR) {
                    current_word[word_len] = '\0';
                    if (!process_word(&word_data, current_word, word_len)) {
                        TRANSITION(STATE_ERROR);
                    }
                }
                
                if (current_state != STATE_ERROR) {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
            }
            
            case STATE_PROCESSING: {
                if (word_data.word_count == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                word_data.processed_count = word_data.word_count;
                for (int i = 0; i < word_data.word_count - 1; i++) {
                    for (int j = i + 1; j < word_data.word_count; j++) {
                        if (strcmp(word_data.words[i], word_data.words[j]) > 0) {
                            char temp[MAX_WORD_LEN];
                            strncpy(temp, word_data.words[i], MAX_WORD_LEN);
                            strncpy(word_data.words[i], word_data.words[j], MAX_WORD_LEN);
                            strncpy(word_data.words[j], temp, MAX_WORD_LEN);
                        }
                    }
                }
                TRANSITION(STATE_WRITING);
                break;
            }
            
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing error\n");
        return 1;
    }
    
    printf("Processed words (%d):\n", word_data.processed_count);
    for (int i = 0; i < word_data.processed_count; i++) {
        printf("%s\n", word_data.words[i]);
    }
    
    return 0;
}
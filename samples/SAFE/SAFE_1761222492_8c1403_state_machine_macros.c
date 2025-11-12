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
#define TO_UPPER(c) ((c >= 'a' && c <= 'z') ? (c - 'a' + 'A') : c)

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int char_count;
} text_data_t;

static int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

static void initialize_data(text_data_t* data) {
    data->word_count = 0;
    data->char_count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(data->words[i], 0, MAX_WORD_LEN);
    }
}

static int process_character(char c, text_data_t* data, int* current_char_pos) {
    if (!IS_VALID_CHAR(c)) return 0;
    
    if (data->word_count >= MAX_WORDS) return 0;
    if (*current_char_pos >= MAX_WORD_LEN - 1) return 0;
    
    data->words[data->word_count][*current_char_pos] = TO_UPPER(c);
    (*current_char_pos)++;
    data->char_count++;
    return 1;
}

static void state_machine(const char* input) {
    text_data_t data;
    initialize_data(&data);
    
    int current_state = STATE_IDLE;
    int current_char_pos = 0;
    size_t input_len = strlen(input);
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        
        switch (current_state) {
            case STATE_IDLE:
                if (IS_VALID_CHAR(c)) {
                    if (process_character(c, &data, &current_char_pos)) {
                        TRANSITION(STATE_READING);
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                }
                break;
                
            case STATE_READING:
                if (IS_VALID_CHAR(c)) {
                    if (!process_character(c, &data, &current_char_pos)) {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    if (current_char_pos > 0) {
                        data.word_count++;
                        current_char_pos = 0;
                    }
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (IS_VALID_CHAR(c)) {
                    if (process_character(c, &data, &current_char_pos)) {
                        TRANSITION(STATE_READING);
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                }
                break;
                
            case STATE_WRITING:
                break;
                
            case STATE_ERROR:
                printf("Error processing input at position %zu\n", i);
                return;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_char_pos > 0 && data.word_count < MAX_WORDS) {
        data.word_count++;
    }
    
    if (current_state != STATE_ERROR) {
        TRANSITION(STATE_WRITING);
        
        printf("Processed %d words, %d characters\n", data.word_count, data.char_count);
        printf("Words in uppercase:\n");
        for (int i = 0; i < data.word_count && i < MAX_WORDS; i++) {
            if (strlen(data.words[i]) > 0) {
                printf("%s\n", data.words[i]);
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    state_machine(input);
    
    return 0;
}
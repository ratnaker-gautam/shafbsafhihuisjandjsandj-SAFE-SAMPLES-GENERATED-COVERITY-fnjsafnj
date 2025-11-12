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

static void process_word(text_data_t* data, const char* word, int len) {
    if (data->word_count >= MAX_WORDS || len >= MAX_WORD_LEN) {
        return;
    }
    
    for (int i = 0; i < len; i++) {
        data->words[data->word_count][i] = TO_UPPER(word[i]);
    }
    data->words[data->word_count][len] = '\0';
    data->char_count += len;
    data->word_count++;
}

static void state_machine_process(const char* input, text_data_t* data) {
    int current_state = STATE_IDLE;
    int input_len = (int)strlen(input);
    int pos = 0;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    memset(data, 0, sizeof(text_data_t));
    
    while (pos < input_len && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                if (IS_VALID_CHAR(input[pos])) {
                    current_word[0] = input[pos];
                    word_len = 1;
                    TRANSITION(STATE_READING);
                }
                pos++;
                break;
                
            case STATE_READING:
                if (IS_VALID_CHAR(input[pos])) {
                    if (word_len < MAX_WORD_LEN - 1) {
                        current_word[word_len++] = input[pos];
                    }
                    pos++;
                } else {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                process_word(data, current_word, word_len);
                word_len = 0;
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_READING && word_len > 0) {
        process_word(data, current_word, word_len);
    }
}

static void print_results(const text_data_t* data) {
    printf("Processed %d words, %d characters\n", data->word_count, data->char_count);
    printf("Words in uppercase:\n");
    for (int i = 0; i < data->word_count; i++) {
        printf("%s\n", data->words[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    text_data_t data;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    state_machine_process(input, &data);
    
    if (data.word_count == 0) {
        printf("No valid words found in input\n");
        return EXIT_SUCCESS;
    }
    
    print_results(&data);
    
    return EXIT_SUCCESS;
}
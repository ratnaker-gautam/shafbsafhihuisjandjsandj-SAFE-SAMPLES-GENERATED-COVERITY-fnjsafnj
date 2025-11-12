//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
} parser_state_t;

typedef enum {
    TOKEN_DIGIT,
    TOKEN_LETTER,
    TOKEN_SPACE,
    TOKEN_NEWLINE,
    TOKEN_EOF,
    TOKEN_INVALID
} token_type_t;

typedef struct {
    int numbers_found;
    int words_found;
    int current_number;
    char current_word[MAX_INPUT_LEN];
    int word_pos;
} parser_data_t;

token_type_t get_token_type(char c) {
    if (c == '\0') return TOKEN_EOF;
    if (c == '\n') return TOKEN_NEWLINE;
    if (isspace(c)) return TOKEN_SPACE;
    if (isdigit(c)) return TOKEN_DIGIT;
    if (isalpha(c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

parser_state_t handle_start(token_type_t token, char c, parser_data_t *data) {
    if (token == TOKEN_DIGIT) {
        data->current_number = c - '0';
        return STATE_READING_NUMBER;
    } else if (token == TOKEN_LETTER) {
        data->current_word[0] = c;
        data->word_pos = 1;
        return STATE_READING_WORD;
    } else if (token == TOKEN_SPACE || token == TOKEN_NEWLINE) {
        return STATE_START;
    } else if (token == TOKEN_EOF) {
        return STATE_DONE;
    }
    return STATE_ERROR;
}

parser_state_t handle_reading_number(token_type_t token, char c, parser_data_t *data) {
    if (token == TOKEN_DIGIT) {
        if (data->current_number <= (INT_MAX - (c - '0')) / 10) {
            data->current_number = data->current_number * 10 + (c - '0');
            return STATE_READING_NUMBER;
        }
        return STATE_ERROR;
    } else if (token == TOKEN_SPACE || token == TOKEN_NEWLINE || token == TOKEN_EOF) {
        data->numbers_found++;
        printf("Found number: %d\n", data->current_number);
        if (token == TOKEN_EOF) return STATE_DONE;
        return STATE_START;
    }
    return STATE_ERROR;
}

parser_state_t handle_reading_word(token_type_t token, char c, parser_data_t *data) {
    if (token == TOKEN_LETTER) {
        if (data->word_pos < MAX_INPUT_LEN - 1) {
            data->current_word[data->word_pos++] = c;
            return STATE_READING_WORD;
        }
        return STATE_ERROR;
    } else if (token == TOKEN_SPACE || token == TOKEN_NEWLINE || token == TOKEN_EOF) {
        data->current_word[data->word_pos] = '\0';
        data->words_found++;
        printf("Found word: %s\n", data->current_word);
        if (token == TOKEN_EOF) return STATE_DONE;
        return STATE_START;
    }
    return STATE_ERROR;
}

void run_state_machine(const char *input) {
    parser_state_t state = STATE_START;
    parser_data_t data = {0};
    size_t pos = 0;
    char current_char;
    
    while (state != STATE_DONE && state != STATE_ERROR && pos < MAX_INPUT_LEN) {
        current_char = input[pos];
        token_type_t token = get_token_type(current_char);
        
        switch (state) {
            case STATE_START:
                state = handle_start(token, current_char, &data);
                break;
            case STATE_READING_NUMBER:
                state = handle_reading_number(token, current_char, &data);
                break;
            case STATE_READING_WORD:
                state = handle_reading_word(token, current_char, &data);
                break;
            case STATE_ERROR:
                printf("Error: Invalid input format\n");
                return;
            case STATE_DONE:
                return;
        }
        
        if (token != TOKEN_EOF) {
            pos++;
        } else {
            break;
        }
    }
    
    if (state == STATE_DONE) {
        printf("Parsing completed. Found %d numbers and %d words.\n", 
               data.numbers_found, data.words_found);
    } else if (state != STATE_ERROR) {
        printf("Error: Input too long or incomplete\n");
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("Error: Empty input\n");
        return 1;
    }
    
    run_state_machine(input);
    return 0
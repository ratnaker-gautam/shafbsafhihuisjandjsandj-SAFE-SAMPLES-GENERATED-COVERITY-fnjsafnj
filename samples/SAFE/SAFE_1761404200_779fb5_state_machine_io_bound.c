//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} parser_state_t;

typedef enum {
    TOKEN_DIGIT,
    TOKEN_LETTER,
    TOKEN_SPACE,
    TOKEN_EOF,
    TOKEN_INVALID
} token_type_t;

typedef struct {
    int numbers_found;
    int words_found;
    int errors_found;
} parser_stats_t;

static token_type_t get_next_token(char c) {
    if (c == '\0' || c == '\n') return TOKEN_EOF;
    if (isspace((unsigned char)c)) return TOKEN_SPACE;
    if (isdigit((unsigned char)c)) return TOKEN_DIGIT;
    if (isalpha((unsigned char)c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

static void process_state_machine(const char* input, parser_stats_t* stats) {
    parser_state_t current_state = STATE_START;
    size_t input_len = strlen(input);
    size_t pos = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR && pos <= input_len) {
        char current_char = (pos < input_len) ? input[pos] : '\0';
        token_type_t token = get_next_token(current_char);
        
        switch (current_state) {
            case STATE_START:
                if (token == TOKEN_DIGIT) {
                    current_state = STATE_READING_NUMBER;
                    stats->numbers_found++;
                } else if (token == TOKEN_LETTER) {
                    current_state = STATE_READING_WORD;
                    stats->words_found++;
                } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                    current_state = STATE_START;
                } else if (token == TOKEN_INVALID) {
                    current_state = STATE_ERROR;
                    stats->errors_found++;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (token == TOKEN_DIGIT) {
                    current_state = STATE_READING_NUMBER;
                } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                    stats->errors_found++;
                }
                break;
                
            case STATE_READING_WORD:
                if (token == TOKEN_LETTER) {
                    current_state = STATE_READING_WORD;
                } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                    stats->errors_found++;
                }
                break;
                
            case STATE_ERROR:
                if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                    current_state = STATE_START;
                }
                break;
                
            case STATE_END:
                break;
        }
        
        pos++;
    }
}

static int read_input_line(char* buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) return -1;
    
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 0;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    parser_stats_t stats = {0, 0, 0};
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (read_input_line(input_buffer, sizeof(input_buffer)) != 0) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    if (strlen(input_buffer) >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    process_state_machine(input_buffer, &stats);
    
    printf("Analysis results:\n");
    printf("Numbers found: %d\n", stats.numbers_found);
    printf("Words found: %d\n", stats.words_found);
    printf("Errors found: %d\n", stats.errors_found);
    
    return EXIT_SUCCESS;
}
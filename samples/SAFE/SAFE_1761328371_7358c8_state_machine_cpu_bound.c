//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} parser_state_t;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} parser_event_t;

static parser_event_t get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

static void process_state_machine(const char *input) {
    parser_state_t current_state = STATE_START;
    size_t input_len = strlen(input);
    size_t pos = 0;
    int number_count = 0;
    int word_count = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR && pos <= input_len) {
        char current_char = (pos < input_len) ? input[pos] : '\0';
        parser_event_t event = get_event(current_char);
        
        switch (current_state) {
            case STATE_START:
                switch (event) {
                    case EVENT_DIGIT:
                        current_state = STATE_READING_NUMBER;
                        number_count++;
                        break;
                    case EVENT_LETTER:
                        current_state = STATE_READING_WORD;
                        word_count++;
                        break;
                    case EVENT_SPACE:
                        break;
                    case EVENT_EOF:
                        current_state = STATE_END;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_NUMBER:
                switch (event) {
                    case EVENT_DIGIT:
                        break;
                    case EVENT_SPACE:
                        current_state = STATE_START;
                        break;
                    case EVENT_EOF:
                        current_state = STATE_END;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_WORD:
                switch (event) {
                    case EVENT_LETTER:
                        break;
                    case EVENT_SPACE:
                        current_state = STATE_START;
                        break;
                    case EVENT_EOF:
                        current_state = STATE_END;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input at position %zu\n", pos);
                return;
                
            case STATE_END:
                break;
        }
        
        pos++;
    }
    
    if (current_state == STATE_END) {
        printf("Parsing completed successfully.\n");
        printf("Numbers found: %d\n", number_count);
        printf("Words found: %d\n", word_count);
    } else if (current_state == STATE_ERROR) {
        printf("Error: Invalid input at position %zu\n", pos);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text to parse (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input)) {
        printf("Error: Input too long\n");
        return EXIT_FAILURE;
    }
    
    process_state_machine(input);
    
    return EXIT_SUCCESS;
}
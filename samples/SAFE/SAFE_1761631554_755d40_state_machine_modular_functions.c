//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
} State;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} Event;

typedef struct {
    int number_count;
    int word_count;
    int error_count;
} Statistics;

State get_next_state(State current, Event event);
Event classify_char(char c);
void process_input(Statistics *stats);
void print_statistics(const Statistics *stats);

int main(void) {
    Statistics stats = {0, 0, 0};
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    process_input(&stats);
    print_statistics(&stats);
    
    return 0;
}

State get_next_state(State current, Event event) {
    switch(current) {
        case STATE_START:
            switch(event) {
                case EVENT_DIGIT: return STATE_READING_NUMBER;
                case EVENT_LETTER: return STATE_READING_WORD;
                case EVENT_SPACE: return STATE_START;
                case EVENT_EOF: return STATE_END;
                default: return STATE_ERROR;
            }
            
        case STATE_READING_NUMBER:
            switch(event) {
                case EVENT_DIGIT: return STATE_READING_NUMBER;
                case EVENT_SPACE: return STATE_START;
                case EVENT_EOF: return STATE_END;
                default: return STATE_ERROR;
            }
            
        case STATE_READING_WORD:
            switch(event) {
                case EVENT_LETTER: return STATE_READING_WORD;
                case EVENT_SPACE: return STATE_START;
                case EVENT_EOF: return STATE_END;
                default: return STATE_ERROR;
            }
            
        case STATE_ERROR:
            switch(event) {
                case EVENT_SPACE: return STATE_START;
                case EVENT_EOF: return STATE_END;
                default: return STATE_ERROR;
            }
            
        default:
            return STATE_ERROR;
    }
}

Event classify_char(char c) {
    if (c == EOF || c == '\0') return EVENT_EOF;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

void process_input(Statistics *stats) {
    char input[MAX_INPUT_LEN + 1];
    State current_state = STATE_START;
    State previous_state = STATE_START;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        stats->error_count++;
        return;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char current_char = input[i];
        Event event = classify_char(current_char);
        previous_state = current_state;
        current_state = get_next_state(current_state, event);
        
        if (previous_state == STATE_READING_NUMBER && 
            (current_state == STATE_START || current_state == STATE_END)) {
            stats->number_count++;
        }
        
        if (previous_state == STATE_READING_WORD && 
            (current_state == STATE_START || current_state == STATE_END)) {
            stats->word_count++;
        }
        
        if (current_state == STATE_ERROR) {
            stats->error_count++;
        }
        
        if (current_state == STATE_END) {
            break;
        }
    }
}

void print_statistics(const Statistics *stats) {
    printf("Numbers found: %d\n", stats->number_count);
    printf("Words found: %d\n", stats->word_count);
    printf("Errors detected: %d\n", stats->error_count);
}
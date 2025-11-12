//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    EVENT_OTHER
} Event;

typedef struct {
    int number_count;
    int word_count;
    int total_chars;
} Statistics;

State get_next_state(State current, Event event);
Event classify_event(int ch);
void process_character(int ch, State *current_state, Statistics *stats);
void print_statistics(const Statistics *stats);

int main(void) {
    State current_state = STATE_START;
    Statistics stats = {0, 0, 0};
    int ch;
    
    printf("Enter text (Ctrl+D to end):\n");
    
    while ((ch = getchar()) != EOF) {
        if (stats.total_chars >= MAX_INPUT_LEN) {
            current_state = STATE_ERROR;
            break;
        }
        
        process_character(ch, &current_state, &stats);
        stats.total_chars++;
        
        if (current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (current_state != STATE_ERROR) {
        process_character(EOF, &current_state, &stats);
    }
    
    print_statistics(&stats);
    
    return current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}

State get_next_state(State current, Event event) {
    static const State transition_table[MAX_STATES][5] = {
        {STATE_READING_NUMBER, STATE_READING_WORD, STATE_START, STATE_END, STATE_ERROR},
        {STATE_READING_NUMBER, STATE_ERROR, STATE_START, STATE_END, STATE_ERROR},
        {STATE_ERROR, STATE_READING_WORD, STATE_START, STATE_END, STATE_ERROR},
        {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
        {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
    };
    
    if (current >= MAX_STATES || event >= 5) {
        return STATE_ERROR;
    }
    
    return transition_table[current][event];
}

Event classify_event(int ch) {
    if (ch == EOF) {
        return EVENT_EOF;
    } else if (isdigit(ch)) {
        return EVENT_DIGIT;
    } else if (isalpha(ch)) {
        return EVENT_LETTER;
    } else if (isspace(ch)) {
        return EVENT_SPACE;
    } else {
        return EVENT_OTHER;
    }
}

void process_character(int ch, State *current_state, Statistics *stats) {
    static int in_number = 0;
    static int in_word = 0;
    
    Event event = classify_event(ch);
    State next_state = get_next_state(*current_state, event);
    
    switch (*current_state) {
        case STATE_START:
            if (next_state == STATE_READING_NUMBER) {
                in_number = 1;
            } else if (next_state == STATE_READING_WORD) {
                in_word = 1;
            }
            break;
            
        case STATE_READING_NUMBER:
            if (next_state == STATE_START) {
                if (in_number) {
                    stats->number_count++;
                    in_number = 0;
                }
            }
            break;
            
        case STATE_READING_WORD:
            if (next_state == STATE_START) {
                if (in_word) {
                    stats->word_count++;
                    in_word = 0;
                }
            }
            break;
            
        case STATE_ERROR:
            break;
            
        case STATE_END:
            break;
    }
    
    if (next_state == STATE_READING_NUMBER && *current_state != STATE_READING_NUMBER) {
        in_number = 1;
    }
    
    if (next_state == STATE_READING_WORD && *current_state != STATE_READING_WORD) {
        in_word = 1;
    }
    
    if (next_state == STATE_END) {
        if (in_number) {
            stats->number_count++;
        }
        if (in_word) {
            stats->word_count++;
        }
    }
    
    *current_state = next_state;
}

void print_statistics(const Statistics *stats) {
    printf("\nText Analysis Results:\n");
    printf("Numbers found: %d\n", stats->number_count);
    printf("Words found: %d\n", stats->word_count);
    printf("Total characters processed: %d\n", stats->total_chars);
}
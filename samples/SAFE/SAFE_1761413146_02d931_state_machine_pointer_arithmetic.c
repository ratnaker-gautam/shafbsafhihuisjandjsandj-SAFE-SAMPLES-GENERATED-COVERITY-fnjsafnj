//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define MAX_STATES 8
#define MAX_TRANSITIONS 16

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_NUMBER,
    STATE_WORD,
    STATE_SYMBOL,
    STATE_END,
    STATE_ERROR,
    STATE_COUNT
} State;

typedef struct {
    State current;
    char trigger;
    State next;
} Transition;

Transition transitions[MAX_TRANSITIONS] = {
    {STATE_START, ' ', STATE_START},
    {STATE_START, '\t', STATE_START},
    {STATE_START, '\n', STATE_START},
    {STATE_START, '\0', STATE_END},
    {STATE_START, 0, STATE_READING},
    {STATE_READING, ' ', STATE_START},
    {STATE_READING, '\t', STATE_START},
    {STATE_READING, '\n', STATE_START},
    {STATE_READING, '\0', STATE_END},
    {STATE_READING, 0, STATE_READING},
    {STATE_NUMBER, ' ', STATE_START},
    {STATE_NUMBER, '\t', STATE_START},
    {STATE_NUMBER, '\n', STATE_START},
    {STATE_NUMBER, '\0', STATE_END},
    {STATE_NUMBER, 0, STATE_NUMBER},
    {STATE_WORD, 0, STATE_ERROR}
};

State get_next_state(State current, char input) {
    Transition *t = transitions;
    while (t < transitions + MAX_TRANSITIONS) {
        if (t->current == current) {
            if (t->trigger == 0 && isalnum(input)) {
                return t->next;
            }
            if (t->trigger == input) {
                return t->next;
            }
        }
        t++;
    }
    return STATE_ERROR;
}

void process_input(const char *input) {
    State current_state = STATE_START;
    const char *ptr = input;
    int word_count = 0;
    int number_count = 0;
    int symbol_count = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        char c = *ptr;
        State next_state = get_next_state(current_state, c);
        
        if (current_state == STATE_READING) {
            if (isdigit(c)) {
                next_state = STATE_NUMBER;
            } else if (isalpha(c)) {
                next_state = STATE_WORD;
            } else {
                next_state = STATE_SYMBOL;
            }
        }
        
        if (current_state != next_state) {
            if (current_state == STATE_WORD) {
                word_count++;
            } else if (current_state == STATE_NUMBER) {
                number_count++;
            } else if (current_state == STATE_SYMBOL) {
                symbol_count++;
            }
        }
        
        current_state = next_state;
        ptr++;
        
        if (ptr - input >= MAX_INPUT) {
            break;
        }
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    printf("Symbols: %d\n", symbol_count);
}

int main() {
    char input[MAX_INPUT + 1] = {0};
    
    printf("Enter text (max %d characters): ", MAX_INPUT);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}
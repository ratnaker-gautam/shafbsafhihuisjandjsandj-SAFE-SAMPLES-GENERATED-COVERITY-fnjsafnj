//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINAL,
    STATE_ERROR
} State;

typedef enum {
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} Event;

State handle_start(Event ev, char ch);
State handle_reading(Event ev, char ch);
State handle_processing(Event ev, char ch);
State handle_final(Event ev, char ch);
State handle_error(Event ev, char ch);

int main(void) {
    State current_state = STATE_START;
    char input[MAX_INPUT_LEN + 1];
    int input_len = 0;
    int word_count = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (int i = 0; i <= input_len; i++) {
        char ch = input[i];
        Event ev;
        
        if (ch == '\0') {
            ev = EVENT_EOF;
        } else if (isspace((unsigned char)ch)) {
            ev = EVENT_SPACE;
        } else if (isprint((unsigned char)ch)) {
            ev = EVENT_CHAR;
        } else {
            ev = EVENT_INVALID;
        }
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(ev, ch);
                break;
            case STATE_READING:
                current_state = handle_reading(ev, ch);
                break;
            case STATE_PROCESSING:
                current_state = handle_processing(ev, ch);
                if (current_state == STATE_READING) {
                    word_count++;
                }
                break;
            case STATE_FINAL:
                current_state = handle_final(ev, ch);
                break;
            case STATE_ERROR:
                current_state = handle_error(ev, ch);
                break;
        }
        
        if (current_state == STATE_ERROR) {
            fprintf(stderr, "Invalid input character\n");
            return 1;
        }
    }
    
    if (current_state == STATE_READING) {
        word_count++;
    }
    
    printf("Word count: %d\n", word_count);
    return 0;
}

State handle_start(Event ev, char ch) {
    switch (ev) {
        case EVENT_CHAR:
            return STATE_READING;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_FINAL;
        case EVENT_INVALID:
            return STATE_ERROR;
    }
    return STATE_ERROR;
}

State handle_reading(Event ev, char ch) {
    switch (ev) {
        case EVENT_CHAR:
            return STATE_READING;
        case EVENT_SPACE:
            return STATE_PROCESSING;
        case EVENT_EOF:
            return STATE_FINAL;
        case EVENT_INVALID:
            return STATE_ERROR;
    }
    return STATE_ERROR;
}

State handle_processing(Event ev, char ch) {
    switch (ev) {
        case EVENT_CHAR:
            return STATE_READING;
        case EVENT_SPACE:
            return STATE_PROCESSING;
        case EVENT_EOF:
            return STATE_FINAL;
        case EVENT_INVALID:
            return STATE_ERROR;
    }
    return STATE_ERROR;
}

State handle_final(Event ev, char ch) {
    return STATE_FINAL;
}

State handle_error(Event ev, char ch) {
    return STATE_ERROR;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 256
#define STATE_COUNT 6

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINALIZING,
    STATE_DONE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_NONE,
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_TERMINATOR,
    EVENT_ERROR
} Event;

typedef struct {
    State current_state;
    char *input_ptr;
    char *output_ptr;
    char output_buffer[MAX_INPUT_LEN];
    int char_count;
    int word_count;
} StateMachine;

Event get_next_event(char *ptr, char *end) {
    if (ptr >= end) return EVENT_ERROR;
    if (*ptr == '\0') return EVENT_TERMINATOR;
    if (*ptr == ' ') return EVENT_SPACE;
    if ((*ptr >= 'a' && *ptr <= 'z') || (*ptr >= 'A' && *ptr <= 'Z')) return EVENT_CHAR;
    return EVENT_ERROR;
}

void process_state_machine(StateMachine *sm, char *input) {
    char *input_end = input + strlen(input);
    sm->input_ptr = input;
    sm->output_ptr = sm->output_buffer;
    sm->current_state = STATE_START;
    sm->char_count = 0;
    sm->word_count = 0;

    while (sm->current_state != STATE_DONE && sm->current_state != STATE_ERROR) {
        Event event = get_next_event(sm->input_ptr, input_end);
        
        switch (sm->current_state) {
            case STATE_START:
                if (event == EVENT_CHAR) {
                    sm->current_state = STATE_READING;
                    sm->word_count++;
                    *(sm->output_ptr) = *(sm->input_ptr);
                    sm->output_ptr++;
                    sm->char_count++;
                    sm->input_ptr++;
                } else if (event == EVENT_SPACE) {
                    sm->input_ptr++;
                } else if (event == EVENT_TERMINATOR) {
                    sm->current_state = STATE_DONE;
                } else {
                    sm->current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING:
                if (event == EVENT_CHAR) {
                    *(sm->output_ptr) = *(sm->input_ptr);
                    sm->output_ptr++;
                    sm->char_count++;
                    sm->input_ptr++;
                } else if (event == EVENT_SPACE) {
                    sm->current_state = STATE_PROCESSING;
                    *(sm->output_ptr) = ' ';
                    sm->output_ptr++;
                    sm->input_ptr++;
                } else if (event == EVENT_TERMINATOR) {
                    sm->current_state = STATE_FINALIZING;
                } else {
                    sm->current_state = STATE_ERROR;
                }
                break;
                
            case STATE_PROCESSING:
                if (event == EVENT_CHAR) {
                    sm->current_state = STATE_READING;
                    sm->word_count++;
                    *(sm->output_ptr) = *(sm->input_ptr);
                    sm->output_ptr++;
                    sm->char_count++;
                    sm->input_ptr++;
                } else if (event == EVENT_SPACE) {
                    sm->input_ptr++;
                } else if (event == EVENT_TERMINATOR) {
                    sm->current_state = STATE_FINALIZING;
                } else {
                    sm->current_state = STATE_ERROR;
                }
                break;
                
            case STATE_FINALIZING:
                *(sm->output_ptr) = '\0';
                sm->current_state = STATE_DONE;
                break;
                
            case STATE_DONE:
                break;
                
            default:
                sm->current_state = STATE_ERROR;
                break;
        }
        
        if (sm->output_ptr >= sm->output_buffer + MAX_INPUT_LEN) {
            sm->current_state = STATE_ERROR;
        }
    }
}

int main() {
    char input_buffer[MAX_INPUT_LEN];
    StateMachine sm;
    
    printf("Enter text to process: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    process_state_machine(&sm, input_buffer);
    
    if (sm.current_state == STATE_ERROR) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Processed text: %s\n", sm.output_buffer);
    printf("Characters processed: %d\n", sm.char_count);
    printf("Words counted: %d\n", sm.word_count);
    
    return 0;
}
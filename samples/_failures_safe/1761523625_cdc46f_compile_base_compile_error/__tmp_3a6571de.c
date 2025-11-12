//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_END,
    STATE_ERROR
} State;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_TERMINATOR,
    EVENT_INVALID
} Event;

State process_event(State current, Event ev, char *output, size_t *out_len) {
    static char buffer[MAX_INPUT_LEN];
    static size_t buf_pos = 0;
    
    switch (current) {
        case STATE_START:
            if (ev == EVENT_DIGIT) {
                buffer[0] = 'N';
                buffer[1] = ':';
                buf_pos = 2;
                return STATE_READING_NUMBER;
            } else if (ev == EVENT_LETTER) {
                buffer[0] = 'W';
                buffer[1] = ':';
                buf_pos = 2;
                return STATE_READING_WORD;
            } else if (ev == EVENT_SPACE) {
                return STATE_START;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_NUMBER:
            if (ev == EVENT_DIGIT) {
                if (buf_pos < MAX_INPUT_LEN - 1) {
                    *(buffer + buf_pos) = *(output + (*out_len));
                    buf_pos++;
                }
                return STATE_READING_NUMBER;
            } else if (ev == EVENT_SPACE || ev == EVENT_TERMINATOR) {
                if (buf_pos < MAX_INPUT_LEN - 1) {
                    *(buffer + buf_pos) = '\0';
                } else {
                    buffer[MAX_INPUT_LEN - 1] = '\0';
                }
                strncpy(output, buffer, MAX_INPUT_LEN);
                *out_len = strlen(buffer);
                buf_pos = 0;
                return (ev == EVENT_TERMINATOR) ? STATE_END : STATE_START;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_WORD:
            if (ev == EVENT_LETTER) {
                if (buf_pos < MAX_INPUT_LEN - 1) {
                    *(buffer + buf_pos) = *(output + (*out_len));
                    buf_pos++;
                }
                return STATE_READING_WORD;
            } else if (ev == EVENT_SPACE || ev == EVENT_TERMINATOR) {
                if (buf_pos < MAX_INPUT_LEN - 1) {
                    *(buffer + buf_pos) = '\0';
                } else {
                    buffer[MAX_INPUT_LEN - 1] = '\0';
                }
                strncpy(output, buffer, MAX_INPUT_LEN);
                *out_len = strlen(buffer);
                buf_pos = 0;
                return (ev == EVENT_TERMINATOR) ? STATE_END : STATE_START;
            } else {
                return STATE_ERROR;
            }
            
        default:
            return STATE_ERROR;
    }
}

Event classify_char(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char output[MAX_INPUT_LEN * 2];
    size_t out_pos = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    State current_state = STATE_START;
    char *ptr = input;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        if (ptr >= input + MAX_INPUT_LEN || *ptr == '\0') {
            break;
        }
        
        Event ev = classify_char(*ptr);
        if (ev == EVENT_INVALID) {
            current_state = STATE_ERROR;
            break;
        }
        
        char *out_ptr = output + out_pos;
        size_t temp_len = out_pos;
        
        if (ev != EVENT_SPACE && ev != EVENT_TERMINATOR) {
            if (temp_len < sizeof(output) - 1) {
                *(out_ptr) = *ptr;
                temp_len++;
            }
        }
        
        current_state = process_event(current_state, ev, output, &temp_len);
        out_pos = temp_len;
        
        if (ev != EVENT_TERMINATOR) {
            ptr++;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    if (out_pos < sizeof(output)) {
        output[out_pos] = '\0';
    } else {
        output[sizeof(output) - 1] = '\0';
    }
    
    printf
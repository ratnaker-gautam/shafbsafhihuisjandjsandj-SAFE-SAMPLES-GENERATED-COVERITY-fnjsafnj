//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINALIZING,
    STATE_DONE
} State;

typedef enum {
    EVENT_NONE,
    EVENT_INPUT_RECEIVED,
    EVENT_PROCESS_COMPLETE,
    EVENT_FINALIZE_COMPLETE,
    EVENT_ERROR
} Event;

State transition_table[STATE_COUNT][STATE_COUNT] = {
    {STATE_START, STATE_READING, STATE_START, STATE_START, STATE_START},
    {STATE_READING, STATE_READING, STATE_PROCESSING, STATE_READING, STATE_READING},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_FINALIZING, STATE_PROCESSING},
    {STATE_FINALIZING, STATE_FINALIZING, STATE_FINALIZING, STATE_FINALIZING, STATE_DONE},
    {STATE_DONE, STATE_DONE, STATE_DONE, STATE_DONE, STATE_DONE}
};

Event get_next_event(State current_state, const char *input_buffer, size_t *input_pos) {
    static char input_chars[] = "ABCDE";
    static size_t input_len = 5;

    if (current_state == STATE_START) {
        return EVENT_INPUT_RECEIVED;
    }

    if (current_state == STATE_READING) {
        if (*input_pos < input_len) {
            return EVENT_INPUT_RECEIVED;
        }
        return EVENT_PROCESS_COMPLETE;
    }

    if (current_state == STATE_PROCESSING) {
        return EVENT_FINALIZE_COMPLETE;
    }

    if (current_state == STATE_FINALIZING) {
        return EVENT_NONE;
    }

    return EVENT_ERROR;
}

void process_state(State state, char *input_buffer, size_t *input_pos, char *output_buffer, size_t *output_pos) {
    static char input_chars[] = "ABCDE";
    static size_t input_len = 5;

    if (state == STATE_READING) {
        if (*input_pos < input_len && *input_pos < MAX_INPUT_LEN - 1) {
            *(input_buffer + *input_pos) = *(input_chars + *input_pos);
            (*input_pos)++;
            *(input_buffer + *input_pos) = '\0';
        }
    } else if (state == STATE_PROCESSING) {
        char *src = input_buffer;
        char *dst = output_buffer;
        size_t i = 0;
        while (i < *input_pos && i < MAX_INPUT_LEN - 1) {
            *(dst + i) = *(src + i) + 1;
            i++;
        }
        *output_pos = i;
        if (*output_pos < MAX_INPUT_LEN) {
            *(output_buffer + *output_pos) = '\0';
        }
    } else if (state == STATE_FINALIZING) {
        if (*output_pos > 0 && *output_pos < MAX_INPUT_LEN) {
            char *ptr = output_buffer;
            size_t count = *output_pos;
            while (count > 0) {
                putchar(*ptr);
                ptr++;
                count--;
            }
            putchar('\n');
        }
    }
}

int main(void) {
    State current_state = STATE_START;
    char input_buffer[MAX_INPUT_LEN];
    char output_buffer[MAX_INPUT_LEN];
    size_t input_pos = 0;
    size_t output_pos = 0;

    memset(input_buffer, 0, sizeof(input_buffer));
    memset(output_buffer, 0, sizeof(output_buffer));

    while (current_state != STATE_DONE) {
        Event event = get_next_event(current_state, input_buffer, &input_pos);
        
        if (event == EVENT_ERROR) {
            break;
        }

        State next_state = *(*(transition_table + current_state) + event);
        
        if (next_state >= STATE_COUNT) {
            break;
        }

        process_state(current_state, input_buffer, &input_pos, output_buffer, &output_pos);
        current_state = next_state;
    }

    return 0;
}
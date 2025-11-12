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
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_OTHER
} event_t;

typedef struct {
    int number_count;
    int word_count;
    int error_count;
} stats_t;

static event_t get_event(int c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_OTHER;
}

static parser_state_t handle_state(parser_state_t current_state, event_t event, stats_t *stats) {
    switch (current_state) {
        case STATE_START:
            switch (event) {
                case EVENT_DIGIT: return STATE_READING_NUMBER;
                case EVENT_LETTER: return STATE_READING_WORD;
                case EVENT_SPACE: return STATE_START;
                case EVENT_EOF: return STATE_END;
                default: return STATE_ERROR;
            }
        
        case STATE_READING_NUMBER:
            switch (event) {
                case EVENT_DIGIT: return STATE_READING_NUMBER;
                case EVENT_SPACE:
                    stats->number_count++;
                    return STATE_START;
                case EVENT_EOF:
                    stats->number_count++;
                    return STATE_END;
                default: return STATE_ERROR;
            }
        
        case STATE_READING_WORD:
            switch (event) {
                case EVENT_LETTER: return STATE_READING_WORD;
                case EVENT_SPACE:
                    stats->word_count++;
                    return STATE_START;
                case EVENT_EOF:
                    stats->word_count++;
                    return STATE_END;
                default: return STATE_ERROR;
            }
        
        case STATE_ERROR:
            switch (event) {
                case EVENT_SPACE:
                    stats->error_count++;
                    return STATE_START;
                case EVENT_EOF:
                    stats->error_count++;
                    return STATE_END;
                default: return STATE_ERROR;
            }
        
        default:
            return STATE_ERROR;
    }
}

static int process_input(FILE *input, stats_t *stats) {
    parser_state_t state = STATE_START;
    int c;
    
    while (state != STATE_END) {
        c = fgetc(input);
        if (c == EOF && state != STATE_END) {
            state = handle_state(state, EVENT_EOF, stats);
        } else {
            event_t event = get_event(c);
            state = handle_state(state, event, stats);
        }
        
        if (state == STATE_ERROR && c != EOF) {
            while ((c = fgetc(input)) != EOF && !isspace(c)) {
            }
            if (c == EOF) {
                state = handle_state(state, EVENT_EOF, stats);
            } else {
                state = handle_state(state, EVENT_SPACE, stats);
            }
        }
    }
    
    return 0;
}

int main(void) {
    stats_t stats = {0, 0, 0};
    char input_buffer[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    FILE *temp_input = fmemopen(input_buffer, strlen(input_buffer), "r");
    if (temp_input == NULL) {
        fprintf(stderr, "Error creating input stream\n");
        return 1;
    }
    
    if (process_input(temp_input, &stats) != 0) {
        fclose(temp_input);
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    fclose(temp_input);
    
    printf("Numbers found: %d\n", stats.number_count);
    printf("Words found: %d\n", stats.word_count);
    printf("Errors found: %d\n", stats.error_count);
    
    return 0;
}
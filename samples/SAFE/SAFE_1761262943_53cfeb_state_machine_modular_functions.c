//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
} state_t;

typedef enum {
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_END,
    EVENT_INVALID
} event_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int word_count;
} context_t;

static event_t get_event(char c) {
    if (c == '\0' || c == '\n') {
        return EVENT_END;
    } else if (c == ' ') {
        return EVENT_SPACE;
    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return EVENT_CHAR;
    } else {
        return EVENT_INVALID;
    }
}

static state_t handle_start(event_t event, context_t *ctx) {
    if (event == EVENT_CHAR) {
        ctx->word_count = 1;
        return STATE_READING;
    } else if (event == EVENT_SPACE) {
        return STATE_START;
    } else if (event == EVENT_END) {
        return STATE_DONE;
    } else {
        return STATE_ERROR;
    }
}

static state_t handle_reading(event_t event, context_t *ctx) {
    if (event == EVENT_CHAR) {
        return STATE_READING;
    } else if (event == EVENT_SPACE) {
        return STATE_PROCESSING;
    } else if (event == EVENT_END) {
        return STATE_DONE;
    } else {
        return STATE_ERROR;
    }
}

static state_t handle_processing(event_t event, context_t *ctx) {
    if (event == EVENT_CHAR) {
        ctx->word_count++;
        return STATE_READING;
    } else if (event == EVENT_SPACE) {
        return STATE_PROCESSING;
    } else if (event == EVENT_END) {
        return STATE_DONE;
    } else {
        return STATE_ERROR;
    }
}

static state_t handle_done(event_t event, context_t *ctx) {
    return STATE_DONE;
}

static state_t handle_error(event_t event, context_t *ctx) {
    return STATE_ERROR;
}

static state_t (*state_handlers[])(event_t, context_t*) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_done,
    handle_error
};

static int process_input(const char *input, context_t *ctx) {
    if (input == NULL || ctx == NULL) {
        return -1;
    }
    
    size_t input_len = strnlen(input, MAX_INPUT_LEN);
    if (input_len >= MAX_INPUT_LEN) {
        return -1;
    }
    
    ctx->current_state = STATE_START;
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char current_char = input[i];
        event_t event = get_event(current_char);
        
        state_t next_state = state_handlers[ctx->current_state](event, ctx);
        
        if (next_state >= MAX_STATES) {
            return -1;
        }
        
        ctx->current_state = next_state;
        
        if (ctx->current_state == STATE_ERROR) {
            return -1;
        }
        
        if (ctx->current_state == STATE_DONE) {
            break;
        }
    }
    
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    context_t ctx;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strnlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (process_input(input, &ctx) != 0) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    if (ctx.current_state == STATE_ERROR) {
        fprintf(stderr, "Invalid input detected\n");
        return EXIT_FAILURE;
    }
    
    printf("Word count: %d\n", ctx.word_count);
    
    return EXIT_SUCCESS;
}
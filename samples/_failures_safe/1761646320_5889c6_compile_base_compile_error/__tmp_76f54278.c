//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

struct Context {
    enum State current_state;
    char input_buffer[256];
    char output_buffer[256];
    size_t input_length;
    size_t output_length;
    int error_code;
    int processed_count;
};

struct Transition {
    enum State from_state;
    enum State to_state;
    int (*condition)(struct Context*);
};

int condition_idle_to_reading(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->input_length == 0;
}

int condition_reading_to_processing(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->input_length > 0 && ctx->input_length < sizeof(ctx->input_buffer);
}

int condition_processing_to_writing(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->output_length > 0 && ctx->output_length < sizeof(ctx->output_buffer);
}

int condition_processing_to_error(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->input_length >= sizeof(ctx->input_buffer);
}

int condition_writing_to_idle(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->output_length == 0;
}

int condition_any_to_error(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->error_code != 0;
}

int condition_idle_to_done(struct Context* ctx) {
    if (ctx == NULL) return 0;
    return ctx->processed_count >= 5;
}

struct Transition transitions[] = {
    {STATE_IDLE, STATE_READING, condition_idle_to_reading},
    {STATE_READING, STATE_PROCESSING, condition_reading_to_processing},
    {STATE_PROCESSING, STATE_WRITING, condition_processing_to_writing},
    {STATE_PROCESSING, STATE_ERROR, condition_processing_to_error},
    {STATE_WRITING, STATE_IDLE, condition_writing_to_idle},
    {STATE_IDLE, STATE_DONE, condition_idle_to_done},
    {STATE_READING, STATE_ERROR, condition_any_to_error},
    {STATE_PROCESSING, STATE_ERROR, condition_any_to_error},
    {STATE_WRITING, STATE_ERROR, condition_any_to_error}
};

#define NUM_TRANSITIONS (sizeof(transitions)/sizeof(transitions[0]))

void initialize_context(struct Context* ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    memset(ctx->output_buffer, 0, sizeof(ctx->output_buffer));
    ctx->input_length = 0;
    ctx->output_length = 0;
    ctx->error_code = 0;
    ctx->processed_count = 0;
}

void handle_idle_state(struct Context* ctx) {
    if (ctx == NULL) return;
    printf("State: IDLE\n");
}

void handle_reading_state(struct Context* ctx) {
    if (ctx == NULL) return;
    printf("State: READING - Enter text: ");
    if (fgets(ctx->input_buffer, sizeof(ctx->input_buffer), stdin) != NULL) {
        ctx->input_length = strlen(ctx->input_buffer);
        if (ctx->input_length > 0 && ctx->input_buffer[ctx->input_length - 1] == '\n') {
            ctx->input_buffer[ctx->input_length - 1] = '\0';
            ctx->input_length--;
        }
    } else {
        ctx->error_code = 1;
    }
}

void handle_processing_state(struct Context* ctx) {
    if (ctx == NULL) return;
    printf("State: PROCESSING\n");
    if (ctx->input_length >= sizeof(ctx->output_buffer)) {
        ctx->error_code = 2;
        return;
    }
    
    ctx->output_length = 0;
    for (size_t i = 0; i < ctx->input_length; i++) {
        if (ctx->output_length < sizeof(ctx->output_buffer) - 1) {
            ctx->output_buffer[ctx->output_length++] = toupper((unsigned char)ctx->input_buffer[i]);
        }
    }
    ctx->output_buffer[ctx->output_length] = '\0';
    ctx->processed_count++;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    ctx->input_length = 0;
}

void handle_writing_state(struct Context* ctx) {
    if (ctx == NULL) return;
    printf("State: WRITING - Output: %s\n", ctx->output_buffer);
    memset(ctx->output_buffer, 0, sizeof(ctx->output_buffer));
    ctx->output_length = 0;
}

void handle_error_state(struct Context* ctx) {
    if (ctx == NULL) return;
    printf("State: ERROR - Code: %d\n", ctx->error_code);
    ctx->current_state = STATE_DONE;
}

void handle_done_state(struct Context* ctx) {
    if (ctx == NULL) return;
    printf("State: DONE - Processed %d items\n", ctx->processed_count);
}

typedef void (*StateHandler)(struct Context*);

StateHandler state_hand
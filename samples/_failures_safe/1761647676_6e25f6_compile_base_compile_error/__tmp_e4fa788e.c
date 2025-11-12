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
    STATE_FINISHED,
    STATE_ERROR
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_DONE,
    EVENT_ERROR,
    EVENT_RESET
} event_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} context_t;

static state_t handle_start(context_t *ctx, event_t event);
static state_t handle_reading(context_t *ctx, event_t event);
static state_t handle_processing(context_t *ctx, event_t event);
static state_t handle_finished(context_t *ctx, event_t event);
static state_t handle_error(context_t *ctx, event_t event);

static void initialize_context(context_t *ctx);
static event_t get_next_event(void);
static int validate_input(const char *input, size_t len);
static void process_data(context_t *ctx);

int main(void) {
    context_t ctx;
    initialize_context(&ctx);
    
    int max_iterations = 100;
    int iteration = 0;
    
    while (ctx.current_state != STATE_FINISHED && 
           ctx.current_state != STATE_ERROR &&
           iteration < max_iterations) {
        
        event_t event = get_next_event();
        state_t (*handlers[MAX_STATES])(context_t *, event_t) = {
            handle_start,
            handle_reading,
            handle_processing,
            handle_finished,
            handle_error
        };
        
        if (ctx.current_state < MAX_STATES && handlers[ctx.current_state] != NULL) {
            state_t new_state = handlers[ctx.current_state](&ctx, event);
            ctx.current_state = new_state;
        } else {
            ctx.current_state = STATE_ERROR;
        }
        
        iteration++;
    }
    
    if (ctx.current_state == STATE_FINISHED) {
        printf("Processing completed successfully. Processed %d data items.\n", ctx.data_count);
    } else if (ctx.current_state == STATE_ERROR) {
        printf("Processing terminated due to error.\n");
    } else {
        printf("Processing terminated due to iteration limit.\n");
    }
    
    return ctx.current_state == STATE_FINISHED ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void initialize_context(context_t *ctx) {
    if (ctx == NULL) return;
    
    ctx->current_state = STATE_START;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->data_count = 0;
}

static event_t get_next_event(void) {
    static int call_count = 0;
    call_count++;
    
    if (call_count > 20) {
        return EVENT_DONE;
    }
    
    int random_val = call_count % 7;
    
    switch (random_val) {
        case 0: return EVENT_START;
        case 1: return EVENT_DATA;
        case 2: return EVENT_PROCESS;
        case 3: return EVENT_DONE;
        case 4: return EVENT_ERROR;
        case 5: return EVENT_RESET;
        default: return EVENT_DATA;
    }
}

static int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] == 0) break;
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    
    return 1;
}

static void process_data(context_t *ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) return;
    
    int sum = 0;
    for (size_t i = 0; i < ctx->buffer_len && i < MAX_INPUT_LEN; i++) {
        if (ctx->buffer[i] == 0) break;
        sum += (int)ctx->buffer[i];
    }
    
    printf("Processed data: '%.*s' (sum: %d)\n", 
           (int)ctx->buffer_len, ctx->buffer, sum);
    ctx->data_count++;
}

static state_t handle_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started.\n");
            return STATE_READING;
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_START;
    }
}

static state_t handle_reading(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            {
                char test_data[] = "SampleData123";
                size_t data_len = strlen(test_data);
                
                if (data_len < MAX_INPUT_LEN && validate_input(test_data, data_len)) {
                    strncpy(ctx->buffer, test_data, MAX_INPUT_LEN - 1);
                    ctx->buffer[MAX_INPUT_LEN - 1] = '\0';
                    ctx->buffer_len = data_len;
                    printf("Data
//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
};

struct Context {
    enum State current_state;
    uint32_t counter;
    char buffer[256];
    size_t buffer_len;
};

struct Event {
    int type;
    char data[128];
    size_t data_len;
};

void context_init(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    ctx->counter = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
}

int validate_event(const struct Event *evt) {
    if (evt == NULL) return 0;
    if (evt->data_len >= sizeof(evt->data)) return 0;
    return 1;
}

enum State handle_idle(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return STATE_ERROR;
    
    if (evt->type == 1) {
        if (evt->data_len > 0 && evt->data_len < sizeof(ctx->buffer)) {
            memcpy(ctx->buffer, evt->data, evt->data_len);
            ctx->buffer_len = evt->data_len;
            ctx->counter = 0;
            return STATE_READY;
        }
    }
    return STATE_IDLE;
}

enum State handle_ready(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return STATE_ERROR;
    
    switch (evt->type) {
        case 2:
            return STATE_PROCESSING;
        case 3:
            return STATE_IDLE;
        default:
            return STATE_READY;
    }
}

enum State handle_processing(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return STATE_ERROR;
    
    switch (evt->type) {
        case 4:
            if (ctx->counter < 1000) {
                for (size_t i = 0; i < ctx->buffer_len && i < sizeof(ctx->buffer); i++) {
                    if (ctx->buffer[i] != 0) {
                        ctx->buffer[i] = (char)((ctx->buffer[i] + 1) % 128);
                    }
                }
                ctx->counter++;
            }
            return STATE_PROCESSING;
        case 5:
            return STATE_COMPLETE;
        case 6:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

enum State handle_complete(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return STATE_ERROR;
    
    if (evt->type == 7) {
        return STATE_IDLE;
    }
    return STATE_COMPLETE;
}

enum State handle_error(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return STATE_ERROR;
    
    if (evt->type == 8) {
        context_init(ctx);
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

enum State process_event(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || !validate_event(evt)) {
        return STATE_ERROR;
    }
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle(ctx, evt);
        case STATE_READY:
            return handle_ready(ctx, evt);
        case STATE_PROCESSING:
            return handle_processing(ctx, evt);
        case STATE_COMPLETE:
            return handle_complete(ctx, evt);
        case STATE_ERROR:
            return handle_error(ctx, evt);
        default:
            return STATE_ERROR;
    }
}

void print_state_info(const struct Context *ctx) {
    if (ctx == NULL) return;
    
    printf("Current state: ");
    switch (ctx->current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READY: printf("READY"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_COMPLETE: printf("COMPLETE"); break;
        case STATE_ERROR: printf("ERROR"); break;
        default: printf("UNKNOWN"); break;
    }
    printf(" | Counter: %u | Buffer: ", ctx->counter);
    for (size_t i = 0; i < ctx->buffer_len && i < 16; i++) {
        printf("%c", ctx->buffer[i] >= 32 && ctx->buffer[i] <= 126 ? ctx->buffer[i] : '.');
    }
    printf("\n");
}

int main(void) {
    struct Context ctx;
    struct Event evt;
    int input_result;
    
    context_init(&ctx);
    
    printf("State Machine Demo\n");
    printf("Available events: 1(load), 2(start), 3(cancel), 4(step), 5(finish), 6(error), 7(reset), 8(recover)\n");
    
    while (1) {
        print_state_info(&ctx);
        printf("Enter event type (0 to exit): ");
        
        input_result = scanf("%d", &evt.type);
        if (input_result != 1)
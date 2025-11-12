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
    uint32_t data_value;
    uint32_t counter;
    char buffer[64];
};

struct Event {
    int type;
    uint32_t value;
};

void state_machine_init(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    ctx->data_value = 0;
    ctx->counter = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int validate_event(const struct Event *evt) {
    if (evt == NULL) return 0;
    if (evt->type < 0 || evt->type > 4) return 0;
    return 1;
}

int handle_idle_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 1) {
        ctx->current_state = STATE_READY;
        ctx->data_value = evt->value;
        return 1;
    }
    return 0;
}

int handle_ready_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 2) {
        ctx->current_state = STATE_PROCESSING;
        ctx->counter = 0;
        return 1;
    } else if (evt->type == 0) {
        ctx->current_state = STATE_IDLE;
        return 1;
    }
    return 0;
}

int handle_processing_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 3) {
        if (ctx->counter < 1000) {
            if (evt->value > 1000 - ctx->counter) {
                ctx->counter = 1000;
            } else {
                ctx->counter += evt->value;
            }
            if (ctx->counter >= ctx->data_value) {
                ctx->current_state = STATE_COMPLETE;
            }
            return 1;
        }
    } else if (evt->type == 4) {
        ctx->current_state = STATE_ERROR;
        return 1;
    }
    return 0;
}

int handle_complete_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 0) {
        ctx->current_state = STATE_IDLE;
        return 1;
    }
    return 0;
}

int handle_error_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 0) {
        ctx->current_state = STATE_IDLE;
        return 1;
    }
    return 0;
}

int process_event(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || !validate_event(evt)) return 0;
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle_state(ctx, evt);
        case STATE_READY:
            return handle_ready_state(ctx, evt);
        case STATE_PROCESSING:
            return handle_processing_state(ctx, evt);
        case STATE_COMPLETE:
            return handle_complete_state(ctx, evt);
        case STATE_ERROR:
            return handle_error_state(ctx, evt);
        default:
            return 0;
    }
}

const char* state_to_string(enum State s) {
    switch (s) {
        case STATE_IDLE: return "IDLE";
        case STATE_READY: return "READY";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_COMPLETE: return "COMPLETE";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    struct Context ctx;
    state_machine_init(&ctx);
    struct Event evt;
    int running = 1;
    printf("State Machine Demo\n");
    printf("Commands: 0=Reset, 1=SetValue, 2=Start, 3=Progress, 4=Error\n");
    while (running) {
        printf("Current state: %s\n", state_to_string(ctx.current_state));
        printf("Data: %u, Counter: %u\n", ctx.data_value, ctx.counter);
        printf("Enter command type (0-4, -1 to quit): ");
        int result = scanf("%d", &evt.type);
        if (result != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        if (evt.type == -1) {
            running = 0;
            continue;
        }
        if (evt.type == 1 || evt.type == 3) {
            printf("Enter value: ");
            result = scanf("%u", &evt.value);
            if (result != 1) {
                printf("Invalid value\n");
                while (getchar() != '\n');
                continue;
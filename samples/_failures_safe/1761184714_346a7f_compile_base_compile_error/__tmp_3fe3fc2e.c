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
    uint32_t param;
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
    if (evt->type < 0 || evt->type > 5) return 0;
    return 1;
}

int handle_idle_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    
    if (evt->type == 1) {
        ctx->current_state = STATE_READY;
        ctx->data_value = evt->param;
        return 1;
    }
    return 0;
}

int handle_ready_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    
    switch (evt->type) {
        case 2:
            if (evt->param > 0 && evt->param <= 1000) {
                ctx->current_state = STATE_PROCESSING;
                ctx->counter = evt->param;
                return 1;
            }
            break;
        case 0:
            ctx->current_state = STATE_IDLE;
            return 1;
    }
    return 0;
}

int handle_processing_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    
    switch (evt->type) {
        case 3:
            if (ctx->counter > 0) {
                ctx->counter--;
                if (ctx->counter == 0) {
                    ctx->current_state = STATE_COMPLETE;
                }
                return 1;
            }
            break;
        case 4:
            ctx->current_state = STATE_ERROR;
            return 1;
        case 0:
            ctx->current_state = STATE_IDLE;
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

const char* state_to_string(enum State state) {
    switch (state) {
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
    struct Event evt;
    char input[32];
    int running = 1;
    
    state_machine_init(&ctx);
    
    printf("State Machine Demo\n");
    printf("Commands: 0=Reset, 1=Start(value), 2=Process(count), 3=Step, 4=Error, 5=Quit\n");
    
    while (running) {
        printf("\nCurrent State: %s | Data: %u | Counter: %u\n", 
               state_to_string(ctx.current_state), ctx.data_value, ctx.counter);
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        int cmd;
        uint32_t param = 0;
        
        if (sscanf(input, "%d %u", &cmd, &param) >= 1) {
            evt.type = cmd;
            evt.param = param;
            
            if (cmd == 5) {
                running = 0;
                continue;
            }
            
            if (process_event(&ctx, &evt)) {
                printf("Event processed successfully\n");
            } else {
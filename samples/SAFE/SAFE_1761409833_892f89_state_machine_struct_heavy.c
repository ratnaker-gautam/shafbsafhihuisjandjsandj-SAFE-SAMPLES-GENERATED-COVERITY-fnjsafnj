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

int handle_idle_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return -1;
    
    if (evt->type == 1) {
        ctx->current_state = STATE_READY;
        ctx->data_value = evt->value;
        return 0;
    }
    return -1;
}

int handle_ready_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return -1;
    
    if (evt->type == 2) {
        if (evt->value > 1000) return -1;
        ctx->current_state = STATE_PROCESSING;
        ctx->counter = evt->value;
        return 0;
    } else if (evt->type == 0) {
        ctx->current_state = STATE_IDLE;
        return 0;
    }
    return -1;
}

int handle_processing_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return -1;
    
    if (evt->type == 3) {
        if (ctx->counter == 0) return -1;
        uint32_t temp = ctx->data_value * ctx->counter;
        if (temp / ctx->counter != ctx->data_value) return -1;
        
        if (snprintf(ctx->buffer, sizeof(ctx->buffer), "Result: %u", temp) >= (int)sizeof(ctx->buffer)) {
            return -1;
        }
        ctx->current_state = STATE_COMPLETE;
        return 0;
    } else if (evt->type == 4) {
        ctx->current_state = STATE_ERROR;
        return 0;
    }
    return -1;
}

int handle_complete_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return -1;
    
    if (evt->type == 0) {
        ctx->current_state = STATE_IDLE;
        return 0;
    }
    return -1;
}

int handle_error_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return -1;
    
    if (evt->type == 0) {
        ctx->current_state = STATE_IDLE;
        return 0;
    }
    return -1;
}

int process_event(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return -1;
    
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
            return -1;
    }
}

int main(void) {
    struct Context machine;
    struct Event event;
    char input_buffer[32];
    int result;
    
    state_machine_init(&machine);
    
    printf("State Machine Demo\n");
    printf("Commands: 0=reset, 1=set_value, 2=set_counter, 3=process, 4=error\n");
    
    while (machine.current_state != STATE_COMPLETE && machine.current_state != STATE_ERROR) {
        printf("Current state: %d | Data: %u | Counter: %u\n", 
               machine.current_state, machine.data_value, machine.counter);
        printf("Enter command (0-4) and optional value: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (sscanf(input_buffer, "%d %u", &event.type, &event.value) < 1) {
            printf("Invalid input\n");
            continue;
        }
        
        if (event.type < 0 || event.type > 4) {
            printf("Invalid command\n");
            continue;
        }
        
        result = process_event(&machine, &event);
        if (result != 0) {
            printf("Invalid transition\n");
        }
    }
    
    if (machine.current_state == STATE_COMPLETE) {
        printf("Processing complete: %s\n", machine.buffer);
    } else if (machine.current_state == STATE_ERROR) {
        printf("Machine entered error state\n");
    }
    
    return 0;
}
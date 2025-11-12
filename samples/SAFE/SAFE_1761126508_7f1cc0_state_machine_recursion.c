//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    PROCESSING,
    VALIDATING,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    int data;
    int step;
    char buffer[32];
};

enum State handle_start(struct Context *ctx) {
    if (ctx->step == 0) {
        printf("Enter initial value (0-100): ");
        if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
            return ERROR;
        }
        if (sscanf(ctx->buffer, "%d", &ctx->data) != 1) {
            return ERROR;
        }
        if (ctx->data < 0 || ctx->data > 100) {
            return ERROR;
        }
        ctx->step = 1;
        return PROCESSING;
    }
    return ERROR;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->step == 1) {
        if (ctx->data < 50) {
            ctx->data *= 2;
        } else {
            ctx->data -= 25;
        }
        ctx->step = 2;
        return VALIDATING;
    }
    return ERROR;
}

enum State handle_validating(struct Context *ctx) {
    if (ctx->step == 2) {
        if (ctx->data < 0 || ctx->data > 200) {
            return ERROR;
        }
        ctx->step = 3;
        return FINISHED;
    }
    return ERROR;
}

enum State handle_finished(struct Context *ctx) {
    if (ctx->step == 3) {
        printf("Final result: %d\n", ctx->data);
        return FINISHED;
    }
    return ERROR;
}

enum State handle_error(struct Context *ctx) {
    printf("Error occurred in state machine\n");
    return ERROR;
}

enum State (*state_handlers[])(struct Context *) = {
    handle_start,
    handle_processing,
    handle_validating,
    handle_finished,
    handle_error
};

void run_state_machine(struct Context *ctx) {
    if (ctx->current_state < START || ctx->current_state > ERROR) {
        return;
    }
    
    enum State next_state = state_handlers[ctx->current_state](ctx);
    
    if (next_state != ctx->current_state) {
        ctx->current_state = next_state;
        run_state_machine(ctx);
    }
}

int main(void) {
    struct Context ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = START;
    ctx.step = 0;
    
    run_state_machine(&ctx);
    
    if (ctx.current_state == FINISHED) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
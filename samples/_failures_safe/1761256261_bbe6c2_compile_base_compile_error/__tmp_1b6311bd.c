//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
};

struct Context {
    enum State current_state;
    uint32_t data_value;
    uint8_t retry_count;
    char buffer[64];
};

static int validate_event(enum Event evt) {
    return evt >= EVENT_START && evt <= EVENT_RESET;
}

static int validate_state(enum State st) {
    return st >= STATE_IDLE && st <= STATE_ERROR;
}

static void handle_idle(struct Context *ctx, enum Event evt) {
    switch (evt) {
        case EVENT_START:
            printf("Transition: IDLE -> READY\n");
            ctx->current_state = STATE_READY;
            ctx->retry_count = 0;
            break;
        default:
            printf("Invalid event for IDLE state\n");
            break;
    }
}

static void handle_ready(struct Context *ctx, enum Event evt) {
    switch (evt) {
        case EVENT_DATA_READY:
            if (ctx->data_value < 1000) {
                printf("Transition: READY -> PROCESSING\n");
                ctx->current_state = STATE_PROCESSING;
            } else {
                printf("Data value too large\n");
                ctx->current_state = STATE_ERROR;
            }
            break;
        case EVENT_RESET:
            printf("Transition: READY -> IDLE\n");
            ctx->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for READY state\n");
            break;
    }
}

static void handle_processing(struct Context *ctx, enum Event evt) {
    switch (evt) {
        case EVENT_SUCCESS:
            printf("Transition: PROCESSING -> COMPLETE\n");
            ctx->current_state = STATE_COMPLETE;
            break;
        case EVENT_FAILURE:
            if (ctx->retry_count < 3) {
                printf("Retry attempt %u\n", ctx->retry_count + 1);
                ctx->retry_count++;
                ctx->current_state = STATE_READY;
            } else {
                printf("Max retries exceeded\n");
                ctx->current_state = STATE_ERROR;
            }
            break;
        default:
            printf("Invalid event for PROCESSING state\n");
            break;
    }
}

static void handle_complete(struct Context *ctx, enum Event evt) {
    switch (evt) {
        case EVENT_RESET:
            printf("Transition: COMPLETE -> IDLE\n");
            ctx->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for COMPLETE state\n");
            break;
    }
}

static void handle_error(struct Context *ctx, enum Event evt) {
    switch (evt) {
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            ctx->current_state = STATE_IDLE;
            ctx->retry_count = 0;
            break;
        default:
            printf("Invalid event for ERROR state\n");
            break;
    }
}

static void process_event(struct Context *ctx, enum Event evt) {
    if (!validate_event(evt) || !validate_state(ctx->current_state)) {
        printf("Invalid state or event\n");
        return;
    }

    switch (ctx->current_state) {
        case STATE_IDLE:
            handle_idle(ctx, evt);
            break;
        case STATE_READY:
            handle_ready(ctx, evt);
            break;
        case STATE_PROCESSING:
            handle_processing(ctx, evt);
            break;
        case STATE_COMPLETE:
            handle_complete(ctx, evt);
            break;
        case STATE_ERROR:
            handle_error(ctx, evt);
            break;
    }
}

static enum Event get_user_event(void) {
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_RESET;
    }

    int value;
    if (sscanf(input, "%d", &value) != 1) {
        return EVENT_RESET;
    }

    if (value < 0 || value > 5) {
        return EVENT_RESET;
    }

    return (enum Event)value;
}

int main(void) {
    struct Context ctx = {STATE_IDLE, 0, 0, {0}};

    printf("State Machine Demo\n");
    printf("Events: 0=START, 1=DATA_READY, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET\n");

    while (1) {
        printf("\nCurrent state: ");
        switch (ctx.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf(" | Data: %u | Retries: %u\n", ctx.data_value, ctx.retry_count);

        printf("Enter event (0-5): ");
        enum Event evt = get_user_event();

        if (evt == EVENT_DATA_READY) {
            printf("
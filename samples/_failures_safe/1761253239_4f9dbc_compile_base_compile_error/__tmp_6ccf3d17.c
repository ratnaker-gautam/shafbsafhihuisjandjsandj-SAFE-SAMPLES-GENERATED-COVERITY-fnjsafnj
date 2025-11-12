//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

enum state {
    INIT,
    READ_COMMAND,
    PROCESS_NUMBER,
    DISPLAY_RESULT,
    ERROR,
    EXIT
};

struct context {
    enum state current_state;
    int number;
    int result;
    char buffer[64];
};

void state_init(struct context *ctx) {
    printf("Calculator State Machine\n");
    printf("Commands: add, sub, mul, div, quit\n");
    printf("Enter command: ");
    ctx->current_state = READ_COMMAND;
}

void state_read_command(struct context *ctx) {
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = EXIT;
        return;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
    }
    
    if (strcmp(ctx->buffer, "quit") == 0) {
        ctx->current_state = EXIT;
    } else if (strcmp(ctx->buffer, "add") == 0 || 
               strcmp(ctx->buffer, "sub") == 0 ||
               strcmp(ctx->buffer, "mul") == 0 ||
               strcmp(ctx->buffer, "div") == 0) {
        printf("Enter number: ");
        ctx->current_state = PROCESS_NUMBER;
    } else {
        printf("Invalid command. Try again.\n");
        printf("Enter command: ");
    }
}

void state_process_number(struct context *ctx) {
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = EXIT;
        return;
    }
    
    char *endptr;
    long num = strtol(ctx->buffer, &endptr, 10);
    
    if (endptr == ctx->buffer || (*endptr != '\n' && *endptr != '\0')) {
        printf("Invalid number. Try again.\n");
        printf("Enter number: ");
        return;
    }
    
    if (num < -1000000 || num > 1000000) {
        printf("Number out of range. Try again.\n");
        printf("Enter number: ");
        return;
    }
    
    ctx->number = (int)num;
    ctx->current_state = DISPLAY_RESULT;
}

void state_display_result(struct context *ctx) {
    char *last_cmd = ctx->buffer;
    
    if (strcmp(last_cmd, "add") == 0) {
        if ((ctx->result > 0 && ctx->number > INT_MAX - ctx->result) ||
            (ctx->result < 0 && ctx->number < INT_MIN - ctx->result)) {
            printf("Integer overflow detected.\n");
            ctx->current_state = ERROR;
            return;
        }
        ctx->result += ctx->number;
    } else if (strcmp(last_cmd, "sub") == 0) {
        if ((ctx->number > 0 && ctx->result < INT_MIN + ctx->number) ||
            (ctx->number < 0 && ctx->result > INT_MAX + ctx->number)) {
            printf("Integer overflow detected.\n");
            ctx->current_state = ERROR;
            return;
        }
        ctx->result -= ctx->number;
    } else if (strcmp(last_cmd, "mul") == 0) {
        if (ctx->result > 0 && ctx->number > 0 && ctx->result > INT_MAX / ctx->number) {
            printf("Integer overflow detected.\n");
            ctx->current_state = ERROR;
            return;
        }
        if (ctx->result > 0 && ctx->number < 0 && ctx->number < INT_MIN / ctx->result) {
            printf("Integer overflow detected.\n");
            ctx->current_state = ERROR;
            return;
        }
        if (ctx->result < 0 && ctx->number > 0 && ctx->result < INT_MIN / ctx->number) {
            printf("Integer overflow detected.\n");
            ctx->current_state = ERROR;
            return;
        }
        if (ctx->result < 0 && ctx->number < 0 && ctx->result < INT_MAX / ctx->number) {
            printf("Integer overflow detected.\n");
            ctx->current_state = ERROR;
            return;
        }
        ctx->result *= ctx->number;
    } else if (strcmp(last_cmd, "div") == 0) {
        if (ctx->number == 0) {
            printf("Division by zero.\n");
            ctx->current_state = ERROR;
            return;
        }
        if (ctx->result == INT_MIN && ctx->number == -1) {
            printf("Integer overflow detected.\n");
            ctx->current_state = ERROR;
            return;
        }
        ctx->result /= ctx->number;
    }
    
    printf("Current result: %d\n", ctx->result);
    printf("Enter command: ");
    ctx->current_state = READ_COMMAND;
}

void state_error(struct context *ctx) {
    printf("Error state. Resetting calculator.\n");
    ctx->result = 0;
    printf("Enter command: ");
    ctx->current_state = READ_COMMAND;
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    ctx.result = 0;
    ctx.number = 0;
    
    while (ctx.current_state != EXIT) {
        switch (
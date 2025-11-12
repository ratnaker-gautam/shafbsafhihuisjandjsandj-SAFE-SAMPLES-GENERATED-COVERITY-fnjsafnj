//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    READING,
    PROCESSING,
    OUTPUT,
    ERROR,
    DONE
};

struct context {
    enum state current;
    int data_count;
    int values[10];
    int sum;
    int processed;
};

int validate_input(const char* input, int* value) {
    if (input == NULL) return 0;
    
    char* endptr;
    long val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (val < INT32_MIN || val > INT32_MAX) return 0;
    
    *value = (int)val;
    return 1;
}

void state_init(struct context* ctx) {
    ctx->current = READING;
    ctx->data_count = 0;
    ctx->sum = 0;
    ctx->processed = 0;
    memset(ctx->values, 0, sizeof(ctx->values));
}

void state_reading(struct context* ctx) {
    char buffer[32];
    printf("Enter number %d (or 'done' to finish): ", ctx->data_count + 1);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        ctx->current = ERROR;
        return;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    if (strcmp(buffer, "done") == 0) {
        if (ctx->data_count > 0) {
            ctx->current = PROCESSING;
        } else {
            ctx->current = DONE;
        }
        return;
    }
    
    int value;
    if (validate_input(buffer, &value)) {
        if (ctx->data_count < 10) {
            ctx->values[ctx->data_count] = value;
            ctx->data_count++;
        } else {
            printf("Maximum 10 numbers allowed\n");
            ctx->current = PROCESSING;
        }
    } else {
        printf("Invalid input. Please enter a valid integer or 'done'\n");
    }
}

void state_processing(struct context* ctx) {
    int i = 0;
    while (i < ctx->data_count) {
        ctx->sum += ctx->values[i];
        i++;
    }
    
    for (int j = 0; j < ctx->data_count; j++) {
        ctx->processed++;
    }
    
    ctx->current = OUTPUT;
}

void state_output(struct context* ctx) {
    printf("Processing complete:\n");
    printf("Numbers entered: %d\n", ctx->data_count);
    printf("Sum: %d\n", ctx->sum);
    
    if (ctx->data_count > 0) {
        double average = (double)ctx->sum / ctx->data_count;
        printf("Average: %.2f\n", average);
    }
    
    ctx->current = DONE;
}

void state_error(struct context* ctx) {
    printf("An error occurred during input\n");
    ctx->current = DONE;
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
    while (ctx.current != DONE) {
        switch (ctx.current) {
            case INIT:
                state_init(&ctx);
                break;
            case READING:
                state_reading(&ctx);
                break;
            case PROCESSING:
                state_processing(&ctx);
                break;
            case OUTPUT:
                state_output(&ctx);
                break;
            case ERROR:
                state_error(&ctx);
                break;
            case DONE:
                break;
        }
    }
    
    printf("Program completed successfully\n");
    return 0;
}
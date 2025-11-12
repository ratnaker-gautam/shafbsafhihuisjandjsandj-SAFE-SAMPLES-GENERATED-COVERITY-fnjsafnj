//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    INIT,
    READING,
    PROCESSING,
    OUTPUT,
    DONE,
    ERROR
};

struct Context {
    enum State current_state;
    int data_count;
    int values[10];
    int processed_count;
    int sum;
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 10) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

void state_machine_loop(struct Context* ctx) {
    while (ctx->current_state != DONE && ctx->current_state != ERROR) {
        switch (ctx->current_state) {
            case INIT:
                ctx->data_count = 0;
                ctx->processed_count = 0;
                ctx->sum = 0;
                printf("Enter up to 10 numbers (one per line, 'done' to finish):\n");
                ctx->current_state = READING;
                break;
                
            case READING: {
                char buffer[32];
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    ctx->current_state = ERROR;
                    break;
                }
                buffer[strcspn(buffer, "\n")] = 0;
                
                if (strcmp(buffer, "done") == 0) {
                    if (ctx->data_count == 0) {
                        printf("No data entered.\n");
                        ctx->current_state = ERROR;
                    } else {
                        ctx->current_state = PROCESSING;
                    }
                    break;
                }
                
                if (!validate_input(buffer)) {
                    printf("Invalid input. Enter numbers only.\n");
                    break;
                }
                
                int value = atoi(buffer);
                if (ctx->data_count < 10) {
                    ctx->values[ctx->data_count] = value;
                    ctx->data_count++;
                } else {
                    printf("Maximum 10 numbers allowed.\n");
                    ctx->current_state = PROCESSING;
                }
                break;
            }
            
            case PROCESSING:
                for (ctx->processed_count = 0; ctx->processed_count < ctx->data_count; ctx->processed_count++) {
                    ctx->sum += ctx->values[ctx->processed_count];
                }
                ctx->current_state = OUTPUT;
                break;
                
            case OUTPUT:
                printf("Processed %d numbers. Sum: %d\n", ctx->data_count, ctx->sum);
                ctx->current_state = DONE;
                break;
                
            default:
                ctx->current_state = ERROR;
                break;
        }
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = INIT;
    
    state_machine_loop(&ctx);
    
    if (ctx.current_state == ERROR) {
        printf("Error occurred during processing.\n");
        return 1;
    }
    
    return 0;
}
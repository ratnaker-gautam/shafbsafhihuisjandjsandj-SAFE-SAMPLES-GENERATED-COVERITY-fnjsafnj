//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    INIT,
    READING,
    PROCESSING,
    WRITING,
    DONE,
    ERROR
};

struct Context {
    enum State current_state;
    int data_value;
    int processed_count;
    int max_iterations;
};

int process_data(int value) {
    if (value < 0 || value > 1000) {
        return -1;
    }
    return (value * 2) % 1000;
}

int main(void) {
    struct Context ctx = {INIT, 0, 0, 10};
    int input_value;
    int result;
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case INIT:
                ctx.data_value = 0;
                ctx.processed_count = 0;
                ctx.current_state = READING;
                break;
                
            case READING:
                printf("Enter data value (0-1000) or -1 to exit: ");
                if (scanf("%d", &input_value) != 1) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                if (input_value == -1) {
                    ctx.current_state = DONE;
                    break;
                }
                
                if (input_value < 0 || input_value > 1000) {
                    printf("Invalid input range\n");
                    ctx.current_state = READING;
                    break;
                }
                
                ctx.data_value = input_value;
                ctx.current_state = PROCESSING;
                break;
                
            case PROCESSING:
                result = process_data(ctx.data_value);
                if (result == -1) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                ctx.data_value = result;
                ctx.processed_count++;
                
                if (ctx.processed_count >= ctx.max_iterations) {
                    ctx.current_state = DONE;
                } else {
                    ctx.current_state = WRITING;
                }
                break;
                
            case WRITING:
                printf("Processed value: %d (iteration %d/%d)\n", 
                       ctx.data_value, ctx.processed_count, ctx.max_iterations);
                ctx.current_state = READING;
                break;
                
            case DONE:
                printf("Processing completed. Total iterations: %d\n", ctx.processed_count);
                break;
                
            case ERROR:
                printf("Error occurred during processing\n");
                break;
        }
    }
    
    if (ctx.current_state == ERROR) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
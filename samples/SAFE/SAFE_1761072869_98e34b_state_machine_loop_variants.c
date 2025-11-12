//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum state {
    INIT,
    READING,
    PROCESSING,
    OUTPUT,
    DONE,
    ERROR
};

struct context {
    enum state current_state;
    int data[10];
    int count;
    int sum;
    int valid;
};

int main(void) {
    struct context ctx = {INIT, {0}, 0, 0, 1};
    char input[32];
    int value;
    char *endptr;
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case INIT:
                printf("Enter up to 10 integers (one per line, 'done' to finish):\n");
                ctx.current_state = READING;
                break;
                
            case READING:
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                input[strcspn(input, "\n")] = '\0';
                
                if (strcmp(input, "done") == 0) {
                    if (ctx.count == 0) {
                        printf("No data entered\n");
                        ctx.current_state = ERROR;
                    } else {
                        ctx.current_state = PROCESSING;
                    }
                    break;
                }
                
                value = strtol(input, &endptr, 10);
                if (endptr == input || *endptr != '\0') {
                    printf("Invalid input: %s\n", input);
                    ctx.current_state = ERROR;
                    break;
                }
                
                if (ctx.count >= 10) {
                    printf("Maximum 10 numbers allowed\n");
                    ctx.current_state = ERROR;
                    break;
                }
                
                ctx.data[ctx.count] = value;
                ctx.count++;
                break;
                
            case PROCESSING:
                ctx.sum = 0;
                for (int i = 0; i < ctx.count; i++) {
                    if (ctx.data[i] > 0 && ctx.sum > INT_MAX - ctx.data[i]) {
                        printf("Integer overflow detected\n");
                        ctx.current_state = ERROR;
                        break;
                    }
                    if (ctx.data[i] < 0 && ctx.sum < INT_MIN - ctx.data[i]) {
                        printf("Integer underflow detected\n");
                        ctx.current_state = ERROR;
                        break;
                    }
                    ctx.sum += ctx.data[i];
                }
                if (ctx.current_state != ERROR) {
                    ctx.current_state = OUTPUT;
                }
                break;
                
            case OUTPUT:
                printf("Numbers entered: ");
                for (int i = 0; i < ctx.count; i++) {
                    printf("%d", ctx.data[i]);
                    if (i < ctx.count - 1) {
                        printf(", ");
                    }
                }
                printf("\n");
                printf("Sum: %d\n", ctx.sum);
                printf("Count: %d\n", ctx.count);
                ctx.current_state = DONE;
                break;
                
            case ERROR:
                printf("Error occurred during processing\n");
                ctx.current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    return ctx.valid ? 0 : 1;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
};

struct Context {
    char buffer[256];
    size_t bytes_read;
    size_t bytes_processed;
    size_t bytes_written;
};

int read_data(struct Context *ctx) {
    if (ctx == NULL) return -1;
    
    printf("Enter data to process (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return -1;
    }
    
    ctx->bytes_read = strlen(ctx->buffer);
    if (ctx->bytes_read > 0 && ctx->buffer[ctx->bytes_read - 1] == '\n') {
        ctx->buffer[ctx->bytes_read - 1] = '\0';
        ctx->bytes_read--;
    }
    
    return ctx->bytes_read > 0 ? 0 : -1;
}

int process_data(struct Context *ctx) {
    if (ctx == NULL || ctx->bytes_read == 0) return -1;
    
    for (size_t i = 0; i < ctx->bytes_read; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    ctx->bytes_processed = ctx->bytes_read;
    return 0;
}

int write_data(struct Context *ctx) {
    if (ctx == NULL || ctx->bytes_processed == 0) return -1;
    
    printf("Processed data: %s\n", ctx->buffer);
    ctx->bytes_written = ctx->bytes_processed;
    return 0;
}

enum Event get_user_input(void) {
    printf("\nAvailable events:\n");
    printf("0 - START\n");
    printf("1 - DATA_READY\n");
    printf("2 - PROCESS_COMPLETE\n");
    printf("3 - WRITE_COMPLETE\n");
    printf("4 - ERROR\n");
    printf("5 - RESET\n");
    printf("Choose event (0-5): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return EVENT_ERROR;
    }
    while (getchar() != '\n');
    
    if (choice >= 0 && choice <= 5) {
        return (enum Event)choice;
    }
    
    return EVENT_ERROR;
}

int main(void) {
    enum State current_state = STATE_IDLE;
    struct Context ctx = {0};
    int running = 1;
    
    printf("State Machine Demo - Text Processor\n");
    
    while (running) {
        enum Event event = get_user_input();
        
        switch (current_state) {
            case STATE_IDLE:
                switch (event) {
                    case EVENT_START:
                        printf("State: IDLE -> READING\n");
                        current_state = STATE_READING;
                        break;
                    case EVENT_RESET:
                        printf("Already in IDLE state\n");
                        break;
                    default:
                        printf("Invalid event for IDLE state\n");
                        break;
                }
                break;
                
            case STATE_READING:
                switch (event) {
                    case EVENT_DATA_READY:
                        if (read_data(&ctx) == 0) {
                            printf("State: READING -> PROCESSING\n");
                            current_state = STATE_PROCESSING;
                        } else {
                            printf("Read failed, staying in READING\n");
                        }
                        break;
                    case EVENT_RESET:
                        printf("State: READING -> IDLE\n");
                        current_state = STATE_IDLE;
                        memset(&ctx, 0, sizeof(ctx));
                        break;
                    case EVENT_ERROR:
                        printf("State: READING -> ERROR\n");
                        current_state = STATE_ERROR;
                        break;
                    default:
                        printf("Invalid event for READING state\n");
                        break;
                }
                break;
                
            case STATE_PROCESSING:
                switch (event) {
                    case EVENT_PROCESS_COMPLETE:
                        if (process_data(&ctx) == 0) {
                            printf("State: PROCESSING -> WRITING\n");
                            current_state = STATE_WRITING;
                        } else {
                            printf("Process failed, staying in PROCESSING\n");
                        }
                        break;
                    case EVENT_RESET:
                        printf("State: PROCESSING -> IDLE\n");
                        current_state = STATE_IDLE;
                        memset(&ctx, 0, sizeof(ctx));
                        break;
                    case EVENT_ERROR:
                        printf("State: PROCESSING -> ERROR\n");
                        current_state = STATE_ERROR;
                        break;
                    default:
                        printf("Invalid event for PROCESSING state\n");
                        break;
                }
                break;
                
            case STATE_WRITING:
                switch (event) {
                    case EVENT_WRITE_COMPLETE:
                        if (write_data(&ctx) == 0) {
                            printf("State: WRITING -> DONE\n");
                            current_state = STATE_DONE;
                        } else {
                            printf("Write failed, staying in WRITING\n");
                        }
                        break;
                    case EVENT_RESET:
                        printf("State: WRITING -> IDLE\n");
                        current_state = STATE_IDLE;
                        memset(&ctx
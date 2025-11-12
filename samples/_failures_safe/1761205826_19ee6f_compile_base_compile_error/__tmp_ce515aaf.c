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

struct EventData {
    int32_t value;
    char description[32];
};

struct StateMachine {
    enum State current_state;
    uint32_t transition_count;
    struct EventData last_event;
};

struct Command {
    char name[16];
    int32_t code;
};

struct SystemContext {
    struct StateMachine machine;
    struct Command commands[8];
    size_t command_count;
    uint32_t total_operations;
};

void initialize_system(struct SystemContext *ctx) {
    if (ctx == NULL) return;
    
    ctx->machine.current_state = STATE_IDLE;
    ctx->machine.transition_count = 0;
    ctx->machine.last_event.value = 0;
    memset(ctx->machine.last_event.description, 0, sizeof(ctx->machine.last_event.description));
    
    ctx->command_count = 0;
    ctx->total_operations = 0;
}

int add_command(struct SystemContext *ctx, const char *name, int32_t code) {
    if (ctx == NULL || name == NULL) return 0;
    if (ctx->command_count >= 8) return 0;
    
    size_t name_len = strlen(name);
    if (name_len >= 16) return 0;
    
    strncpy(ctx->commands[ctx->command_count].name, name, 15);
    ctx->commands[ctx->command_count].name[15] = '\0';
    ctx->commands[ctx->command_count].code = code;
    ctx->command_count++;
    
    return 1;
}

enum State handle_event(struct SystemContext *ctx, enum State current, const struct EventData *event) {
    if (ctx == NULL || event == NULL) return current;
    
    switch (current) {
        case STATE_IDLE:
            if (event->value > 0 && event->value < 100) {
                return STATE_READY;
            }
            break;
            
        case STATE_READY:
            if (event->value >= 50 && event->value <= 200) {
                return STATE_PROCESSING;
            } else if (event->value < 0) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event->value == 0) {
                return STATE_COMPLETE;
            } else if (event->value < 0 || event->value > 1000) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_COMPLETE:
            return STATE_IDLE;
            
        case STATE_ERROR:
            if (event->value == 999) {
                return STATE_IDLE;
            }
            break;
    }
    
    return current;
}

int process_command(struct SystemContext *ctx, int32_t command_code) {
    if (ctx == NULL) return 0;
    
    for (size_t i = 0; i < ctx->command_count; i++) {
        if (ctx->commands[i].code == command_code) {
            struct EventData event;
            event.value = command_code;
            snprintf(event.description, sizeof(event.description), "Command_%d", command_code);
            
            enum State new_state = handle_event(ctx, ctx->machine.current_state, &event);
            
            if (new_state != ctx->machine.current_state) {
                ctx->machine.current_state = new_state;
                ctx->machine.transition_count++;
                memcpy(&ctx->machine.last_event, &event, sizeof(event));
            }
            
            ctx->total_operations++;
            return 1;
        }
    }
    
    return 0;
}

void print_state_info(const struct SystemContext *ctx) {
    if (ctx == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("Current State: %s\n", state_names[ctx->machine.current_state]);
    printf("Transitions: %u\n", ctx->machine.transition_count);
    printf("Last Event: %d - %s\n", ctx->machine.last_event.value, ctx->machine.last_event.description);
    printf("Total Operations: %u\n", ctx->total_operations);
    printf("Available Commands:\n");
    
    for (size_t i = 0; i < ctx->command_count; i++) {
        printf("  %s (code: %d)\n", ctx->commands[i].name, ctx->commands[i].code);
    }
    printf("\n");
}

int main(void) {
    struct SystemContext system;
    initialize_system(&system);
    
    add_command(&system, "START", 10);
    add_command(&system, "PROCESS", 75);
    add_command(&system, "FINISH", 0);
    add_command(&system, "RESET", 999);
    add_command(&system, "INVALID", -5);
    
    printf("Initial State:\n");
    print_state_info(&system);
    
    int test_commands[] = {10, 75, 0, 999, 50, -5};
    size_t num_tests = sizeof(test_commands) / sizeof(test_commands[0]);
    
    for (size_t i = 0; i < num_tests; i++) {
        printf("Processing command: %d\n", test_commands[i]);
        
        if (process_command(&system
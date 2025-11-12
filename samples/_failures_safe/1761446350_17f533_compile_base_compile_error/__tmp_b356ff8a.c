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

struct SystemConfig {
    int32_t max_transitions;
    int32_t timeout_value;
    char system_name[16];
};

struct ProcessingContext {
    int32_t data_buffer[8];
    uint32_t data_count;
    int32_t checksum;
};

void initialize_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    sm->last_event.value = 0;
    memset(sm->last_event.description, 0, sizeof(sm->last_event.description));
}

void initialize_system_config(struct SystemConfig *config) {
    if (config == NULL) return;
    config->max_transitions = 100;
    config->timeout_value = 5000;
    strncpy(config->system_name, "StateMachine", sizeof(config->system_name)-1);
    config->system_name[sizeof(config->system_name)-1] = '\0';
}

void initialize_processing_context(struct ProcessingContext *ctx) {
    if (ctx == NULL) return;
    ctx->data_count = 0;
    ctx->checksum = 0;
    for (int i = 0; i < 8; i++) {
        ctx->data_buffer[i] = 0;
    }
}

int validate_event_data(const struct EventData *event) {
    if (event == NULL) return 0;
    if (event->value < -1000 || event->value > 1000) return 0;
    if (strlen(event->description) >= sizeof(event->description)) return 0;
    return 1;
}

enum State handle_idle_state(struct StateMachine *sm, const struct EventData *event, const struct SystemConfig *config) {
    if (sm == NULL || event == NULL || config == NULL) return STATE_ERROR;
    
    if (!validate_event_data(event)) return STATE_ERROR;
    
    if (event->value > 0 && strcmp(event->description, "START") == 0) {
        return STATE_READY;
    }
    
    return STATE_IDLE;
}

enum State handle_ready_state(struct StateMachine *sm, const struct EventData *event, struct ProcessingContext *ctx) {
    if (sm == NULL || event == NULL || ctx == NULL) return STATE_ERROR;
    
    if (!validate_event_data(event)) return STATE_ERROR;
    
    if (event->value >= 1 && event->value <= 8) {
        if (ctx->data_count < 8) {
            ctx->data_buffer[ctx->data_count] = event->value;
            ctx->checksum += event->value;
            ctx->data_count++;
        }
        
        if (ctx->data_count >= 4) {
            return STATE_PROCESSING;
        }
    }
    
    return STATE_READY;
}

enum State handle_processing_state(struct StateMachine *sm, const struct EventData *event, struct ProcessingContext *ctx) {
    if (sm == NULL || event == NULL || ctx == NULL) return STATE_ERROR;
    
    if (!validate_event_data(event)) return STATE_ERROR;
    
    if (strcmp(event->description, "PROCESS") == 0) {
        int32_t result = 0;
        for (uint32_t i = 0; i < ctx->data_count; i++) {
            if (i < 8) {
                result += ctx->data_buffer[i];
            }
        }
        
        if (result == ctx->checksum) {
            return STATE_COMPLETE;
        }
    }
    
    return STATE_PROCESSING;
}

enum State transition_state(struct StateMachine *sm, const struct EventData *event, struct ProcessingContext *ctx, const struct SystemConfig *config) {
    if (sm == NULL || event == NULL || ctx == NULL || config == NULL) return STATE_ERROR;
    
    if (sm->transition_count >= (uint32_t)config->max_transitions) {
        return STATE_ERROR;
    }
    
    enum State new_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            new_state = handle_idle_state(sm, event, config);
            break;
        case STATE_READY:
            new_state = handle_ready_state(sm, event, ctx);
            break;
        case STATE_PROCESSING:
            new_state = handle_processing_state(sm, event, ctx);
            break;
        case STATE_COMPLETE:
        case STATE_ERROR:
            break;
    }
    
    if (new_state != sm->current_state) {
        sm->current_state = new_state;
        sm->transition_count++;
        memcpy(&sm->last_event, event, sizeof(struct EventData));
    }
    
    return new_state;
}

void print_state_info(const struct StateMachine *sm, const struct ProcessingContext *ctx) {
    if (sm == NULL || ctx == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("Current State: %s\n", state_names[sm->current_state]);
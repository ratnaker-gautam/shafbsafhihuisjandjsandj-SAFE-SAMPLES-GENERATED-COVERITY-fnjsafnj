//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

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

struct StateMachine {
    enum State current_state;
    uint32_t data_count;
    uint32_t max_data;
    char buffer[256];
    uint32_t buffer_pos;
};

struct DataRecord {
    uint32_t id;
    char name[32];
    double value;
    uint8_t flags;
};

struct ProcessingContext {
    struct DataRecord *records;
    uint32_t record_count;
    uint32_t max_records;
    double running_total;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 100;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

void processing_context_init(struct ProcessingContext *ctx, uint32_t max_records) {
    if (ctx == NULL || max_records == 0) return;
    ctx->records = malloc(max_records * sizeof(struct DataRecord));
    if (ctx->records == NULL) return;
    ctx->record_count = 0;
    ctx->max_records = max_records;
    ctx->running_total = 0.0;
}

void processing_context_cleanup(struct ProcessingContext *ctx) {
    if (ctx == NULL) return;
    if (ctx->records != NULL) {
        free(ctx->records);
        ctx->records = NULL;
    }
    ctx->record_count = 0;
    ctx->max_records = 0;
    ctx->running_total = 0.0;
}

int validate_data_record(const struct DataRecord *record) {
    if (record == NULL) return 0;
    if (record->id == 0) return 0;
    for (size_t i = 0; i < sizeof(record->name) && record->name[i] != '\0'; i++) {
        if (!isalnum((unsigned char)record->name[i]) && record->name[i] != '_') {
            return 0;
        }
    }
    return 1;
}

enum State handle_event(struct StateMachine *sm, struct ProcessingContext *ctx, enum Event event) {
    if (sm == NULL || ctx == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->current_state = STATE_READING;
                sm->data_count = 0;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA_READY) {
                if (sm->data_count < sm->max_data && ctx->record_count < ctx->max_records) {
                    struct DataRecord new_record;
                    new_record.id = sm->data_count + 1;
                    snprintf(new_record.name, sizeof(new_record.name), "Record_%u", new_record.id);
                    new_record.value = (double)(new_record.id * 10);
                    new_record.flags = 0x01;
                    
                    if (validate_data_record(&new_record)) {
                        ctx->records[ctx->record_count] = new_record;
                        ctx->record_count++;
                        sm->data_count++;
                        sm->current_state = STATE_PROCESSING;
                    } else {
                        sm->current_state = STATE_ERROR;
                    }
                } else {
                    sm->current_state = STATE_ERROR;
                }
            } else if (event == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_PROCESS_COMPLETE) {
                if (ctx->record_count > 0) {
                    ctx->running_total += ctx->records[ctx->record_count - 1].value;
                    sm->current_state = STATE_WRITING;
                } else {
                    sm->current_state = STATE_ERROR;
                }
            } else if (event == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_WRITING:
            if (event == EVENT_WRITE_COMPLETE) {
                if (sm->data_count < sm->max_data) {
                    sm->current_state = STATE_READING;
                } else {
                    sm->current_state = STATE_DONE;
                }
            } else if (event == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_count = 0;
                ctx->record_count = 0;
                ctx->running_total = 0.0;
            }
            break;
            
        case STATE_DONE:
            break;
    }
    
    return sm->current_state;
}

void simulate_state_machine(void) {
    struct StateMachine sm;
    struct ProcessingContext ctx;
    
    state_machine_init(&sm);
    processing_context_init(&ctx, 50);
    
    if (ctx.records == NULL) {
        return;
    }
    
    printf("Starting state machine simulation\n
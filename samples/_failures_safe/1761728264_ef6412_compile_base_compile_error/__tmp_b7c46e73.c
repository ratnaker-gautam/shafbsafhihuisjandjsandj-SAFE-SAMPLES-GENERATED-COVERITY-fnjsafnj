//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
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
    for (int i = 0; i < 31; i++) {
        if (record->name[i] == '\0') break;
        if (!isalnum(record->name[i]) && record->name[i] != '_') return 0;
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
                if (sm->data_count < sm->max_data) {
                    sm->current_state = STATE_PROCESSING;
                } else {
                    sm->current_state = STATE_ERROR;
                }
            } else if (event == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_PROCESS_COMPLETE) {
                if (ctx->record_count < ctx->max_records) {
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
                sm->data_count++;
                if (sm->data_count >= sm->max_data) {
                    sm->current_state = STATE_DONE;
                } else {
                    sm->current_state = STATE_READING;
                }
            } else if (event == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_count = 0;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_count = 0;
            }
            break;
    }
    
    return sm->current_state;
}

int process_data_record(struct ProcessingContext *ctx, const struct DataRecord *record) {
    if (ctx == NULL || record == NULL) return 0;
    if (!validate_data_record(record)) return 0;
    if (ctx->record_count >= ctx->max_records) return 0;
    
    ctx->records[ctx->record_count] = *record;
    ctx->running_total += record->value;
    ctx->record_count++;
    
    return 1;
}

void simulate_data_processing(struct StateMachine *sm, struct ProcessingContext *ctx) {
    if (sm == NULL || ctx == NULL) return;
    
    for (uint32_t i = 0; i < 5; i++) {
        struct DataRecord record;
        record.id = i + 1;
        snprintf(record.name, sizeof(record.name), "record_%u", i + 1);
        record.value = (double)(i + 1);
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
    uint32_t timestamp;
};

struct ProcessingContext {
    struct DataRecord *records;
    uint32_t record_count;
    uint32_t max_records;
    double sum;
    double average;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 100;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

void processing_context_init(struct ProcessingContext *pc, uint32_t max_records) {
    if (pc == NULL || max_records == 0) return;
    pc->records = malloc(max_records * sizeof(struct DataRecord));
    if (pc->records == NULL) return;
    pc->record_count = 0;
    pc->max_records = max_records;
    pc->sum = 0.0;
    pc->average = 0.0;
}

void processing_context_cleanup(struct ProcessingContext *pc) {
    if (pc == NULL) return;
    if (pc->records != NULL) {
        free(pc->records);
        pc->records = NULL;
    }
}

int validate_data_record(const struct DataRecord *record) {
    if (record == NULL) return 0;
    if (record->id == 0) return 0;
    if (strlen(record->name) == 0) return 0;
    for (size_t i = 0; i < strlen(record->name); i++) {
        if (!isalnum(record->name[i])) return 0;
    }
    return 1;
}

enum State handle_event(struct StateMachine *sm, enum Event event, struct ProcessingContext *pc) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->current_state = STATE_READING;
                sm->data_count = 0;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA_READY) {
                if (pc != NULL && pc->record_count < pc->max_records) {
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
                sm->current_state = STATE_WRITING;
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

int read_data_record(struct ProcessingContext *pc) {
    if (pc == NULL || pc->records == NULL) return 0;
    if (pc->record_count >= pc->max_records) return 0;
    
    struct DataRecord *record = &pc->records[pc->record_count];
    record->id = pc->record_count + 1;
    snprintf(record->name, sizeof(record->name), "Record%d", record->id);
    record->value = (double)(record->id * 10) + 0.5;
    record->timestamp = (uint32_t)record->id * 1000;
    
    if (!validate_data_record(record)) return 0;
    
    pc->record_count++;
    return 1;
}

int process_data(struct ProcessingContext *pc) {
    if (pc == NULL || pc->records == NULL) return 0;
    if (pc->record_count == 0) return 0;
    
    pc->sum = 0.0;
    for (uint32_t i = 0; i < pc->record_count; i++) {
        pc->sum += pc->records[i].value;
    }
    pc->
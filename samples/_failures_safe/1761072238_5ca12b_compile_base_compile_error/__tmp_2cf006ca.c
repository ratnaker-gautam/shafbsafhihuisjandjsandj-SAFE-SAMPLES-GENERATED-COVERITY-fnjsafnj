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
    uint8_t valid;
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
    if (record->name[0] == '\0') return 0;
    return 1;
}

void process_data_record(struct ProcessingContext *pc, const struct DataRecord *record) {
    if (pc == NULL || record == NULL) return;
    if (!validate_data_record(record)) return;
    if (pc->record_count >= pc->max_records) return;
    
    pc->records[pc->record_count] = *record;
    pc->sum += record->value;
    pc->record_count++;
    
    if (pc->record_count > 0) {
        pc->average = pc->sum / pc->record_count;
    }
}

enum State handle_event(struct StateMachine *sm, enum Event event, struct ProcessingContext *pc) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            switch (event) {
                case EVENT_START:
                    return STATE_READING;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return STATE_IDLE;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA_READY:
                    if (pc != NULL && pc->record_count < pc->max_records) {
                        return STATE_PROCESSING;
                    }
                    return STATE_READING;
                case EVENT_ERROR:
                    return STATE_ERROR;
                case EVENT_RESET:
                    return STATE_IDLE;
                default:
                    return STATE_READING;
            }
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_PROCESS_COMPLETE:
                    return STATE_WRITING;
                case EVENT_ERROR:
                    return STATE_ERROR;
                case EVENT_RESET:
                    return STATE_IDLE;
                default:
                    return STATE_PROCESSING;
            }
            
        case STATE_WRITING:
            switch (event) {
                case EVENT_WRITE_COMPLETE:
                    if (sm->data_count >= sm->max_data) {
                        return STATE_DONE;
                    }
                    return STATE_READING;
                case EVENT_ERROR:
                    return STATE_ERROR;
                case EVENT_RESET:
                    return STATE_IDLE;
                default:
                    return STATE_WRITING;
            }
            
        case STATE_ERROR:
            switch (event) {
                case EVENT_RESET:
                    return STATE_IDLE;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_DONE:
            switch (event) {
                case EVENT_RESET:
                    return STATE_IDLE;
                default:
                    return STATE_DONE;
            }
            
        default:
            return STATE_ERROR;
    }
}

void simulate_data_reading(struct StateMachine *sm, struct ProcessingContext *pc) {
    if (sm == NULL || pc == NULL) return;
    if (pc->record_count >= pc->max_records) return;
    
    struct DataRecord record;
    record.id = pc->record_count + 1;
    snprintf(record.name, sizeof(record.name), "Record_%u", record.id);
    record.value = (double)(record.id * 10) / 3.0;
    record.valid = 1;
    
    process_data_record(pc, &record);
    sm->data_count++;
}

void write_results(const struct ProcessingContext *pc) {
    if (pc == NULL) return;
    if (pc->
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

void processing_context_init(struct ProcessingContext *pc, uint32_t max_records) {
    if (pc == NULL || max_records == 0) return;
    pc->records = malloc(max_records * sizeof(struct DataRecord));
    if (pc->records == NULL) return;
    pc->record_count = 0;
    pc->max_records = max_records;
    pc->running_total = 0.0;
}

void processing_context_cleanup(struct ProcessingContext *pc) {
    if (pc == NULL) return;
    if (pc->records != NULL) {
        free(pc->records);
        pc->records = NULL;
    }
    pc->record_count = 0;
    pc->max_records = 0;
    pc->running_total = 0.0;
}

int validate_record(const struct DataRecord *rec) {
    if (rec == NULL) return 0;
    if (rec->id == 0) return 0;
    for (size_t i = 0; i < sizeof(rec->name) && rec->name[i] != '\0'; i++) {
        if (!isalnum((unsigned char)rec->name[i]) && rec->name[i] != '_') {
            return 0;
        }
    }
    return 1;
}

enum State handle_event(struct StateMachine *sm, struct ProcessingContext *pc, enum Event event) {
    if (sm == NULL || pc == NULL) return STATE_ERROR;

    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->current_state = STATE_READING;
                sm->data_count = 0;
                pc->record_count = 0;
                pc->running_total = 0.0;
            }
            break;

        case STATE_READING:
            if (event == EVENT_DATA_READY) {
                if (sm->data_count < sm->max_data && pc->record_count < pc->max_records) {
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
                if (pc->record_count < pc->max_records) {
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
                pc->record_count = 0;
                pc->running_total = 0.0;
            }
            break;

        case STATE_DONE:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_count = 0;
                pc->record_count = 0;
                pc->running_total = 0.0;
            }
            break;
    }

    return sm->current_state;
}

int simulate_data_read(struct ProcessingContext *pc) {
    if (pc == NULL || pc->record_count >= pc->max_records) return 0;

    struct DataRecord *rec = &pc->records[pc->record_count];
    rec->id = pc->record_count + 1;
    snprintf(rec->name, sizeof(rec->name), "Record_%u", rec->id);
    rec->value = (double)(rec->id * 10) / 3.0;
    rec->flags = (rec->id % 2) ? 0x01 : 0x02;
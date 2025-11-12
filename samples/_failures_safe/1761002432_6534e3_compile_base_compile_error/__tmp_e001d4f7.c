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

struct DataPacket {
    char content[64];
    uint32_t length;
    uint32_t checksum;
};

struct ProcessingUnit {
    struct DataPacket input;
    struct DataPacket output;
    uint32_t transform_count;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 10;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

uint32_t calculate_checksum(const char *data, uint32_t len) {
    if (data == NULL || len == 0) return 0;
    uint32_t sum = 0;
    for (uint32_t i = 0; i < len; i++) {
        sum = (sum + (uint8_t)data[i]) % 256;
    }
    return sum;
}

int validate_data_packet(const struct DataPacket *packet) {
    if (packet == NULL) return 0;
    if (packet->length > sizeof(packet->content)) return 0;
    if (packet->checksum != calculate_checksum(packet->content, packet->length)) return 0;
    return 1;
}

void process_data_packet(struct ProcessingUnit *pu) {
    if (pu == NULL) return;
    if (!validate_data_packet(&pu->input)) {
        memset(&pu->output, 0, sizeof(pu->output));
        return;
    }
    
    pu->output.length = pu->input.length;
    for (uint32_t i = 0; i < pu->input.length; i++) {
        pu->output.content[i] = toupper((unsigned char)pu->input.content[i]);
    }
    pu->output.checksum = calculate_checksum(pu->output.content, pu->output.length);
    pu->transform_count++;
}

enum State handle_event(struct StateMachine *sm, enum Event event, struct ProcessingUnit *pu) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                state_machine_init(sm);
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA_READY) {
                if (sm->data_count < sm->max_data) {
                    return STATE_PROCESSING;
                } else {
                    return STATE_DONE;
                }
            } else if (event == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_PROCESS_COMPLETE) {
                return STATE_WRITING;
            } else if (event == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_WRITING:
            if (event == EVENT_WRITE_COMPLETE) {
                sm->data_count++;
                return STATE_READING;
            } else if (event == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
                return STATE_IDLE;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
                return STATE_IDLE;
            }
            break;
    }
    
    return sm->current_state;
}

void simulate_data_read(struct DataPacket *packet) {
    if (packet == NULL) return;
    
    const char *sample_data[] = {
        "hello world",
        "state machine",
        "data processing",
        "c programming",
        "struct heavy"
    };
    static uint32_t sample_index = 0;
    
    uint32_t data_len = strlen(sample_data[sample_index]);
    if (data_len >= sizeof(packet->content)) {
        data_len = sizeof(packet->content) - 1;
    }
    
    memcpy(packet->content, sample_data[sample_index], data_len);
    packet->length = data_len;
    packet->checksum = calculate_checksum(packet->content, packet->length);
    
    sample_index = (sample_index + 1) % 5;
}

int main(void) {
    struct StateMachine sm;
    struct ProcessingUnit pu;
    struct DataPacket current_data;
    
    state_machine_init(&sm);
    memset(&pu, 0, sizeof(pu));
    memset(&current_data, 0, sizeof(current_data));
    
    printf("Starting state machine simulation\n");
    
    sm.current_state = handle_event(&sm, EVENT_START, &pu);
    
    while (sm.current_state != STATE_DONE && sm.current_state != STATE_ERROR) {
        switch (sm.current_state
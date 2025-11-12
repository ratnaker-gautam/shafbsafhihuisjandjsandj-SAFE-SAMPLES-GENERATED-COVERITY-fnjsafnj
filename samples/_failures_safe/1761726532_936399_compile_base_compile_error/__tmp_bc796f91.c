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

uint32_t calculate_checksum(const char *data, uint32_t length) {
    if (data == NULL || length == 0) return 0;
    uint32_t sum = 0;
    for (uint32_t i = 0; i < length; i++) {
        sum = (sum + (uint8_t)data[i]) % 256;
    }
    return sum;
}

int validate_packet(const struct DataPacket *packet) {
    if (packet == NULL) return 0;
    if (packet->length > sizeof(packet->content)) return 0;
    if (packet->checksum != calculate_checksum(packet->content, packet->length)) return 0;
    return 1;
}

void process_packet(struct ProcessingUnit *pu) {
    if (pu == NULL) return;
    pu->transform_count = 0;
    if (!validate_packet(&pu->input)) {
        memset(&pu->output, 0, sizeof(pu->output));
        return;
    }
    
    pu->output.length = pu->input.length;
    for (uint32_t i = 0; i < pu->input.length; i++) {
        if (isalpha(pu->input.content[i])) {
            pu->output.content[i] = toupper(pu->input.content[i]);
            pu->transform_count++;
        } else {
            pu->output.content[i] = pu->input.content[i];
        }
    }
    pu->output.checksum = calculate_checksum(pu->output.content, pu->output.length);
}

enum State handle_event(struct StateMachine *sm, enum Event event) {
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
                if (sm->data_count < sm->max_data) {
                    sm->current_state = STATE_PROCESSING;
                } else {
                    sm->current_state = STATE_DONE;
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
                sm->data_count++;
                sm->current_state = STATE_READING;
            } else if (event == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
            }
            break;
            
        case STATE_DONE:
            break;
    }
    
    return sm->current_state;
}

void simulate_data_read(struct DataPacket *packet) {
    if (packet == NULL) return;
    const char *sample_data[] = {
        "hello world",
        "test packet",
        "data stream",
        "state machine",
        "processing unit"
    };
    static uint32_t sample_index = 0;
    
    if (sample_index >= 5) sample_index = 0;
    
    const char *data = sample_data[sample_index];
    uint32_t len = strlen(data);
    if (len >= sizeof(packet->content)) len = sizeof(packet->content) - 1;
    
    memcpy(packet->content, data, len);
    packet->content[len] = '\0';
    packet->length = len;
    packet->checksum = calculate_checksum(packet->content, packet->length);
    sample_index++;
}

int main(void) {
    struct StateMachine sm;
    struct ProcessingUnit pu;
    struct DataPacket current_packet;
    
    state_machine_init(&sm);
    
    printf("Starting state machine simulation\n");
    
    while (sm.current_state != STATE_DONE && sm.current_state != STATE_ERROR) {
        switch (sm.current_state) {
            case
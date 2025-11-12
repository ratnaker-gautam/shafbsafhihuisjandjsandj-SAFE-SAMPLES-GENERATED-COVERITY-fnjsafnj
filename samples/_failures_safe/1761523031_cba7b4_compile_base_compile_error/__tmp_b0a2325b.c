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
    pu->output.length = pu->input.length;
    
    for (uint32_t i = 0; i < pu->input.length; i++) {
        char c = pu->input.content[i];
        if (isalpha(c)) {
            if (islower(c)) {
                pu->output.content[i] = toupper(c);
            } else {
                pu->output.content[i] = tolower(c);
            }
            pu->transform_count++;
        } else {
            pu->output.content[i] = c;
        }
    }
    
    pu->output.checksum = calculate_checksum(pu->output.content, pu->output.length);
}

enum State handle_event(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            switch (event) {
                case EVENT_START:
                    sm->current_state = STATE_READING;
                    sm->data_count = 0;
                    break;
                case EVENT_RESET:
                    state_machine_init(sm);
                    break;
                default:
                    sm->current_state = STATE_ERROR;
                    break;
            }
            break;
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA_READY:
                    if (sm->data_count < sm->max_data) {
                        sm->current_state = STATE_PROCESSING;
                    } else {
                        sm->current_state = STATE_DONE;
                    }
                    break;
                case EVENT_ERROR:
                    sm->current_state = STATE_ERROR;
                    break;
                case EVENT_RESET:
                    state_machine_init(sm);
                    break;
                default:
                    sm->current_state = STATE_ERROR;
                    break;
            }
            break;
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_PROCESS_COMPLETE:
                    sm->current_state = STATE_WRITING;
                    break;
                case EVENT_ERROR:
                    sm->current_state = STATE_ERROR;
                    break;
                case EVENT_RESET:
                    state_machine_init(sm);
                    break;
                default:
                    sm->current_state = STATE_ERROR;
                    break;
            }
            break;
            
        case STATE_WRITING:
            switch (event) {
                case EVENT_WRITE_COMPLETE:
                    sm->data_count++;
                    sm->current_state = STATE_READING;
                    break;
                case EVENT_ERROR:
                    sm->current_state = STATE_ERROR;
                    break;
                case EVENT_RESET:
                    state_machine_init(sm);
                    break;
                default:
                    sm->current_state = STATE_ERROR;
                    break;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
            }
            break;
    }
    
    return sm->current_state;
}

int read_input_data(struct DataPacket *packet) {
    if (packet == NULL) return 0;
    
    printf("Enter data (max 63 characters): ");
    if (fgets(packet->content, sizeof(packet->content), stdin) == NULL) {
        return 0;
    }
    
    packet->length = strlen(packet->content);
    if (packet->length > 0 && packet->content[packet->length - 1] == '\n') {
        packet
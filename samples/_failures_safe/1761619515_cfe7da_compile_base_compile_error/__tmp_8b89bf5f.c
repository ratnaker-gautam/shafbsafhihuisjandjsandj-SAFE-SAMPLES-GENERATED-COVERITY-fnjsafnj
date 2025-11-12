//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
};

struct Transition {
    enum State next_state;
    int (*action)(void*);
};

struct StateMachine {
    enum State current_state;
    struct Transition transitions[STATE_COUNT][EVENT_COUNT];
    char buffer[256];
    size_t buffer_size;
    int error_code;
};

struct DataPacket {
    char data[128];
    size_t size;
    int checksum;
};

int action_idle_start(void* context) {
    struct StateMachine* sm = (struct StateMachine*)context;
    printf("Starting data processing pipeline\n");
    sm->buffer_size = 0;
    sm->error_code = 0;
    return 0;
}

int action_reading_data_ready(void* context) {
    struct StateMachine* sm = (struct StateMachine*)context;
    struct DataPacket packet;
    
    printf("Simulating data read...\n");
    strncpy(packet.data, "Sample data packet", sizeof(packet.data) - 1);
    packet.data[sizeof(packet.data) - 1] = '\0';
    packet.size = strlen(packet.data);
    packet.checksum = 42;
    
    if (packet.size >= sizeof(sm->buffer)) {
        return -1;
    }
    
    strncpy(sm->buffer, packet.data, sizeof(sm->buffer) - 1);
    sm->buffer[sizeof(sm->buffer) - 1] = '\0';
    sm->buffer_size = packet.size;
    
    printf("Read %zu bytes into buffer\n", sm->buffer_size);
    return 0;
}

int action_processing_complete(void* context) {
    struct StateMachine* sm = (struct StateMachine*)context;
    
    if (sm->buffer_size == 0 || sm->buffer_size >= sizeof(sm->buffer)) {
        return -1;
    }
    
    printf("Processing data: ");
    for (size_t i = 0; i < sm->buffer_size && i < sizeof(sm->buffer); i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
        putchar(sm->buffer[i]);
    }
    printf("\n");
    
    return 0;
}

int action_writing_complete(void* context) {
    struct StateMachine* sm = (struct StateMachine*)context;
    
    if (sm->buffer_size == 0) {
        return -1;
    }
    
    printf("Writing %zu bytes to output\n", sm->buffer_size);
    printf("Output: %s\n", sm->buffer);
    return 0;
}

int action_error(void* context) {
    struct StateMachine* sm = (struct StateMachine*)context;
    sm->error_code = 1;
    printf("Error occurred in state machine\n");
    return 0;
}

int action_reset(void* context) {
    struct StateMachine* sm = (struct StateMachine*)context;
    sm->current_state = STATE_IDLE;
    sm->buffer_size = 0;
    sm->error_code = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    printf("State machine reset to idle\n");
    return 0;
}

void initialize_state_machine(struct StateMachine* sm) {
    if (!sm) return;
    
    sm->current_state = STATE_IDLE;
    sm->buffer_size = 0;
    sm->error_code = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    
    for (int s = 0; s < STATE_COUNT; s++) {
        for (int e = 0; e < EVENT_COUNT; e++) {
            sm->transitions[s][e].next_state = STATE_ERROR;
            sm->transitions[s][e].action = action_error;
        }
    }
    
    sm->transitions[STATE_IDLE][EVENT_START].next_state = STATE_READING;
    sm->transitions[STATE_IDLE][EVENT_START].action = action_idle_start;
    
    sm->transitions[STATE_READING][EVENT_DATA_READY].next_state = STATE_PROCESSING;
    sm->transitions[STATE_READING][EVENT_DATA_READY].action = action_reading_data_ready;
    
    sm->transitions[STATE_PROCESSING][EVENT_PROCESS_COMPLETE].next_state = STATE_WRITING;
    sm->transitions[STATE_PROCESSING][EVENT_PROCESS_COMPLETE].action = action_processing_complete;
    
    sm->transitions[STATE_WRITING][EVENT_WRITE_COMPLETE].next_state = STATE_IDLE;
    sm->transitions[STATE_WRITING][EVENT_WRITE_COMPLETE].action = action_writing_complete;
    
    sm->transitions[STATE_ERROR][EVENT_RESET].next_state = STATE_IDLE;
    sm->transitions[STATE_ERROR][EVENT_RESET].action =
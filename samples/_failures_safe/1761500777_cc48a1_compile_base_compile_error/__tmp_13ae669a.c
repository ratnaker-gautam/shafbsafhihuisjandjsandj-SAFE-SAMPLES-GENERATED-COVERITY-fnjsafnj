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

struct Event {
    int type;
    int data;
};

struct StateMachine {
    enum State current_state;
    int counter;
    int max_events;
    int error_code;
};

struct EventQueue {
    struct Event *events;
    int front;
    int rear;
    int capacity;
    int size;
};

int event_queue_init(struct EventQueue *queue, int capacity) {
    if (capacity <= 0) return 0;
    queue->events = malloc(sizeof(struct Event) * capacity);
    if (!queue->events) return 0;
    queue->front = 0;
    queue->rear = -1;
    queue->capacity = capacity;
    queue->size = 0;
    return 1;
}

void event_queue_destroy(struct EventQueue *queue) {
    if (queue->events) free(queue->events);
}

int event_queue_enqueue(struct EventQueue *queue, struct Event event) {
    if (queue->size >= queue->capacity) return 0;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->events[queue->rear] = event;
    queue->size++;
    return 1;
}

int event_queue_dequeue(struct EventQueue *queue, struct Event *event) {
    if (queue->size <= 0) return 0;
    *event = queue->events[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return 1;
}

void state_machine_init(struct StateMachine *sm, int max_events) {
    sm->current_state = STATE_IDLE;
    sm->counter = 0;
    sm->max_events = max_events > 0 ? max_events : 10;
    sm->error_code = 0;
}

int state_machine_handle_event(struct StateMachine *sm, struct Event event) {
    if (event.type < 0 || event.type > 4) return 0;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event.type == 1) {
                sm->current_state = STATE_READY;
                sm->counter = 0;
            }
            break;
        case STATE_READY:
            if (event.type == 2) {
                sm->current_state = STATE_PROCESSING;
            } else if (event.type == 4) {
                sm->current_state = STATE_ERROR;
                sm->error_code = event.data;
            }
            break;
        case STATE_PROCESSING:
            if (event.type == 3) {
                sm->counter++;
                if (sm->counter >= sm->max_events) {
                    sm->current_state = STATE_COMPLETE;
                }
            } else if (event.type == 4) {
                sm->current_state = STATE_ERROR;
                sm->error_code = event.data;
            }
            break;
        case STATE_COMPLETE:
            if (event.type == 1) {
                sm->current_state = STATE_IDLE;
            }
            break;
        case STATE_ERROR:
            if (event.type == 1) {
                sm->current_state = STATE_IDLE;
                sm->error_code = 0;
            }
            break;
    }
    return 1;
}

void print_state_info(struct StateMachine *sm) {
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    printf("Current State: %s", state_names[sm->current_state]);
    if (sm->current_state == STATE_PROCESSING) {
        printf(" (Progress: %d/%d)", sm->counter, sm->max_events);
    } else if (sm->current_state == STATE_ERROR) {
        printf(" (Error Code: %d)", sm->error_code);
    }
    printf("\n");
}

int main(void) {
    struct StateMachine sm;
    struct EventQueue queue;
    int queue_capacity = 20;
    
    if (!event_queue_init(&queue, queue_capacity)) {
        fprintf(stderr, "Failed to initialize event queue\n");
        return 1;
    }
    
    state_machine_init(&sm, 5);
    
    printf("State Machine Demo\n");
    printf("Available events: 1=START, 2=BEGIN_PROCESS, 3=PROCESS_ITEM, 4=ERROR, 0=RESET\n");
    
    int running = 1;
    while (running) {
        print_state_info(&sm);
        printf("Enter event type (-1 to quit): ");
        
        int input;
        if (scanf("%d", &input) != 1) {
            fprintf(stderr, "Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (input == -1) {
            running = 0;
            continue;
        }
        
        struct Event event;
        event.type = input;
        event.data = 0;
        
        if (input == 4) {
            printf("Enter error code: ");
            if (scanf("%d", &event.data) != 1) {
                fprintf(stderr, "Invalid error code\n");
                while (getchar() != '\n');
                continue;
            }
        }
        
        if (!state_machine_
//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum vending_state {
    IDLE,
    SELECTING,
    PROCESSING,
    DISPENSING,
    ERROR
};

struct vending_machine {
    enum vending_state state;
    int selection;
    int balance;
    int item_count;
    int max_items;
    int *item_stock;
};

int initialize_machine(struct vending_machine *machine, int max_items) {
    if (machine == NULL || max_items <= 0) {
        return 0;
    }
    
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    machine->max_items = max_items;
    machine->item_count = 0;
    
    machine->item_stock = malloc(sizeof(int) * max_items);
    if (machine->item_stock == NULL) {
        return 0;
    }
    
    for (int i = 0; i < max_items; i++) {
        machine->item_stock[i] = 5;
    }
    machine->item_count = max_items;
    
    return 1;
}

void cleanup_machine(struct vending_machine *machine) {
    if (machine != NULL && machine->item_stock != NULL) {
        free(machine->item_stock);
        machine->item_stock = NULL;
    }
}

int handle_idle_state(struct vending_machine *machine, int input) {
    if (machine == NULL) {
        return 0;
    }
    
    if (input >= 0 && input < machine->max_items) {
        machine->selection = input;
        machine->state = SELECTING;
        return 1;
    }
    
    return 0;
}

int handle_selecting_state(struct vending_machine *machine, int input) {
    if (machine == NULL) {
        return 0;
    }
    
    if (input > 0) {
        if (machine->balance + input > 1000) {
            machine->state = ERROR;
            return 0;
        }
        machine->balance += input;
        
        if (machine->balance >= 100) {
            machine->state = PROCESSING;
        }
        return 1;
    }
    
    return 0;
}

int handle_processing_state(struct vending_machine *machine) {
    if (machine == NULL) {
        return 0;
    }
    
    if (machine->selection < 0 || machine->selection >= machine->max_items) {
        machine->state = ERROR;
        return 0;
    }
    
    if (machine->item_stock[machine->selection] <= 0) {
        machine->state = ERROR;
        return 0;
    }
    
    if (machine->balance < 100) {
        machine->state = ERROR;
        return 0;
    }
    
    machine->balance -= 100;
    machine->item_stock[machine->selection]--;
    machine->state = DISPENSING;
    return 1;
}

int handle_dispensing_state(struct vending_machine *machine) {
    if (machine == NULL) {
        return 0;
    }
    
    int change = machine->balance;
    machine->balance = 0;
    machine->selection = -1;
    machine->state = IDLE;
    
    return change;
}

int handle_error_state(struct vending_machine *machine) {
    if (machine == NULL) {
        return 0;
    }
    
    machine->selection = -1;
    machine->balance = 0;
    machine->state = IDLE;
    return 1;
}

int process_input(struct vending_machine *machine, int input) {
    if (machine == NULL) {
        return -1;
    }
    
    switch (machine->state) {
        case IDLE:
            return handle_idle_state(machine, input) ? 0 : -1;
            
        case SELECTING:
            return handle_selecting_state(machine, input) ? 0 : -1;
            
        case PROCESSING:
            return handle_processing_state(machine) ? 0 : -1;
            
        case DISPENSING:
            return handle_dispensing_state(machine);
            
        case ERROR:
            return handle_error_state(machine) ? 0 : -1;
            
        default:
            machine->state = ERROR;
            return -1;
    }
}

void print_state(struct vending_machine *machine) {
    if (machine == NULL) {
        return;
    }
    
    printf("Current state: ");
    switch (machine->state) {
        case IDLE:
            printf("IDLE");
            break;
        case SELECTING:
            printf("SELECTING (Item %d)", machine->selection);
            break;
        case PROCESSING:
            printf("PROCESSING");
            break;
        case DISPENSING:
            printf("DISPENSING");
            break;
        case ERROR:
            printf("ERROR");
            break;
    }
    printf(" | Balance: %d\n", machine->balance);
}

int main(void) {
    struct vending_machine machine;
    const int max_items = 5;
    
    if (!initialize_machine(&machine, max_items)) {
        fprintf(stderr, "Failed to initialize vending machine\n");
        return 1;
    }
    
    printf("Vending Machine Simulator\n");
    printf("Items: 0-%d (Price: 100 each)\n", max_items - 1);
    printf("Commands: item_number (0-%d) to select, amount (1-999) to insert,\
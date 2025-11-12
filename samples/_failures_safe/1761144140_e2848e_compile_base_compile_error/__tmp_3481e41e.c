//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
    if (machine == NULL || max_items <= 0) return 0;
    
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    machine->max_items = max_items;
    machine->item_count = max_items;
    
    machine->item_stock = malloc(sizeof(int) * max_items);
    if (machine->item_stock == NULL) return 0;
    
    for (int i = 0; i < max_items; i++) {
        machine->item_stock[i] = 5;
    }
    
    return 1;
}

void cleanup_machine(struct vending_machine *machine) {
    if (machine != NULL && machine->item_stock != NULL) {
        free(machine->item_stock);
    }
}

int handle_idle_state(struct vending_machine *machine, int input) {
    if (input >= 0 && input < machine->item_count) {
        machine->selection = input;
        machine->state = SELECTING;
        return 1;
    }
    return 0;
}

int handle_selecting_state(struct vending_machine *machine, int input) {
    if (input == 1) {
        if (machine->selection >= 0 && machine->selection < machine->item_count) {
            if (machine->item_stock[machine->selection] > 0) {
                machine->state = PROCESSING;
                return 1;
            }
        }
        machine->state = ERROR;
    } else if (input == 0) {
        machine->selection = -1;
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_processing_state(struct vending_machine *machine, int input) {
    if (input == 50) {
        if (machine->balance <= INT_MAX - 50) {
            machine->balance += 50;
            if (machine->balance >= 100) {
                machine->state = DISPENSING;
                return 1;
            }
        }
    } else if (input == 0) {
        machine->balance = 0;
        machine->selection = -1;
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_dispensing_state(struct vending_machine *machine) {
    if (machine->selection >= 0 && machine->selection < machine->item_count) {
        if (machine->item_stock[machine->selection] > 0) {
            machine->item_stock[machine->selection]--;
            machine->balance = 0;
            machine->selection = -1;
            machine->state = IDLE;
            return 1;
        }
    }
    machine->state = ERROR;
    return 0;
}

void handle_error_state(struct vending_machine *machine) {
    machine->balance = 0;
    machine->selection = -1;
    machine->state = IDLE;
}

void print_state(struct vending_machine *machine) {
    printf("Current state: ");
    switch (machine->state) {
        case IDLE: printf("IDLE"); break;
        case SELECTING: printf("SELECTING"); break;
        case PROCESSING: printf("PROCESSING"); break;
        case DISPENSING: printf("DISPENSING"); break;
        case ERROR: printf("ERROR"); break;
    }
    printf("\n");
    
    if (machine->state == IDLE) {
        printf("Items available:\n");
        for (int i = 0; i < machine->item_count; i++) {
            printf("  %d: Stock %d\n", i, machine->item_stock[i]);
        }
        printf("Enter item number to select: ");
    } else if (machine->state == SELECTING) {
        printf("Selected item: %d\n", machine->selection);
        printf("Enter 1 to continue, 0 to cancel: ");
    } else if (machine->state == PROCESSING) {
        printf("Balance: %d, Required: 100\n", machine->balance);
        printf("Insert 50 coins (enter 50) or 0 to cancel: ");
    } else if (machine->state == DISPENSING) {
        printf("Dispensing item %d\n", machine->selection);
    } else if (machine->state == ERROR) {
        printf("Error occurred, returning to idle\n");
    }
}

int get_valid_input(void) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
    
    char *endptr;
    long value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return -1;
    if (value < INT_MIN || value > INT_MAX) return -1;
    
    return (int)value;
}

int main(void) {
    struct vending_machine machine;
    
    if (!initialize_machine(&machine, 5)) {
        fprintf(stderr, "Failed to initialize vending machine\n
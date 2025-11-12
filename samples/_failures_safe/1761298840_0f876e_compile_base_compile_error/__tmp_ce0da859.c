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
        return 0;
    } else if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        return 1;
    }
    return 0;
}

int handle_processing_state(struct vending_machine *machine, int input) {
    if (input >= 1 && input <= 5) {
        machine->balance += input;
        if (machine->balance >= 3) {
            machine->state = DISPENSING;
            return 1;
        }
        return 1;
    } else if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
        return 1;
    }
    return 0;
}

int handle_dispensing_state(struct vending_machine *machine) {
    if (machine->selection >= 0 && machine->selection < machine->item_count) {
        if (machine->item_stock[machine->selection] > 0) {
            machine->item_stock[machine->selection]--;
            machine->balance -= 3;
            if (machine->balance < 0) machine->balance = 0;
            machine->selection = -1;
            machine->state = IDLE;
            return 1;
        }
    }
    machine->state = ERROR;
    return 0;
}

void handle_error_state(struct vending_machine *machine) {
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
}

void print_state(struct vending_machine *machine) {
    printf("Current state: ");
    switch (machine->state) {
        case IDLE:
            printf("IDLE - Ready for selection\n");
            printf("Available items (0-%d): ", machine->item_count - 1);
            for (int i = 0; i < machine->item_count; i++) {
                printf("%d[%d] ", i, machine->item_stock[i]);
            }
            printf("\n");
            break;
        case SELECTING:
            printf("SELECTING - Item %d selected\n", machine->selection);
            printf("Enter 1 to continue, 0 to cancel: ");
            break;
        case PROCESSING:
            printf("PROCESSING - Insert coins (1-5), need %d more\n", 3 - machine->balance);
            printf("Enter coin amount (1-5) or 0 to cancel: ");
            break;
        case DISPENSING:
            printf("DISPENSING - Dispensing item\n");
            break;
        case ERROR:
            printf("ERROR - Resetting\n");
            break;
    }
}

int get_valid_input(int min, int max) {
    int input;
    char buffer[100];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
    
    if (sscanf(buffer, "%d", &input) != 1) return -1;
    
    if (input >= min && input <= max) return input;
    
    return -1;
}

int main(void) {
    struct vending_machine machine;
    int max_items = 4;
    
    if (!initialize_machine(&machine, max_items)) {
        fprintf(stderr, "Failed to initialize vending machine\n");
        return 1;
    }
    
    printf("Vending Machine Simulator\n");
    printf("Commands: item_number (0-%d), 1=confirm, 0=cancel, coins (1-5)\n", max_items - 1);
    
    int running = 1;
    while (running) {
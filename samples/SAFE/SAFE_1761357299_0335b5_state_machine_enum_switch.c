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
};

void init_machine(struct vending_machine *machine) {
    if (machine == NULL) return;
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    machine->item_count = 5;
    machine->max_items = 10;
}

int handle_idle(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input >= 1 && input <= 3) {
        machine->selection = input;
        machine->state = SELECTING;
        return 1;
    }
    return 0;
}

int handle_selecting(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        return 1;
    }
    if (input >= 1 && input <= 3) {
        machine->selection = input;
        return 1;
    }
    if (input == 9) {
        if (machine->item_count > 0) {
            machine->state = PROCESSING;
            return 1;
        } else {
            machine->state = ERROR;
            return 0;
        }
    }
    return 0;
}

int handle_processing(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 5) {
        machine->balance += 5;
        if (machine->balance >= 10) {
            machine->state = DISPENSING;
            return 1;
        }
        return 1;
    }
    if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
        return 1;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    if (machine == NULL) return 0;
    if (machine->item_count > 0) {
        machine->item_count--;
        machine->balance = 0;
        machine->selection = -1;
        machine->state = IDLE;
        return 1;
    }
    machine->state = ERROR;
    return 0;
}

int handle_error(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
        return 1;
    }
    return 0;
}

void print_state(struct vending_machine *machine) {
    if (machine == NULL) return;
    switch (machine->state) {
        case IDLE:
            printf("State: IDLE\n");
            printf("Available items: %d\n", machine->item_count);
            printf("Select item (1-3) or 0 to exit: ");
            break;
        case SELECTING:
            printf("State: SELECTING\n");
            printf("Selected item: %d\n", machine->selection);
            printf("Press 1-3 to change, 9 to buy, 0 to cancel: ");
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
            printf("Selected item: %d\n", machine->selection);
            printf("Balance: %d\n", machine->balance);
            printf("Insert 5 coins (press 5) or 0 to cancel: ");
            break;
        case DISPENSING:
            printf("State: DISPENSING\n");
            printf("Dispensing item %d\n", machine->selection);
            break;
        case ERROR:
            printf("State: ERROR\n");
            printf("Out of items or invalid operation\n");
            printf("Press 0 to continue: ");
            break;
    }
}

int main(void) {
    struct vending_machine machine;
    int input;
    
    init_machine(&machine);
    
    while (1) {
        print_state(&machine);
        
        if (machine.state == DISPENSING) {
            if (handle_dispensing(&machine)) {
                continue;
            }
        }
        
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            continue;
        }
        
        if (input < 0 || input > 9) {
            continue;
        }
        
        switch (machine.state) {
            case IDLE:
                if (input == 0) {
                    return 0;
                }
                handle_idle(&machine, input);
                break;
            case SELECTING:
                handle_selecting(&machine, input);
                break;
            case PROCESSING:
                handle_processing(&machine, input);
                break;
            case ERROR:
                handle_error(&machine, input);
                break;
            case DISPENSING:
                break;
        }
    }
    
    return 0;
}
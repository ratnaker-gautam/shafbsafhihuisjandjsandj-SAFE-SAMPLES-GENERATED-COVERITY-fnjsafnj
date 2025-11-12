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
    if (input == 1) {
        machine->state = SELECTING;
        return 1;
    }
    return 0;
}

int handle_selecting(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input >= 0 && input <= 2) {
        machine->selection = input;
        machine->state = PROCESSING;
        return 1;
    }
    return 0;
}

int handle_processing(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 1) {
        if (machine->balance < 100) {
            machine->balance += 1;
        }
        return 1;
    } else if (input == 2) {
        if (machine->balance >= 2) {
            if (machine->item_count > 0) {
                machine->balance -= 2;
                machine->item_count--;
                machine->state = DISPENSING;
            } else {
                machine->state = ERROR;
            }
        } else {
            machine->state = ERROR;
        }
        return 1;
    } else if (input == 3) {
        machine->state = IDLE;
        machine->balance = 0;
        machine->selection = -1;
        return 1;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 1) {
        machine->state = IDLE;
        machine->selection = -1;
        return 1;
    }
    return 0;
}

int handle_error(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 1) {
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
            printf("Options: 1-Start selection, 0-Exit\n");
            break;
        case SELECTING:
            printf("State: SELECTING\n");
            printf("Options: 0-Water, 1-Soda, 2-Juice\n");
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
            printf("Selection: %d, Balance: %d\n", machine->selection, machine->balance);
            printf("Options: 1-Insert coin, 2-Confirm, 3-Cancel\n");
            break;
        case DISPENSING:
            printf("State: DISPENSING\n");
            printf("Item dispensed! Remaining: %d\n", machine->item_count);
            printf("Options: 1-Continue\n");
            break;
        case ERROR:
            printf("State: ERROR\n");
            printf("Insufficient funds or out of stock\n");
            printf("Options: 1-Reset\n");
            break;
    }
}

int get_valid_input(void) {
    char buffer[16];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    char *endptr;
    long value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    if (value < 0 || value > 9) {
        return -1;
    }
    return (int)value;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    int running = 1;
    
    while (running) {
        print_state(&machine);
        printf("Enter choice: ");
        
        int input = get_valid_input();
        if (input == -1) {
            printf("Invalid input. Please enter a number 0-9.\n");
            continue;
        }
        
        int handled = 0;
        switch (machine.state) {
            case IDLE:
                handled = handle_idle(&machine, input);
                if (input == 0) {
                    running = 0;
                    handled = 1;
                }
                break;
            case SELECTING:
                handled = handle_selecting(&machine, input);
                break;
            case PROCESSING:
                handled = handle_processing(&machine, input);
                break;
            case DISPENSING:
                handled = handle_dispensing(&machine, input);
                break;
            case ERROR:
                handled = handle_error(&machine, input);
                break;
        }
        
        if (!hand
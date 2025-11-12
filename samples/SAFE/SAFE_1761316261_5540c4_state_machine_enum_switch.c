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
        machine->balance += 1;
    } else if (input == 2) {
        if (machine->balance >= 2) {
            if (machine->item_count > 0) {
                machine->item_count--;
                machine->balance -= 2;
                machine->state = DISPENSING;
                return 1;
            } else {
                machine->state = ERROR;
                return 0;
            }
        }
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
            break;
        case SELECTING:
            printf("State: SELECTING\n");
            break;
        case PROCESSING:
            printf("State: PROCESSING (Balance: %d)\n", machine->balance);
            break;
        case DISPENSING:
            printf("State: DISPENSING\n");
            break;
        case ERROR:
            printf("State: ERROR\n");
            break;
    }
}

void print_menu(struct vending_machine *machine) {
    if (machine == NULL) return;
    switch (machine->state) {
        case IDLE:
            printf("1. Start selection\n");
            break;
        case SELECTING:
            printf("Select item (0-2): ");
            break;
        case PROCESSING:
            printf("1. Insert coin\n2. Purchase\n3. Cancel\n");
            break;
        case DISPENSING:
            printf("1. Collect item\n");
            break;
        case ERROR:
            printf("1. Return to idle\n");
            break;
    }
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    int input;
    int result;
    
    printf("Vending Machine Simulator\n");
    
    while (1) {
        print_state(&machine);
        print_menu(&machine);
        
        result = scanf("%d", &input);
        if (result != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (input < 0 || input > 3) {
            printf("Invalid choice\n");
            continue;
        }
        
        switch (machine.state) {
            case IDLE:
                handle_idle(&machine, input);
                break;
            case SELECTING:
                handle_selecting(&machine, input);
                break;
            case PROCESSING:
                handle_processing(&machine, input);
                break;
            case DISPENSING:
                handle_dispensing(&machine, input);
                break;
            case ERROR:
                handle_error(&machine, input);
                break;
        }
        
        if (machine.state == DISPENSING) {
            printf("Item dispensed!\n");
        } else if (machine.state == ERROR) {
            printf("Out of stock!\n");
        }
        
        printf("\n");
    }
    
    return 0;
}
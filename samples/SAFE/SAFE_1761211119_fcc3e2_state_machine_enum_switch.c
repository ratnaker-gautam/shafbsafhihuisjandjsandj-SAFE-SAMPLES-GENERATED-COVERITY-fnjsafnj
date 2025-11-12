//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    machine->item_count = 10;
    machine->max_items = 20;
}

int handle_idle(struct vending_machine *machine, int input) {
    if (machine == NULL) return -1;
    if (input == 1) {
        machine->state = SELECTING;
        return 0;
    } else if (input == 2) {
        machine->balance += 5;
        return 0;
    }
    return -1;
}

int handle_selecting(struct vending_machine *machine, int input) {
    if (machine == NULL) return -1;
    if (input >= 0 && input <= 9) {
        machine->selection = input;
        machine->state = PROCESSING;
        return 0;
    } else if (input == 99) {
        machine->state = IDLE;
        return 0;
    }
    return -1;
}

int handle_processing(struct vending_machine *machine) {
    if (machine == NULL) return -1;
    if (machine->selection < 0 || machine->selection > 9) {
        machine->state = ERROR;
        return -1;
    }
    if (machine->balance < 3) {
        machine->state = ERROR;
        return -1;
    }
    if (machine->item_count <= 0) {
        machine->state = ERROR;
        return -1;
    }
    machine->balance -= 3;
    machine->item_count--;
    machine->state = DISPENSING;
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    if (machine == NULL) return -1;
    machine->selection = -1;
    machine->state = IDLE;
    return 0;
}

int handle_error(struct vending_machine *machine) {
    if (machine == NULL) return -1;
    machine->selection = -1;
    machine->state = IDLE;
    return 0;
}

void print_state(struct vending_machine *machine) {
    if (machine == NULL) return;
    switch (machine->state) {
        case IDLE:
            printf("State: IDLE | Balance: %d | Items: %d\n", machine->balance, machine->item_count);
            printf("Options: 1=Select, 2=Add $5\n");
            break;
        case SELECTING:
            printf("State: SELECTING | Balance: %d\n", machine->balance);
            printf("Select item (0-9) or 99=Cancel\n");
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
            break;
        case DISPENSING:
            printf("State: DISPENSING | Item dispensed!\n");
            break;
        case ERROR:
            printf("State: ERROR | Insufficient funds or out of stock\n");
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
        
        if (machine.state == IDLE || machine.state == SELECTING) {
            if (scanf("%d", &input) != 1) {
                while (getchar() != '\n');
                continue;
            }
        } else {
            input = 0;
        }
        
        switch (machine.state) {
            case IDLE:
                result = handle_idle(&machine, input);
                if (result != 0) {
                    printf("Invalid input\n");
                }
                break;
            case SELECTING:
                result = handle_selecting(&machine, input);
                if (result != 0) {
                    printf("Invalid selection\n");
                }
                break;
            case PROCESSING:
                result = handle_processing(&machine);
                if (result != 0) {
                    printf("Processing failed\n");
                }
                break;
            case DISPENSING:
                result = handle_dispensing(&machine);
                break;
            case ERROR:
                result = handle_error(&machine);
                break;
        }
        
        printf("---\n");
    }
    
    return 0;
}
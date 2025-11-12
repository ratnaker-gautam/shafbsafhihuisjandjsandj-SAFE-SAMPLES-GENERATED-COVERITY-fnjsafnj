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
    int inventory[5];
};

void init_machine(struct vending_machine *machine) {
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    for (int i = 0; i < 5; i++) {
        machine->inventory[i] = 3;
    }
}

int handle_idle(struct vending_machine *machine) {
    printf("Current state: IDLE\n");
    printf("Options: 1-Insert coin 2-Select item 3-Exit\n");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return 0;
    }
    
    switch (choice) {
        case 1:
            machine->balance += 25;
            printf("Balance: %d cents\n", machine->balance);
            return 0;
        case 2:
            machine->state = SELECTING;
            return 0;
        case 3:
            return 1;
        default:
            printf("Invalid choice\n");
            return 0;
    }
}

int handle_selecting(struct vending_machine *machine) {
    printf("Current state: SELECTING\n");
    printf("Items: 1-Soda(75) 2-Chips(50) 3-Candy(45) 4-Water(60) 5-Coffee(80)\n");
    printf("Balance: %d cents\n", machine->balance);
    
    if (scanf("%d", &machine->selection) != 1) {
        while (getchar() != '\n');
        printf("Invalid selection\n");
        return 0;
    }
    
    if (machine->selection < 1 || machine->selection > 5) {
        printf("Invalid selection\n");
        machine->selection = -1;
        return 0;
    }
    
    machine->state = PROCESSING;
    return 0;
}

int handle_processing(struct vending_machine *machine) {
    printf("Current state: PROCESSING\n");
    
    int item_index = machine->selection - 1;
    int prices[] = {75, 50, 45, 60, 80};
    
    if (machine->inventory[item_index] <= 0) {
        printf("Item out of stock\n");
        machine->state = ERROR;
        return 0;
    }
    
    if (machine->balance < prices[item_index]) {
        printf("Insufficient funds. Need %d, have %d\n", prices[item_index], machine->balance);
        machine->state = ERROR;
        return 0;
    }
    
    machine->balance -= prices[item_index];
    machine->inventory[item_index]--;
    machine->state = DISPENSING;
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    printf("Current state: DISPENSING\n");
    printf("Dispensing item %d\n", machine->selection);
    printf("Change returned: %d cents\n", machine->balance);
    
    machine->balance = 0;
    machine->selection = -1;
    machine->state = IDLE;
    return 0;
}

int handle_error(struct vending_machine *machine) {
    printf("Current state: ERROR\n");
    printf("Options: 1-Continue 2-Cancel\n");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return 0;
    }
    
    switch (choice) {
        case 1:
            machine->state = IDLE;
            machine->selection = -1;
            break;
        case 2:
            printf("Returning %d cents\n", machine->balance);
            machine->balance = 0;
            machine->selection = -1;
            machine->state = IDLE;
            break;
        default:
            printf("Invalid choice\n");
            return 0;
    }
    return 0;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    int exit_flag = 0;
    
    printf("Vending Machine Simulator\n");
    
    while (!exit_flag) {
        switch (machine.state) {
            case IDLE:
                exit_flag = handle_idle(&machine);
                break;
            case SELECTING:
                exit_flag = handle_selecting(&machine);
                break;
            case PROCESSING:
                exit_flag = handle_processing(&machine);
                break;
            case DISPENSING:
                exit_flag = handle_dispensing(&machine);
                break;
            case ERROR:
                exit_flag = handle_error(&machine);
                break;
        }
        
        if (exit_flag) {
            break;
        }
        
        printf("\n");
    }
    
    printf("Goodbye!\n");
    return 0;
}
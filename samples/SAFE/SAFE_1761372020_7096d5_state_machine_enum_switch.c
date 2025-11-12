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
    printf("Options: 1-Select item, 2-Insert coin, 3-Exit\n");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return 0;
    }
    
    switch (choice) {
        case 1:
            machine->state = SELECTING;
            break;
        case 2:
            if (machine->balance < 100) {
                machine->balance += 25;
                printf("Balance: %d cents\n", machine->balance);
            } else {
                printf("Maximum balance reached\n");
            }
            break;
        case 3:
            return 1;
        default:
            printf("Invalid choice\n");
    }
    return 0;
}

int handle_selecting(struct vending_machine *machine) {
    printf("Current state: SELECTING\n");
    printf("Items: 0-Soda(75), 1-Chips(50), 2-Candy(45), 3-Water(60), 4-Cookie(40)\n");
    printf("Select item (0-4) or 5 to cancel: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        machine->state = IDLE;
        return 0;
    }
    
    if (choice == 5) {
        machine->state = IDLE;
        return 0;
    }
    
    if (choice >= 0 && choice <= 4) {
        if (machine->inventory[choice] > 0) {
            machine->selection = choice;
            machine->state = PROCESSING;
        } else {
            printf("Item out of stock\n");
            machine->state = IDLE;
        }
    } else {
        printf("Invalid selection\n");
        machine->state = IDLE;
    }
    return 0;
}

int handle_processing(struct vending_machine *machine) {
    printf("Current state: PROCESSING\n");
    
    int prices[] = {75, 50, 45, 60, 40};
    int price = prices[machine->selection];
    
    printf("Selected item %d, price: %d cents\n", machine->selection, price);
    printf("Your balance: %d cents\n", machine->balance);
    
    if (machine->balance >= price) {
        machine->balance -= price;
        machine->state = DISPENSING;
    } else {
        printf("Insufficient funds. Need %d more cents\n", price - machine->balance);
        machine->state = IDLE;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    printf("Current state: DISPENSING\n");
    
    char *items[] = {"Soda", "Chips", "Candy", "Water", "Cookie"};
    printf("Dispensing: %s\n", items[machine->selection]);
    
    machine->inventory[machine->selection]--;
    printf("Remaining inventory: %d\n", machine->inventory[machine->selection]);
    
    if (machine->balance > 0) {
        printf("Returning change: %d cents\n", machine->balance);
        machine->balance = 0;
    }
    
    machine->selection = -1;
    machine->state = IDLE;
    return 0;
}

int handle_error(struct vending_machine *machine) {
    printf("Current state: ERROR\n");
    printf("System error occurred. Resetting...\n");
    init_machine(machine);
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
            default:
                machine.state = ERROR;
        }
        
        printf("---\n");
    }
    
    printf("Goodbye!\n");
    return 0;
}
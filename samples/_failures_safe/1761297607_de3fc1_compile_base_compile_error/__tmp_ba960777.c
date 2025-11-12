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
    printf("Options: 1-Insert coin, 2-Select item, 0-Exit\n");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return 0;
    }
    
    switch (choice) {
        case 0:
            return -1;
        case 1:
            machine->balance += 25;
            printf("Coin inserted. Balance: %d\n", machine->balance);
            return 0;
        case 2:
            machine->state = SELECTING;
            return 0;
        default:
            printf("Invalid choice\n");
            return 0;
    }
}

int handle_selecting(struct vending_machine *machine) {
    printf("Current state: SELECTING\n");
    printf("Items: 0-Soda(25), 1-Chips(50), 2-Candy(35), 3-Water(20), 4-Cookie(40)\n");
    printf("Balance: %d. Select item (0-4) or 5-Cancel: ", machine->balance);
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        printf("Invalid input\n");
        return 0;
    }
    
    if (choice == 5) {
        machine->state = IDLE;
        return 0;
    }
    
    if (choice < 0 || choice > 4) {
        printf("Invalid selection\n");
        return 0;
    }
    
    machine->selection = choice;
    machine->state = PROCESSING;
    return 0;
}

int handle_processing(struct vending_machine *machine) {
    printf("Current state: PROCESSING\n");
    
    int prices[] = {25, 50, 35, 20, 40};
    int price = prices[machine->selection];
    
    if (machine->inventory[machine->selection] <= 0) {
        printf("Item out of stock\n");
        machine->state = ERROR;
        return 0;
    }
    
    if (machine->balance < price) {
        printf("Insufficient balance. Need %d, have %d\n", price, machine->balance);
        machine->state = ERROR;
        return 0;
    }
    
    machine->balance -= price;
    machine->inventory[machine->selection]--;
    machine->state = DISPENSING;
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    printf("Current state: DISPENSING\n");
    
    char *items[] = {"Soda", "Chips", "Candy", "Water", "Cookie"};
    printf("Dispensing: %s\n", items[machine->selection]);
    printf("Remaining balance: %d\n", machine->balance);
    
    if (machine->balance > 0) {
        printf("Returning change: %d\n", machine->balance);
        machine->balance = 0;
    }
    
    machine->selection = -1;
    machine->state = IDLE;
    return 0;
}

int handle_error(struct vending_machine *machine) {
    printf("Current state: ERROR\n");
    printf("Options: 1-Return to idle, 2-Return coins\n");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return 0;
    }
    
    switch (choice) {
        case 1:
            machine->state = IDLE;
            break;
        case 2:
            if (machine->balance > 0) {
                printf("Returning coins: %d\n", machine->balance);
                machine->balance = 0;
            }
            machine->state = IDLE;
            break;
        default:
            printf("Invalid choice\n");
            break;
    }
    
    machine->selection = -1;
    return 0;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    
    printf("Vending Machine Simulator\n");
    
    while (1) {
        int result = 0;
        
        switch (machine.state) {
            case IDLE:
                result = handle_idle(&machine);
                break;
            case SELECTING:
                result = handle_selecting(&machine);
                break;
            case PROCESSING:
                result = handle_processing(&machine);
                break;
            case DISPENSING:
                result = handle_dispensing(&machine);
                break;
            case ERROR:
                result = handle_error(&machine);
                break;
        }
        
        if (result == -1) {
            break;
        }
        
        printf("---\n");
    }
    
    printf("Good
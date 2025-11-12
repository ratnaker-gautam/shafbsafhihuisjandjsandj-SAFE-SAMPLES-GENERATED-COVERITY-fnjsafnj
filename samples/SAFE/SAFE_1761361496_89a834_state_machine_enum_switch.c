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
    printf("Options: 1-Select item, 0-Exit\n");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        return -1;
    }
    
    if (choice == 0) {
        return 0;
    } else if (choice == 1) {
        machine->state = SELECTING;
        return 1;
    } else {
        printf("Invalid choice\n");
        return 1;
    }
}

int handle_selecting(struct vending_machine *machine) {
    printf("Current state: SELECTING\n");
    printf("Items: 0-Soda($1), 1-Chips($2), 2-Candy($1), 3-Water($1), 4-Coffee($3)\n");
    printf("Select item (0-4): ");
    
    if (scanf("%d", &machine->selection) != 1) {
        return -1;
    }
    
    if (machine->selection < 0 || machine->selection > 4) {
        printf("Invalid selection\n");
        return 1;
    }
    
    if (machine->inventory[machine->selection] <= 0) {
        printf("Item out of stock\n");
        machine->state = IDLE;
        return 1;
    }
    
    machine->state = PROCESSING;
    return 1;
}

int handle_processing(struct vending_machine *machine) {
    printf("Current state: PROCESSING\n");
    
    int prices[] = {1, 2, 1, 1, 3};
    int price = prices[machine->selection];
    
    printf("Item price: $%d\n", price);
    printf("Insert money (current balance: $%d): ", machine->balance);
    
    int money;
    if (scanf("%d", &money) != 1) {
        return -1;
    }
    
    if (money < 0) {
        printf("Invalid amount\n");
        return 1;
    }
    
    if (machine->balance + money < 0) {
        printf("Balance overflow\n");
        machine->state = ERROR;
        return 1;
    }
    
    machine->balance += money;
    
    if (machine->balance >= price) {
        machine->state = DISPENSING;
    } else {
        printf("Insufficient funds. Need $%d more\n", price - machine->balance);
        printf("Options: 1-Add more money, 2-Cancel\n");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            return -1;
        }
        
        if (choice == 2) {
            machine->state = IDLE;
            machine->balance = 0;
        }
    }
    
    return 1;
}

int handle_dispensing(struct vending_machine *machine) {
    printf("Current state: DISPENSING\n");
    
    int prices[] = {1, 2, 1, 1, 3};
    int price = prices[machine->selection];
    
    if (machine->inventory[machine->selection] <= 0) {
        printf("Item out of stock\n");
        machine->state = ERROR;
        return 1;
    }
    
    machine->inventory[machine->selection]--;
    machine->balance -= price;
    
    printf("Item dispensed! Change: $%d\n", machine->balance);
    
    machine->balance = 0;
    machine->state = IDLE;
    return 1;
}

int handle_error(struct vending_machine *machine) {
    printf("Current state: ERROR\n");
    printf("An error occurred. Resetting...\n");
    
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    return 1;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    
    int result = 1;
    
    while (result > 0) {
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
            default:
                printf("Invalid state\n");
                result = -1;
                break;
        }
        
        if (result == -1) {
            printf("Input error\n");
            break;
        }
        
        printf("\n");
    }
    
    printf("Goodbye!\n");
    return 0;
}
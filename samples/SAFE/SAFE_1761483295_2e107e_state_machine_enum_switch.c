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

void initialize_machine(struct vending_machine *machine) {
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    machine->item_count = 5;
    machine->max_items = 10;
}

int handle_idle(struct vending_machine *machine) {
    printf("Current balance: %d\n", machine->balance);
    printf("Enter selection (1-3) or 0 to add coins: ");
    
    int input;
    if (scanf("%d", &input) != 1) {
        return -1;
    }
    
    if (input == 0) {
        printf("Enter coin amount (1-100): ");
        int coin;
        if (scanf("%d", &coin) != 1) {
            return -1;
        }
        if (coin < 1 || coin > 100) {
            printf("Invalid coin amount\n");
            return 0;
        }
        machine->balance += coin;
        printf("Added %d coins. New balance: %d\n", coin, machine->balance);
        return 0;
    }
    
    if (input < 1 || input > 3) {
        printf("Invalid selection\n");
        return 0;
    }
    
    machine->selection = input;
    machine->state = SELECTING;
    return 0;
}

int handle_selecting(struct vending_machine *machine) {
    int prices[] = {50, 75, 100};
    int selected_price = prices[machine->selection - 1];
    
    printf("Selected item %d, price: %d, your balance: %d\n", 
           machine->selection, selected_price, machine->balance);
    
    if (machine->balance < selected_price) {
        printf("Insufficient funds\n");
        machine->state = IDLE;
        return 0;
    }
    
    if (machine->item_count <= 0) {
        printf("Out of stock\n");
        machine->state = IDLE;
        return 0;
    }
    
    machine->state = PROCESSING;
    return 0;
}

int handle_processing(struct vending_machine *machine) {
    int prices[] = {50, 75, 100};
    int selected_price = prices[machine->selection - 1];
    
    machine->balance -= selected_price;
    machine->item_count--;
    
    printf("Processing item %d...\n", machine->selection);
    machine->state = DISPENSING;
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    printf("Dispensing item %d\n", machine->selection);
    printf("Remaining balance: %d\n", machine->balance);
    printf("Items remaining: %d\n", machine->item_count);
    
    machine->selection = -1;
    machine->state = IDLE;
    return 0;
}

int handle_error(struct vending_machine *machine) {
    printf("Error occurred. Resetting machine.\n");
    machine->selection = -1;
    machine->state = IDLE;
    return 0;
}

int process_state(struct vending_machine *machine) {
    switch (machine->state) {
        case IDLE:
            return handle_idle(machine);
        case SELECTING:
            return handle_selecting(machine);
        case PROCESSING:
            return handle_processing(machine);
        case DISPENSING:
            return handle_dispensing(machine);
        case ERROR:
            return handle_error(machine);
        default:
            machine->state = ERROR;
            return 0;
    }
}

int main(void) {
    struct vending_machine machine;
    initialize_machine(&machine);
    
    printf("Vending Machine Simulator\n");
    printf("Items: 1($50), 2($75), 3($100)\n");
    printf("Enter 0 to add coins, 4 to exit\n");
    
    while (1) {
        int result = process_state(&machine);
        if (result == -1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            machine.state = IDLE;
            continue;
        }
        
        if (machine.state == IDLE) {
            printf("Enter selection (1-3) or 0 to add coins, 4 to exit: ");
            int input;
            if (scanf("%d", &input) != 1) {
                while (getchar() != '\n');
                continue;
            }
            if (input == 4) {
                break;
            }
        }
    }
    
    printf("Final balance: %d\n", machine.balance);
    printf("Thank you for using the vending machine!\n");
    
    return 0;
}
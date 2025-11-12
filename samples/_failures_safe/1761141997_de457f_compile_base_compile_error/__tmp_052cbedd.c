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
    RETURNING_CHANGE,
    ERROR
};

struct vending_machine {
    enum vending_state state;
    int selection;
    int balance;
    int item_prices[5];
    int item_stock[5];
};

void init_machine(struct vending_machine *machine) {
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    int prices[] = {50, 75, 100, 125, 150};
    int stock[] = {5, 5, 5, 5, 5};
    for (int i = 0; i < 5; i++) {
        machine->item_prices[i] = prices[i];
        machine->item_stock[i] = stock[i];
    }
}

int handle_idle(struct vending_machine *machine, int input) {
    if (input >= 1 && input <= 5) {
        machine->selection = input - 1;
        machine->state = SELECTING;
        return 1;
    }
    return 0;
}

int handle_selecting(struct vending_machine *machine, int input) {
    if (input >= 10 && input <= 100 && input % 10 == 0) {
        if (machine->balance + input <= 1000) {
            machine->balance += input;
            return 1;
        }
    }
    if (input == 0) {
        machine->state = PROCESSING;
        return 1;
    }
    return 0;
}

int handle_processing(struct vending_machine *machine) {
    if (machine->selection < 0 || machine->selection >= 5) {
        machine->state = ERROR;
        return 0;
    }
    if (machine->item_stock[machine->selection] <= 0) {
        machine->state = ERROR;
        return 0;
    }
    if (machine->balance < machine->item_prices[machine->selection]) {
        machine->state = ERROR;
        return 0;
    }
    machine->balance -= machine->item_prices[machine->selection];
    machine->item_stock[machine->selection]--;
    machine->state = DISPENSING;
    return 1;
}

int handle_dispensing(struct vending_machine *machine) {
    printf("Dispensing item %d\n", machine->selection + 1);
    if (machine->balance > 0) {
        machine->state = RETURNING_CHANGE;
    } else {
        machine->state = IDLE;
        machine->selection = -1;
    }
    return 1;
}

int handle_returning_change(struct vending_machine *machine) {
    printf("Returning %d cents\n", machine->balance);
    machine->balance = 0;
    machine->state = IDLE;
    machine->selection = -1;
    return 1;
}

int handle_error(struct vending_machine *machine) {
    printf("Error occurred\n");
    if (machine->balance > 0) {
        machine->state = RETURNING_CHANGE;
    } else {
        machine->state = IDLE;
        machine->selection = -1;
    }
    return 1;
}

void print_state(struct vending_machine *machine) {
    const char *state_names[] = {
        "IDLE", "SELECTING", "PROCESSING", "DISPENSING", "RETURNING_CHANGE", "ERROR"
    };
    printf("Current state: %s\n", state_names[machine->state]);
    printf("Balance: %d\n", machine->balance);
    if (machine->selection >= 0) {
        printf("Selection: %d\n", machine->selection + 1);
    }
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    
    printf("Vending Machine Simulator\n");
    printf("Items: 1($0.50) 2($0.75) 3($1.00) 4($1.25) 5($1.50)\n");
    printf("Input: 1-5=select, 10-100=insert money, 0=confirm, -1=quit\n");
    
    int running = 1;
    while (running) {
        print_state(&machine);
        printf("Enter input: ");
        
        int input;
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (input == -1) {
            running = 0;
            continue;
        }
        
        int valid = 0;
        switch (machine.state) {
            case IDLE:
                valid = handle_idle(&machine, input);
                break;
            case SELECTING:
                valid = handle_selecting(&machine, input);
                break;
            case PROCESSING:
                valid = handle_processing(&machine);
                break;
            case DISPENSING:
                valid = handle_dispensing(&machine);
                break;
            case RETURNING_CHANGE:
                valid = handle_returning_change(&machine);
                break;
            case ERROR:
                valid = handle_error(&machine);
                break;
        }
        
        if (!valid && machine.state != PROCESSING && machine.state != DISPENSING && 
            machine.state != RETURNING_CHANGE && machine.state != ERROR)
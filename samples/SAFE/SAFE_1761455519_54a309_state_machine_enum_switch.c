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

int main(void) {
    struct vending_machine machine;
    machine.state = IDLE;
    machine.selection = 0;
    machine.balance = 0;
    machine.item_count = 5;
    machine.max_items = 10;
    
    char input[32];
    int running = 1;
    
    while (running) {
        switch (machine.state) {
            case IDLE:
                printf("Vending Machine - Current balance: %d\n", machine.balance);
                printf("Commands: insert <amount>, select <item 1-%d>, quit\n", machine.item_count);
                
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    machine.state = ERROR;
                    break;
                }
                
                input[strcspn(input, "\n")] = '\0';
                
                if (strncmp(input, "insert ", 7) == 0) {
                    char *amount_str = input + 7;
                    if (strlen(amount_str) > 0 && strlen(amount_str) < 10) {
                        char *endptr;
                        long amount = strtol(amount_str, &endptr, 10);
                        if (*endptr == '\0' && amount > 0 && amount <= 1000) {
                            if (machine.balance <= 1000 - amount) {
                                machine.balance += (int)amount;
                                printf("Inserted %d. New balance: %d\n", (int)amount, machine.balance);
                            } else {
                                printf("Balance too high\n");
                            }
                        } else {
                            printf("Invalid amount\n");
                        }
                    } else {
                        printf("Invalid input\n");
                    }
                } else if (strncmp(input, "select ", 7) == 0) {
                    char *item_str = input + 7;
                    if (strlen(item_str) > 0 && strlen(item_str) < 10) {
                        char *endptr;
                        long selection = strtol(item_str, &endptr, 10);
                        if (*endptr == '\0' && selection >= 1 && selection <= machine.item_count) {
                            machine.selection = (int)selection;
                            machine.state = SELECTING;
                        } else {
                            printf("Invalid selection\n");
                        }
                    } else {
                        printf("Invalid input\n");
                    }
                } else if (strcmp(input, "quit") == 0) {
                    running = 0;
                } else {
                    printf("Unknown command\n");
                }
                break;
                
            case SELECTING:
                printf("Selected item %d. Cost: 50. Current balance: %d\n", machine.selection, machine.balance);
                printf("Confirm purchase? (yes/no): ");
                
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    machine.state = ERROR;
                    break;
                }
                
                input[strcspn(input, "\n")] = '\0';
                
                if (strcmp(input, "yes") == 0) {
                    machine.state = PROCESSING;
                } else if (strcmp(input, "no") == 0) {
                    machine.state = IDLE;
                } else {
                    printf("Invalid response\n");
                }
                break;
                
            case PROCESSING:
                if (machine.balance >= 50) {
                    machine.balance -= 50;
                    printf("Processing purchase...\n");
                    machine.state = DISPENSING;
                } else {
                    printf("Insufficient funds. Need 50, have %d\n", machine.balance);
                    machine.state = IDLE;
                }
                break;
                
            case DISPENSING:
                printf("Dispensing item %d...\n", machine.selection);
                printf("Enjoy your purchase! Remaining balance: %d\n", machine.balance);
                machine.state = IDLE;
                break;
                
            case ERROR:
                printf("An error occurred. Resetting...\n");
                machine.state = IDLE;
                machine.balance = 0;
                machine.selection = 0;
                break;
        }
    }
    
    printf("Thank you for using the vending machine!\n");
    return 0;
}
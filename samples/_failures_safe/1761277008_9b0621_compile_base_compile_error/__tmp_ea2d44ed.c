//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    int level;
    char message[MAX_LINE_LENGTH];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARNING") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "CRITICAL") == 0) return 4;
    return -1;
}

const char* level_to_string(int level) {
    switch (level) {
        case 0: return "DEBUG";
        case 1: return "INFO";
        case 2: return "WARNING";
        case 3: return "ERROR";
        case 4: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

int count_entries_by_level(struct LogEntry* entries, int count, int target_level, int index) {
    if (index >= count) return 0;
    int current = (entries[index].level == target_level) ? 1 : 0;
    return current + count_entries_by_level(entries, count, target_level, index + 1);
}

void print_entries_by_level(struct LogEntry* entries, int count, int target_level, int index) {
    if (index >= count) return;
    if (entries[index].level == target_level) {
        printf("[%s] %s\n", level_to_string(entries[index].level), entries[index].message);
    }
    print_entries_by_level(entries, count, target_level, index + 1);
}

int read_log_file(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (format: LEVEL MESSAGE). Enter 'END' to finish:\n");
    
    while (count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        if (strlen(line) == 0) continue;
        
        char level_str[32];
        char message[MAX_LINE_LENGTH];
        
        if (sscanf(line, "%31s %1023[^\n]", level_str, message) != 2) {
            printf("Invalid format. Use: LEVEL MESSAGE\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Unknown log level: %s\n", level_str);
            continue;
        }
        
        if (strlen(message) >= MAX_LINE_LENGTH) {
            printf("Message too long\n");
            continue;
        }
        
        entries[count].level = level;
        strncpy(entries[count].message, message, MAX_LINE_LENGTH - 1);
        entries[count].message[MAX_LINE_LENGTH - 1] = '\0';
        count++;
    }
    
    return count;
}

void display_statistics(struct LogEntry* entries, int count, int current_level) {
    if (current_level > 4) return;
    
    int level_count = count_entries_by_level(entries, count, current_level, 0);
    if (level_count > 0) {
        printf("%s: %d entries\n", level_to_string(current_level), level_count);
    }
    
    display_statistics(entries, count, current_level + 1);
}

void show_menu() {
    printf("\nLog Analyzer Menu:\n");
    printf("1. Show statistics\n");
    printf("2. Show entries by level\n");
    printf("3. Exit\n");
    printf("Choose option: ");
}

int main() {
    struct LogEntry entries[1000];
    int entry_count = 0;
    
    entry_count = read_log_file(entries, 1000);
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nSuccessfully loaded %d log entries.\n", entry_count);
    
    int running = 1;
    while (running) {
        show_menu();
        
        char input[32];
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        int choice;
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        switch (choice) {
            case 1: {
                printf("\nLog Statistics:\n");
                display_statistics(entries, entry_count, 0);
                break;
            }
            case 2: {
                printf("Enter level to show (DEBUG/INFO/WARNING/ERROR/CRITICAL): ");
                char level_input[32];
                if (fgets(level_input, sizeof(level_input), stdin) == NULL) break;
                level_input[strcspn(level_input, "\n")] = 0;
                
                int target_level = parse_log_level(level_input);
                if (target_level == -1) {
                    printf
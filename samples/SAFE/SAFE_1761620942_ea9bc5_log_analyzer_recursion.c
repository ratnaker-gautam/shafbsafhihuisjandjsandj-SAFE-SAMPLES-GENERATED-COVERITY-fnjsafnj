//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char line[MAX_LINE_LENGTH];
    int level;
    struct LogEntry* next;
};

int parse_log_level(const char* line) {
    if (strstr(line, "ERROR")) return 3;
    if (strstr(line, "WARN")) return 2;
    if (strstr(line, "INFO")) return 1;
    if (strstr(line, "DEBUG")) return 0;
    return -1;
}

struct LogEntry* read_log_file(const char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;
    
    struct LogEntry* head = NULL;
    struct LogEntry* tail = NULL;
    char buffer[MAX_LINE_LENGTH];
    *count = 0;
    
    while (fgets(buffer, sizeof(buffer), file)) {
        if (*count >= MAX_LEVELS) break;
        
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) == 0) continue;
        
        struct LogEntry* entry = malloc(sizeof(struct LogEntry));
        if (!entry) break;
        
        strncpy(entry->line, buffer, sizeof(entry->line) - 1);
        entry->line[sizeof(entry->line) - 1] = 0;
        entry->level = parse_log_level(buffer);
        entry->next = NULL;
        
        if (!head) {
            head = entry;
        } else {
            tail->next = entry;
        }
        tail = entry;
        (*count)++;
    }
    
    fclose(file);
    return head;
}

void print_logs_by_level_recursive(struct LogEntry* head, int target_level, int* printed) {
    if (!head) return;
    
    if (head->level == target_level) {
        printf("%s\n", head->line);
        (*printed)++;
    }
    
    print_logs_by_level_recursive(head->next, target_level, printed);
}

void free_log_entries(struct LogEntry* head) {
    if (!head) return;
    free_log_entries(head->next);
    free(head);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    int count;
    struct LogEntry* logs = read_log_file(argv[1], &count);
    if (!logs) {
        printf("Error: Could not read log file '%s'\n", argv[1]);
        return 1;
    }
    
    printf("Found %d log entries\n", count);
    printf("\nERROR level logs:\n");
    
    int printed = 0;
    print_logs_by_level_recursive(logs, 3, &printed);
    
    if (printed == 0) {
        printf("No ERROR level logs found\n");
    } else {
        printf("Total ERROR logs: %d\n", printed);
    }
    
    free_log_entries(logs);
    return 0;
}
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
    if (strstr(line, "[ERROR]")) return 3;
    if (strstr(line, "[WARN]")) return 2;
    if (strstr(line, "[INFO]")) return 1;
    if (strstr(line, "[DEBUG]")) return 0;
    return -1;
}

struct LogEntry* create_entry(const char* line) {
    struct LogEntry* entry = malloc(sizeof(struct LogEntry));
    if (!entry) return NULL;
    
    strncpy(entry->line, line, MAX_LINE_LENGTH - 1);
    entry->line[MAX_LINE_LENGTH - 1] = '\0';
    entry->level = parse_log_level(line);
    entry->next = NULL;
    
    return entry;
}

void analyze_log_recursive(struct LogEntry* current, int* level_counts, int depth) {
    if (!current || depth >= MAX_LEVELS) return;
    
    if (current->level >= 0 && current->level < MAX_LEVELS) {
        level_counts[current->level]++;
    }
    
    analyze_log_recursive(current->next, level_counts, depth + 1);
}

void free_log_entries(struct LogEntry* head) {
    if (!head) return;
    free_log_entries(head->next);
    free(head);
}

int main() {
    struct LogEntry* head = NULL;
    struct LogEntry* tail = NULL;
    char buffer[MAX_LINE_LENGTH];
    int level_counts[MAX_LEVELS] = {0};
    const char* level_names[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    
    printf("Enter log lines (empty line to finish):\n");
    
    while (1) {
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strlen(buffer) == 0) break;
        
        struct LogEntry* new_entry = create_entry(buffer);
        if (!new_entry) {
            fprintf(stderr, "Memory allocation failed\n");
            free_log_entries(head);
            return 1;
        }
        
        if (!head) {
            head = new_entry;
            tail = new_entry;
        } else {
            tail->next = new_entry;
            tail = new_entry;
        }
    }
    
    if (!head) {
        printf("No log entries provided.\n");
        return 0;
    }
    
    analyze_log_recursive(head, level_counts, 0);
    
    printf("\nLog Level Analysis:\n");
    printf("==================\n");
    for (int i = 0; i < 4; i++) {
        printf("%s: %d entries\n", level_names[i], level_counts[i]);
    }
    
    int total_entries = 0;
    for (int i = 0; i < 4; i++) {
        total_entries += level_counts[i];
    }
    printf("Total: %d entries\n", total_entries);
    
    free_log_entries(head);
    return 0;
}
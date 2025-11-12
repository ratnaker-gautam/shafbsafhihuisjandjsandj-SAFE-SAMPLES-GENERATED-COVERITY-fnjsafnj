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
    struct LogEntry *next;
};

int parse_log_level(const char *line) {
    if (strstr(line, "[ERROR]")) return 3;
    if (strstr(line, "[WARN]")) return 2;
    if (strstr(line, "[INFO]")) return 1;
    if (strstr(line, "[DEBUG]")) return 0;
    return -1;
}

void print_log_tree(struct LogEntry *head, int current_level, int max_level) {
    if (head == NULL || current_level > max_level) return;
    
    if (head->level == current_level) {
        printf("%*s%s", current_level * 2, "", head->line);
        print_log_tree(head->next, current_level, max_level);
    } else if (head->level > current_level) {
        print_log_tree(head, current_level + 1, max_level);
    } else {
        print_log_tree(head->next, current_level, max_level);
    }
}

struct LogEntry* read_log_file(const char *filename, int *line_count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return NULL;
    
    struct LogEntry *head = NULL;
    struct LogEntry *tail = NULL;
    char buffer[MAX_LINE_LENGTH];
    *line_count = 0;
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (*line_count >= MAX_LEVELS) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
        
        struct LogEntry *entry = malloc(sizeof(struct LogEntry));
        if (entry == NULL) break;
        
        strncpy(entry->line, buffer, sizeof(entry->line) - 1);
        entry->line[sizeof(entry->line) - 1] = '\0';
        entry->level = parse_log_level(buffer);
        entry->next = NULL;
        
        if (head == NULL) {
            head = entry;
            tail = entry;
        } else {
            tail->next = entry;
            tail = entry;
        }
        
        (*line_count)++;
    }
    
    fclose(file);
    return head;
}

void free_log_entries(struct LogEntry *head) {
    if (head == NULL) return;
    free_log_entries(head->next);
    free(head);
}

int count_logs_by_level(struct LogEntry *head, int target_level) {
    if (head == NULL) return 0;
    int count = (head->level == target_level) ? 1 : 0;
    return count + count_logs_by_level(head->next, target_level);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    int line_count;
    struct LogEntry *log_entries = read_log_file(argv[1], &line_count);
    
    if (log_entries == NULL) {
        printf("Error: Could not read file '%s'\n", argv[1]);
        return 1;
    }
    
    printf("Log entries organized by level:\n");
    printf("===============================\n");
    print_log_tree(log_entries, 0, 3);
    
    printf("\nLog level statistics:\n");
    printf("=====================\n");
    printf("DEBUG: %d entries\n", count_logs_by_level(log_entries, 0));
    printf("INFO:  %d entries\n", count_logs_by_level(log_entries, 1));
    printf("WARN:  %d entries\n", count_logs_by_level(log_entries, 2));
    printf("ERROR: %d entries\n", count_logs_by_level(log_entries, 3));
    
    free_log_entries(log_entries);
    return 0;
}
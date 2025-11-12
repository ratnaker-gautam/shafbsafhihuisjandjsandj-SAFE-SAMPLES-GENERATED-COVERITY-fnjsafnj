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

void analyze_log_recursive(struct LogEntry* entries, int count, int current_level, int* level_counts) {
    if (current_level >= MAX_LEVELS || current_level < 0) return;
    if (count <= 0) return;
    
    struct LogEntry* current = entries;
    int processed = 0;
    
    while (current != NULL && processed < count) {
        if (current->level == current_level) {
            level_counts[current_level]++;
        }
        current = current->next;
        processed++;
    }
    
    analyze_log_recursive(entries, count, current_level + 1, level_counts);
}

struct LogEntry* read_log_file(const char* filename, int* total_lines) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;
    
    struct LogEntry* head = NULL;
    struct LogEntry* tail = NULL;
    char buffer[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (line_count >= MAX_LEVELS) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        if (len >= MAX_LINE_LENGTH - 1) {
            buffer[MAX_LINE_LENGTH - 1] = '\0';
        }
        
        struct LogEntry* new_entry = malloc(sizeof(struct LogEntry));
        if (!new_entry) break;
        
        strncpy(new_entry->line, buffer, MAX_LINE_LENGTH - 1);
        new_entry->line[MAX_LINE_LENGTH - 1] = '\0';
        new_entry->level = parse_log_level(buffer);
        new_entry->next = NULL;
        
        if (!head) {
            head = new_entry;
            tail = new_entry;
        } else {
            tail->next = new_entry;
            tail = new_entry;
        }
        
        line_count++;
    }
    
    fclose(file);
    *total_lines = line_count;
    return head;
}

void free_log_entries(struct LogEntry* head) {
    while (head != NULL) {
        struct LogEntry* next = head->next;
        free(head);
        head = next;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    int total_lines = 0;
    struct LogEntry* entries = read_log_file(argv[1], &total_lines);
    
    if (!entries) {
        printf("Error: Could not read log file '%s'\n", argv[1]);
        return 1;
    }
    
    if (total_lines == 0) {
        printf("No log entries found.\n");
        free_log_entries(entries);
        return 0;
    }
    
    int level_counts[MAX_LEVELS] = {0};
    analyze_log_recursive(entries, total_lines, 0, level_counts);
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", total_lines);
    printf("DEBUG level: %d\n", level_counts[0]);
    printf("INFO level: %d\n", level_counts[1]);
    printf("WARN level: %d\n", level_counts[2]);
    printf("ERROR level: %d\n", level_counts[3]);
    
    int unknown_count = total_lines;
    for (int i = 0; i < 4; i++) {
        unknown_count -= level_counts[i];
    }
    printf("Unknown level: %d\n", unknown_count);
    
    free_log_entries(entries);
    return 0;
}
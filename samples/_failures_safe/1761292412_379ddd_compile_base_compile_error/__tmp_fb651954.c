//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH];
    int indent_level;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    int indent = 0;
    while (temp[indent] == ' ' || temp[indent] == '\t') {
        indent++;
    }
    entry->indent_level = indent / 2;
    
    char* timestamp_start = temp + indent;
    char* space1 = strchr(timestamp_start, ' ');
    if (!space1) return 0;
    
    strncpy(entry->timestamp, timestamp_start, space1 - timestamp_start);
    entry->timestamp[space1 - timestamp_start] = '\0';
    
    char* level_start = space1 + 1;
    char* space2 = strchr(level_start, ' ');
    if (!space2) return 0;
    
    strncpy(entry->level, level_start, space2 - level_start);
    entry->level[space2 - level_start] = '\0';
    
    char* message_start = space2 + 1;
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void print_log_hierarchy(struct LogEntry* entries, int count, int current, int parent_level) {
    if (current >= count) return;
    
    struct LogEntry* entry = &entries[current];
    
    if (entry->indent_level <= parent_level) return;
    
    for (int i = 0; i < entry->indent_level; i++) {
        printf("  ");
    }
    
    printf("[%s] %s: %s\n", entry->timestamp, entry->level, entry->message);
    
    print_log_hierarchy(entries, count, current + 1, entry->indent_level);
    
    if (current + 1 < count && entries[current + 1].indent_level > parent_level) {
        print_log_hierarchy(entries, count, current + 1, parent_level);
    }
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    if (!filename || !entries || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[count] = entry;
            count++;
        }
    }
    
    fclose(file);
    return count;
}

int count_errors_recursive(struct LogEntry* entries, int count, int current, int parent_level) {
    if (current >= count) return 0;
    
    struct LogEntry* entry = &entries[current];
    
    if (entry->indent_level <= parent_level) return 0;
    
    int error_count = 0;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        error_count = 1;
    }
    
    error_count += count_errors_recursive(entries, count, current + 1, entry->indent_level);
    
    if (current + 1 < count && entries[current + 1].indent_level > parent_level) {
        error_count += count_errors_recursive(entries, count, current + 1, parent_level);
    }
    
    return error_count;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    struct LogEntry entries[1000];
    int entry_count = read_log_file(argv[1], entries, 1000);
    
    if (entry_count == 0) {
        printf("No valid log entries found or file cannot be read.\n");
        return 1;
    }
    
    printf("Log Hierarchy:\n");
    printf("==============\n");
    
    int current = 0;
    while (current < entry_count) {
        if (entries[current].indent_level == 0) {
            print_log_hierarchy(entries, entry_count, current, -1);
        }
        current++;
    }
    
    printf("\nError Statistics:\n");
    printf("================\n");
    
    int total_errors = 0;
    current = 0;
    while (current < entry_count) {
        if (entries[current].indent_level == 0) {
            int subtree_errors = count_errors_recursive(entries, entry_count, current, -1);
            printf("Subtree starting with '%s': %d errors\n", entries[current].message, subtree_errors);
            total_
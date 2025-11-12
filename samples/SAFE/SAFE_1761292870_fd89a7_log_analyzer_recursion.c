//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char level_buf[16];
    int timestamp;
    char message_buf[MAX_LINE_LENGTH];
    
    if (sscanf(line, "%d %15s %1023[^\n]", &timestamp, level_buf, message_buf) != 3) {
        return 0;
    }
    
    if (timestamp < 0 || timestamp > 999999999) return 0;
    
    entry->timestamp = timestamp;
    strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int depth) {
    if (depth >= MAX_LEVELS) return;
    if (start > end) return;
    if (!entries) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = start; i <= end; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("%*sTime range: %d - %d\n", depth * 2, "", entries[start].timestamp, entries[end].timestamp);
    printf("%*sErrors: %d, Warnings: %d, Info: %d\n", depth * 2, "", error_count, warning_count, info_count);
    
    if (end - start > 1) {
        int mid = start + (end - start) / 2;
        analyze_log_recursive(entries, start, mid, depth + 1);
        analyze_log_recursive(entries, mid + 1, end, depth + 1);
    }
}

int compare_entries(const void* a, const void* b) {
    const struct LogEntry* entry_a = (const struct LogEntry*)a;
    const struct LogEntry* entry_b = (const struct LogEntry*)b;
    
    if (entry_a->timestamp < entry_b->timestamp) return -1;
    if (entry_a->timestamp > entry_b->timestamp) return 1;
    return 0;
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: timestamp LEVEL message):\n");
    printf("Example: 123456789 INFO System started\n");
    printf("Enter 'END' to finish input\n");
    
    while (entry_count < 100) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry new_entry;
        if (parse_log_line(line, &new_entry)) {
            entries[entry_count] = new_entry;
            entry_count++;
        } else {
            printf("Invalid log format. Skipping.\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    qsort(entries, entry_count, sizeof(struct LogEntry), compare_entries);
    
    printf("\nLog Analysis Results:\n");
    analyze_log_recursive(entries, 0, entry_count - 1, 0);
    
    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[64];
    char level[16];
    char message[256];
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int is_valid_timestamp(const char* timestamp) {
    if (timestamp == NULL) return 0;
    if (strlen(timestamp) != 19) return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->timestamp, entry_b->timestamp);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && entry_count < MAX_ENTRIES) {
        line_count++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_timestamp(entry.timestamp)) {
                entries[entry_count] = entry;
                total_errors += entry.error_count;
                entry_count++;
            }
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    qsort(entries, entry_count, sizeof(LogEntry), compare_entries);
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total error entries: %d\n", total_errors);
    printf("Error rate: %.2f%%\n", (total_errors * 100.0) / entry_count);
    
    printf("\nFirst 5 entries:\n");
    for (int i = 0; i < 5 && i < entry_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    printf("\nLast 5 entries:\n");
    for (int i = (entry_count > 5 ? entry_count - 5 : 0); i < entry_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}
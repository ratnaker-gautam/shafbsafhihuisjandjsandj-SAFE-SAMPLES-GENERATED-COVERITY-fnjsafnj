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
    
    if (timestamp < 0) return 0;
    
    strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->timestamp = timestamp;
    
    return 1;
}

void analyze_log_recursive(struct LogEntry* entries, int count, int index, int* error_count, int* warning_count, int* info_count) {
    if (index >= count || index < 0) return;
    
    if (strcmp(entries[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        (*info_count)++;
    }
    
    analyze_log_recursive(entries, count, index + 1, error_count, warning_count, info_count);
}

void print_log_summary(struct LogEntry* entries, int count, int index) {
    if (index >= count || index < 0) return;
    
    printf("Time: %d | Level: %-8s | Message: %s\n", 
           entries[index].timestamp, entries[index].level, entries[index].message);
    
    print_log_summary(entries, count, index + 1);
}

int read_log_file(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (format: timestamp LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (count < max_entries) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[count] = entry;
            count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    return count;
}

int main() {
    struct LogEntry entries[MAX_LEVELS];
    int entry_count;
    
    entry_count = read_log_file(entries, MAX_LEVELS);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    int error_count = 0, warning_count = 0, info_count = 0;
    
    analyze_log_recursive(entries, entry_count, 0, &error_count, &warning_count, &info_count);
    
    printf("\n=== LOG ANALYSIS SUMMARY ===\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\n=== LOG ENTRIES ===\n");
    print_log_summary(entries, entry_count, 0);
    
    return 0;
}
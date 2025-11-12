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
    char message_buf[MAX_LINE_LENGTH];
    int timestamp;
    
    int parsed = sscanf(line, "%d %15s %1023[^\n]", 
                       &timestamp, level_buf, message_buf);
    
    if (parsed != 3) return 0;
    
    if (timestamp < 0) return 0;
    
    if (strlen(level_buf) >= 16 || strlen(message_buf) >= MAX_LINE_LENGTH) return 0;
    
    strncpy(entry->level, level_buf, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message_buf, MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    entry->timestamp = timestamp;
    
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
    
    printf("Depth %d: Range [%d-%d] - Errors: %d, Warnings: %d, Info: %d\n",
           depth, start, end, error_count, warning_count, info_count);
    
    if (end - start <= 1) return;
    
    int mid = start + (end - start) / 2;
    
    analyze_log_recursive(entries, start, mid, depth + 1);
    analyze_log_recursive(entries, mid + 1, end, depth + 1);
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: timestamp LEVEL message):\n");
    printf("Example: 1234567890 INFO System started\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < 100) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    analyze_log_recursive(entries, 0, entry_count - 1, 0);
    
    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[64];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    if (timestamp == NULL) return 0;
    
    char* level = strtok(NULL, " ");
    if (level == NULL) return 0;
    
    char* message = strtok(NULL, "\n");
    if (message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) - 1) return 0;
    if (strlen(level) >= sizeof(entry->level) - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    if (level == NULL) return 0;
    return (strcmp(level, "INFO") == 0 || 
            strcmp(level, "WARN") == 0 || 
            strcmp(level, "ERROR") == 0);
}

void analyze_logs(struct LogEntry* logs, int count) {
    if (logs == NULL || count <= 0) return;
    
    int info_count = 0;
    int warn_count = 0;
    int error_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(logs[i].level, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    printf("Total entries: %d\n", count);
}

int read_log_file(struct LogEntry* logs, int max_entries) {
    if (logs == NULL || max_entries <= 0) return 0;
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry) && is_valid_level(entry.level)) {
            logs[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    return entry_count;
}

int main() {
    struct LogEntry logs[MAX_ENTRIES];
    
    printf("Log Analyzer\n");
    printf("============\n");
    
    int entry_count = read_log_file(logs, MAX_ENTRIES);
    
    if (entry_count > 0) {
        analyze_logs(logs, entry_count);
    } else {
        printf("No valid log entries found.\n");
    }
    
    return 0;
}
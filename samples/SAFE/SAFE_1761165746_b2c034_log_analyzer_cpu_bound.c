//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
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
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int validate_timestamp(const char* timestamp) {
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

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Processing simulated log data\n");
    printf("Enter log lines (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (validate_timestamp(entry.timestamp)) {
                entries[entry_count] = entry;
                total_errors += entry.error_count;
                entry_count++;
            }
        }
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total log entries processed: %d\n", entry_count);
    printf("Total ERROR entries: %d\n", total_errors);
    
    if (entry_count > 0) {
        int warning_count = 0;
        int info_count = 0;
        
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "WARNING") == 0) {
                warning_count++;
            } else if (strcmp(entries[i].level, "INFO") == 0) {
                info_count++;
            }
        }
        
        printf("INFO entries: %d\n", info_count);
        printf("WARNING entries: %d\n", warning_count);
        
        if (total_errors > 0) {
            printf("\nERROR messages found:\n");
            for (int i = 0; i < entry_count; i++) {
                if (entries[i].error_count) {
                    printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
                }
            }
        }
    }
    
    return 0;
}
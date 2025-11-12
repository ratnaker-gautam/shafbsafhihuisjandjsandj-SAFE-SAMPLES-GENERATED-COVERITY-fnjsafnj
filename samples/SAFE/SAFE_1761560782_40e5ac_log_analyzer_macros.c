//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 10
#define LOG_LEVEL_ERROR "ERROR"
#define LOG_LEVEL_WARN "WARN"
#define LOG_LEVEL_INFO "INFO"
#define IS_VALID_LEVEL(level) (strcmp(level, LOG_LEVEL_ERROR) == 0 || strcmp(level, LOG_LEVEL_WARN) == 0 || strcmp(level, LOG_LEVEL_INFO) == 0)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 3];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "");
    
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= TIMESTAMP_LEN - 1) return 0;
    if (strlen(level) >= LEVEL_LEN - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    if (!IS_VALID_LEVEL(level)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void analyze_logs(LogEntry entries[], int count) {
    int error_count = 0, warn_count = 0, info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, LOG_LEVEL_ERROR) == 0) error_count++;
        else if (strcmp(entries[i].level, LOG_LEVEL_WARN) == 0) warn_count++;
        else if (strcmp(entries[i].level, LOG_LEVEL_INFO) == 0) info_count++;
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warn_count);
    printf("INFO:  %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        for (int i = count - 1; i >= 0 && error_count > 0; i--) {
            if (strcmp(entries[i].level, LOG_LEVEL_ERROR) == 0) {
                printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
                error_count--;
                if (error_count <= 2) break;
            }
        }
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log lines (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: ERROR, WARN, INFO\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LEN];
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}
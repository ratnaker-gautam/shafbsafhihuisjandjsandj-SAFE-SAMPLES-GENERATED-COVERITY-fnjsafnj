//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Processing simulated log data...\n");
    
    for (int i = 0; i < 5000 && entry_count < MAX_ENTRIES; i++) {
        char timestamp[32];
        char level[16];
        char message[256];
        
        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);
        strftime(timestamp, 32, "%Y-%m-%d %H:%M:%S", tm_info);
        
        const char* levels[] = {"INFO", "WARNING", "ERROR"};
        int level_idx = rand() % 3;
        strcpy(level, levels[level_idx]);
        
        const char* messages[] = {
            "System started successfully",
            "User login detected",
            "Database connection established",
            "Memory usage at 75%",
            "Network latency increased",
            "Critical system failure",
            "Backup completed",
            "Security alert triggered"
        };
        int msg_idx = rand() % 8;
        strcpy(message, messages[msg_idx]);
        
        snprintf(line, MAX_LINE_LEN, "%s %s %s", timestamp, level, message);
        
        if (parse_log_line(line, &entries[entry_count])) {
            total_errors += entries[entry_count].error_count;
            entry_count++;
        }
    }
    
    printf("Processed %d log entries\n", entry_count);
    printf("Total errors: %d\n", total_errors);
    
    int error_percent = (entry_count > 0) ? (total_errors * 100) / entry_count : 0;
    printf("Error percentage: %d%%\n", error_percent);
    
    int info_count = 0, warning_count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
    }
    
    printf("INFO entries: %d\n", info_count);
    printf("WARNING entries: %d\n", warning_count);
    
    return 0;
}
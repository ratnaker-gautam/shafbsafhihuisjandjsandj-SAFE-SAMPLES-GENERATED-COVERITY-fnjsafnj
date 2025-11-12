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
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* token = strtok(temp_line, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, token);
    
    entry->error_count = 0;
    if (strcmp(entry->level, "ERROR") == 0) {
        entry->error_count = 1;
    }
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Processing log entries...\n");
    
    for (int i = 0; i < 5000; i++) {
        if (entry_count >= MAX_ENTRIES) break;
        
        const char* levels[] = {"INFO", "WARNING", "ERROR"};
        const char* messages[] = {
            "System started", "User login", "File accessed",
            "Database connection", "Memory allocation", "Network timeout",
            "Invalid input", "Permission denied", "Disk full"
        };
        
        int level_idx = rand() % 3;
        int msg_idx = rand() % 9;
        
        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);
        char timestamp[32];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
        
        snprintf(line, sizeof(line), "%s %s %s", timestamp, levels[level_idx], messages[msg_idx]);
        
        if (parse_log_line(line, &entries[entry_count])) {
            total_errors += entries[entry_count].error_count;
            entry_count++;
        }
    }
    
    printf("Processing complete.\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total errors found: %d\n", total_errors);
    
    if (entry_count > 0) {
        double error_rate = (double)total_errors / entry_count * 100.0;
        printf("Error rate: %.2f%%\n", error_rate);
        
        int info_count = 0, warning_count = 0;
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "INFO") == 0) info_count++;
            else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        }
        
        printf("INFO entries: %d\n", info_count);
        printf("WARNING entries: %d\n", warning_count);
    }
    
    return 0;
}
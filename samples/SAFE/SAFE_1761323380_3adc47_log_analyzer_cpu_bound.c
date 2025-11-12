//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    entry->error_count = 0;
    if (strcmp(level, "ERROR") == 0) {
        entry->error_count = 1;
    }
    
    return 1;
}

void generate_sample_logs(LogEntry* logs, int count) {
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login detected",
        "Database connection established",
        "Memory usage high",
        "Network timeout occurred",
        "File not found",
        "Permission denied",
        "Configuration updated"
    };
    
    time_t current_time = time(NULL);
    
    for (int i = 0; i < count; i++) {
        int level_idx = rand() % 3;
        int msg_idx = rand() % 8;
        time_t log_time = current_time - (rand() % 86400);
        
        struct tm* timeinfo = localtime(&log_time);
        strftime(logs[i].timestamp, sizeof(logs[i].timestamp), 
                "%Y-%m-%d %H:%M:%S", timeinfo);
        
        strcpy(logs[i].level, levels[level_idx]);
        strcpy(logs[i].message, messages[msg_idx]);
        
        logs[i].error_count = (strcmp(levels[level_idx], "ERROR") == 0) ? 1 : 0;
    }
}

void analyze_logs(LogEntry* logs, int count) {
    int total_errors = 0;
    int total_warnings = 0;
    int total_info = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            total_errors++;
        } else if (strcmp(logs[i].level, "WARNING") == 0) {
            total_warnings++;
        } else if (strcmp(logs[i].level, "INFO") == 0) {
            total_info++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", total_info);
    printf("WARNING entries: %d\n", total_warnings);
    printf("ERROR entries: %d\n", total_errors);
    printf("Error rate: %.2f%%\n", (total_errors * 100.0) / count);
    
    if (total_errors > 0) {
        printf("\nRecent ERROR entries:\n");
        int printed = 0;
        for (int i = count - 1; i >= 0 && printed < 5; i--) {
            if (strcmp(logs[i].level, "ERROR") == 0) {
                printf("- %s: %s\n", logs[i].timestamp, logs[i].message);
                printed++;
            }
        }
    }
}

int main() {
    srand((unsigned int)time(NULL));
    
    LogEntry logs[MAX_ENTRIES];
    int log_count = 5000 + (rand() % 5001);
    
    if (log_count > MAX_ENTRIES) {
        log_count = MAX_ENTRIES;
    }
    
    printf("Generating %d sample log entries...\n", log_count);
    generate_sample_logs(logs, log_count);
    
    printf("Analyzing logs...\n");
    analyze_logs(logs, log_count);
    
    int valid_parsed = 0;
    for (int i = 0; i < log_count; i++) {
        char line[MAX_LINE_LENGTH];
        snprintf(line, sizeof(line), "%s %s %s", 
                logs[i].timestamp, logs[i].level, logs[i].message);
        
        LogEntry parsed_entry;
        if (parse_log_line(line, &parsed_entry)) {
            valid_parsed++;
        }
    }
    
    printf("\nParsing validation: %d/%d entries successfully parsed\n", 
           valid_parsed, log_count);
    
    return 0;
}
//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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
        strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int is_valid_log_level(const char* level) {
    const char* valid_levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    for (int i = 0; i < 5; i++) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

void generate_sample_logs(LogEntry* logs, int count) {
    const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    const char* messages[] = {
        "User login successful",
        "Database connection established",
        "Memory usage high",
        "Failed to open file",
        "Network timeout",
        "Invalid input received",
        "Backup completed",
        "Configuration updated"
    };
    
    time_t current_time = time(NULL);
    
    for (int i = 0; i < count; i++) {
        struct tm* time_info = localtime(&current_time);
        strftime(logs[i].timestamp, sizeof(logs[i].timestamp), 
                "%Y-%m-%d %H:%M:%S", time_info);
        
        int level_idx = rand() % 5;
        strcpy(logs[i].level, levels[level_idx]);
        
        int msg_idx = rand() % 8;
        strcpy(logs[i].message, messages[msg_idx]);
        
        logs[i].error_count = (strcmp(levels[level_idx], "ERROR") == 0) ? 1 : 0;
        
        current_time += (rand() % 60) + 1;
    }
}

void analyze_logs(LogEntry* logs, int count) {
    int total_errors = 0;
    int level_counts[5] = {0};
    const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < count; i++) {
        total_errors += logs[i].error_count;
        
        for (int j = 0; j < 5; j++) {
            if (strcmp(logs[i].level, level_names[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", count);
    printf("Total errors: %d\n", total_errors);
    printf("Level distribution:\n");
    for (int i = 0; i < 5; i++) {
        printf("  %s: %d (%.1f%%)\n", level_names[i], level_counts[i],
               count > 0 ? (level_counts[i] * 100.0) / count : 0.0);
    }
    
    if (total_errors > 0) {
        printf("\nError entries:\n");
        for (int i = 0; i < count; i++) {
            if (logs[i].error_count > 0) {
                printf("  [%s] %s: %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
            }
        }
    }
}

int main() {
    srand((unsigned int)time(NULL));
    
    LogEntry logs[MAX_ENTRIES];
    int log_count = 500 + (rand() % 951);
    
    if (log_count > MAX_ENTRIES) {
        log_count = MAX_ENTRIES;
    }
    
    printf("Generating %d sample log entries...\n", log_count);
    generate_sample_logs(logs, log_count);
    
    printf("Analyzing logs...\n");
    analyze_logs(logs, log_count);
    
    int valid_entries = 0;
    for (int i = 0; i < log_count; i++) {
        if (is_valid_log_level(logs[i].level)) {
            valid_entries++;
        }
    }
    
    printf("\nValidation: %d/%d entries have valid log levels\n", valid_entries
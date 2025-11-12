//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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
    
    time_t now = time(NULL);
    
    for (int i = 0; i < count; i++) {
        time_t log_time = now - (count - i) * 60;
        struct tm* tm_info = localtime(&log_time);
        
        if (tm_info != NULL) {
            strftime(logs[i].timestamp, sizeof(logs[i].timestamp), 
                    "%Y-%m-%d %H:%M:%S", tm_info);
        } else {
            snprintf(logs[i].timestamp, sizeof(logs[i].timestamp), 
                    "2024-01-01 00:00:00");
        }
        
        int level_idx = rand() % 3;
        strcpy(logs[i].level, levels[level_idx]);
        
        int msg_idx = rand() % 8;
        strcpy(logs[i].message, messages[msg_idx]);
        
        logs[i].error_count = (strcmp(logs[i].level, "ERROR") == 0) ? 1 : 0;
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
    printf("INFO messages: %d\n", total_info);
    printf("WARNING messages: %d\n", total_warnings);
    printf("ERROR messages: %d\n", total_errors);
    printf("Error rate: %.2f%%\n", (total_errors * 100.0) / count);
    
    if (total_errors > 0) {
        printf("\nRecent ERROR messages:\n");
        int error_shown = 0;
        for (int i = count - 1; i >= 0 && error_shown < 5; i--) {
            if (strcmp(logs[i].level, "ERROR") == 0) {
                printf("- %s: %s\n", logs[i].timestamp, logs[i].message);
                error_shown++;
            }
        }
    }
}

int main() {
    srand((unsigned int)time(NULL));
    
    LogEntry logs[MAX_ENTRIES];
    int log_count = 500 + (rand() % 500);
    
    if (log_count > MAX_ENTRIES) {
        log_count = MAX_ENTRIES;
    }
    
    printf("Generating %d sample log entries...\n", log_count);
    generate_sample_logs(logs, log_count);
    
    printf("Analyzing logs...\n\n");
    analyze_logs(logs, log_count);
    
    printf("\nPerforming CPU-intensive pattern analysis...\n");
    
    int pattern_matches = 0;
    for (int i = 0; i < log_count; i++) {
        for (int j = 0; j < strlen(logs[i].message); j++) {
            if (strstr(logs[i].message, "time") != NULL) {
                pattern_matches++;
                break;
            }
        }
    }
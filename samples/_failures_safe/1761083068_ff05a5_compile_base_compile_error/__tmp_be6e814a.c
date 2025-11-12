//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char* token = strtok(temp, " ");
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
    
    return 1;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || 
           strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0 ||
           strcmp(level, "DEBUG") == 0;
}

void generate_sample_logs(LogEntry* logs, int count) {
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    const char* messages[] = {
        "System started successfully",
        "User login detected",
        "Database connection established",
        "High memory usage warning",
        "Network timeout occurred",
        "Backup completed",
        "Security scan finished",
        "Configuration updated"
    };
    
    time_t now = time(NULL);
    struct tm tm_info;
    localtime_r(&now, &tm_info);
    
    for (int i = 0; i < count; i++) {
        tm_info.tm_sec += 1;
        mktime(&tm_info);
        strftime(logs[i].timestamp, sizeof(logs[i].timestamp), 
                "%Y-%m-%d %H:%M:%S", &tm_info);
        
        int level_idx = rand() % 4;
        strcpy(logs[i].level, levels[level_idx]);
        
        int msg_idx = rand() % 8;
        strcpy(logs[i].message, messages[msg_idx]);
    }
}

void analyze_logs(LogEntry* logs, int count) {
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    int error_indices[MAX_ENTRIES];
    int error_count_total = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
            if (error_count_total < MAX_ENTRIES) {
                error_indices[error_count_total++] = i;
            }
        }
        else if (strcmp(logs[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (error_count_total > 0) {
        printf("\nError entries:\n");
        for (int i = 0; i < error_count_total && i < 5; i++) {
            int idx = error_indices[i];
            printf("%s %s %s\n", logs[idx].timestamp, logs[idx].level, logs[idx].message);
        }
        if (error_count_total > 5) {
            printf("... and %d more errors\n", error_count_total - 5);
        }
    }
    
    printf("\nLevel distribution:\n");
    printf("INFO: %.1f%%\n", (info_count * 100.0) / count);
    printf("WARN: %.1f%%\n", (warn_count * 100.0) / count);
    printf("ERROR: %.1f%%\n", (error_count * 100.0) / count);
    printf("DEBUG: %.1f%%\n", (debug_count * 100.0) / count);
}

int main() {
    srand((unsigned int)time(NULL));
    
    int log_count;
    printf("Enter number of log entries to generate (1-%d): ", MAX_ENTRIES);
    if (scanf("%d", &log_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (log_count < 1 || log_count > MAX_ENTRIES) {
        printf("Invalid number of entries\n");
        return
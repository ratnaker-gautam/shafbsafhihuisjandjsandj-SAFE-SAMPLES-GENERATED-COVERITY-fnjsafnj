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
    int error_code;
} LogEntry;

int parse_log_entry(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char* token = strtok(temp, " ");
    if (!token) return 0;
    if (strlen(token) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (!token) return 0;
    if (strlen(token) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, " ");
    if (!token) return 0;
    
    char* code_str = strtok(NULL, " ");
    if (!code_str) return 0;
    
    char* endptr;
    long code = strtol(code_str, &endptr, 10);
    if (endptr == code_str || *endptr != '\0') return 0;
    if (code < 0 || code > 10000) return 0;
    entry->error_code = (int)code;
    
    token = strtok(NULL, "");
    if (!token) return 0;
    if (strlen(token) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, token);
    
    return 1;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || 
           strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0;
}

void generate_sample_logs(LogEntry* logs, int count) {
    const char* levels[] = {"INFO", "WARN", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login detected",
        "Database connection established",
        "High memory usage detected",
        "Network latency above threshold",
        "Authentication failed",
        "Disk space running low",
        "Backup completed",
        "Security scan finished",
        "Configuration updated"
    };
    
    for (int i = 0; i < count; i++) {
        snprintf(logs[i].timestamp, sizeof(logs[i].timestamp), 
                "2024-%02d-%02dT%02d:%02d:%02d",
                1 + (i % 12), 1 + (i % 28), i % 24, i % 60, i % 60);
        
        int level_idx = i % 3;
        strcpy(logs[i].level, levels[level_idx]);
        
        int msg_idx = i % 10;
        strcpy(logs[i].message, messages[msg_idx]);
        
        logs[i].error_code = (i % 1000) + 1;
    }
}

void analyze_logs(LogEntry* logs, int count) {
    int info_count = 0, warn_count = 0, error_count = 0;
    int total_error_code = 0;
    int error_codes[1001] = {0};
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(logs[i].level, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
            total_error_code += logs[i].error_code;
            if (logs[i].error_code >= 0 && logs[i].error_code <= 1000) {
                error_codes[logs[i].error_code]++;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    
    if (error_count > 0) {
        printf("Average error code: %.2f\n", (double)total_error_code / error_count);
        
        int max_freq = 0;
        int common_code = 0;
        for (int i = 1; i <= 1000; i++) {
            if (error_codes[i] > max_freq) {
                max_freq = error_codes[i];
                common_code = i;
            }
        }
        if (common_code > 0) {
            printf("Most common error code: %d (appeared %d times)\n", common_code, max_freq);
        }
    }
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 5000;
    
    if (log_count <= 0 || log_count > MAX_ENTRIES) {
        fprintf(stderr, "Invalid log count\n");
        return 1;
    }
    
    printf("Generating %d sample log entries...\n", log_count);
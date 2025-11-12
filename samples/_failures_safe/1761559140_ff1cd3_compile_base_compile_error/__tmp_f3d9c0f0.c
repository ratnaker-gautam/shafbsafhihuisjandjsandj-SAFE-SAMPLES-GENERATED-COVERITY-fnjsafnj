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
           strcmp(level, "ERROR") == 0;
}

void generate_sample_logs(LogEntry* logs, int count) {
    const char* levels[] = {"INFO", "WARN", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login detected",
        "Database connection established",
        "High memory usage detected",
        "Network latency increased",
        "Backup completed",
        "Security scan finished",
        "Configuration updated"
    };
    
    time_t base_time = time(NULL);
    for (int i = 0; i < count; i++) {
        int level_idx = rand() % 3;
        int msg_idx = rand() % 8;
        
        time_t log_time = base_time - (count - i) * 60;
        struct tm* tm_info = localtime(&log_time);
        strftime(logs[i].timestamp, sizeof(logs[i].timestamp), 
                "%Y-%m-%d %H:%M:%S", tm_info);
        
        strcpy(logs[i].level, levels[level_idx]);
        strcpy(logs[i].message, messages[msg_idx]);
    }
}

void analyze_logs(LogEntry* logs, int count) {
    int info_count = 0, warn_count = 0, error_count = 0;
    char latest_timestamp[32] = "";
    char earliest_timestamp[32] = "";
    
    if (count > 0) {
        strcpy(earliest_timestamp, logs[0].timestamp);
        strcpy(latest_timestamp, logs[count-1].timestamp);
    }
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO level: %d\n", info_count);
    printf("WARN level: %d\n", warn_count);
    printf("ERROR level: %d\n", error_count);
    printf("Time range: %s to %s\n", earliest_timestamp, latest_timestamp);
    
    double info_percent = count > 0 ? (info_count * 100.0) / count : 0;
    double warn_percent = count > 0 ? (warn_count * 100.0) / count : 0;
    double error_percent = count > 0 ? (error_count * 100.0) / count : 0;
    
    printf("Distribution: INFO %.1f%%, WARN %.1f%%, ERROR %.1f%%\n", 
           info_percent, warn_percent, error_percent);
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
    
    printf("Analyzing logs...\n");
    analyze_logs(logs, log_count);
    
    printf("Performing CPU-intensive pattern analysis...\n");
    
    int pattern_matches = 0;
    for (int i = 0; i < log_count; i++) {
        for (int j = 0; j < strlen(logs[i].message); j++) {
            for (int k = j + 1; k < strlen(logs[i].message); k++) {
                if (logs[i].message[j] == logs[i].message[k]) {
                    pattern_matches++;
                }
            }
        }
    }
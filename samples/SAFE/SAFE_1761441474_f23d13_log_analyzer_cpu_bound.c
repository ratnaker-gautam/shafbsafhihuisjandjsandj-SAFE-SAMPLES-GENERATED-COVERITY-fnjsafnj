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
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    char* token = strtok(temp_line, "|");
    int part_count = 0;
    
    while (token != NULL && part_count < 3) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp) ||
        strlen(parts[1]) >= sizeof(entry->level) ||
        strlen(parts[2]) >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->timestamp, parts[0], sizeof(entry->timestamp) - 1);
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    entry->level[sizeof(entry->level) - 1] = '\0';
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs(LogEntry* logs, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    char last_timestamp[32] = "";
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(logs[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(logs[i].level, "INFO") == 0) {
            info_count++;
        }
        
        if (i == count - 1) {
            strncpy(last_timestamp, logs[i].timestamp, sizeof(last_timestamp) - 1);
            last_timestamp[sizeof(last_timestamp) - 1] = '\0';
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Last timestamp: %s\n", last_timestamp);
    
    if (count > 0) {
        double error_rate = (double)error_count / count * 100.0;
        printf("Error rate: %.2f%%\n", error_rate);
    }
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    const char* sample_logs[] = {
        "2024-01-15 10:30:15|INFO|System started successfully",
        "2024-01-15 10:31:22|WARNING|High memory usage detected",
        "2024-01-15 10:32:45|ERROR|Database connection failed",
        "2024-01-15 10:33:10|INFO|Backup completed",
        "2024-01-15 10:34:55|ERROR|File not found: config.xml",
        "2024-01-15 10:35:20|INFO|User login successful",
        "2024-01-15 10:36:30|WARNING|Slow response time",
        "2024-01-15 10:37:45|INFO|Cache cleared",
        "2024-01-15 10:38:12|ERROR|Permission denied",
        "2024-01-15 10:39:00|INFO|System shutdown initiated"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && log_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &logs[log_count])) {
            log_count++;
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    clock_t start_time = clock();
    
    for (int iteration = 0; iteration < 1000; iteration++) {
        analyze_logs(logs, log_count);
    }
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("CPU-bound analysis completed in %.3f seconds\n", cpu_time);
    
    return 0;
}
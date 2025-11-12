//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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

void analyze_logs(LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    char most_common_level[16] = "";
    int level_counts[3] = {0};
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
            level_counts[0]++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
            level_counts[1]++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
            level_counts[2]++;
        }
    }
    
    int max_count = level_counts[0];
    strcpy(most_common_level, "ERROR");
    
    if (level_counts[1] > max_count) {
        max_count = level_counts[1];
        strcpy(most_common_level, "WARNING");
    }
    if (level_counts[2] > max_count) {
        strcpy(most_common_level, "INFO");
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Most common level: %s\n", most_common_level);
    printf("Error percentage: %.2f%%\n", (float)error_count / count * 100);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 WARNING High memory usage detected",
        "2024-01-15T10:32:00 INFO User login: john_doe",
        "2024-01-15T10:33:45 ERROR Database connection failed",
        "2024-01-15T10:34:10 INFO Backup completed",
        "2024-01-15T10:35:20 WARNING CPU temperature high",
        "2024-01-15T10:36:05 ERROR File not found: config.xml",
        "2024-01-15T10:37:00 INFO Service restarted",
        "2024-01-15T10:38:30 INFO New user registered",
        "2024-01-15T10:39:15 WARNING Network latency detected"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            entry_count++;
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    clock_t start_time = clock();
    
    for (int iteration = 0; iteration < 1000; iteration++) {
        analyze_logs(entries, entry_count);
    }
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\nCPU-bound analysis completed.\n");
    printf("Processing time for 1000 iterations: %.3f seconds\n", cpu_time);
    
    return 0;
}
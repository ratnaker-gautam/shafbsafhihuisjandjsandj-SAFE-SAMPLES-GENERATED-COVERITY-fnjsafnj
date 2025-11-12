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
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->error_count = 0;
    if (strstr(message, "error") != NULL || strstr(message, "ERROR") != NULL) {
        entry->error_count = 1;
    }
    
    return 1;
}

int is_valid_timestamp(const char* timestamp) {
    if (timestamp == NULL) return 0;
    if (strlen(timestamp) != 19) return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

void analyze_logs(LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int total_errors = 0;
    int warning_count = 0;
    int info_count = 0;
    char first_timestamp[32] = "";
    char last_timestamp[32] = "";
    
    for (int i = 0; i < count; i++) {
        total_errors += entries[i].error_count;
        
        if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
        
        if (i == 0) {
            strncpy(first_timestamp, entries[i].timestamp, 31);
            first_timestamp[31] = '\0';
        }
        if (i == count - 1) {
            strncpy(last_timestamp, entries[i].timestamp, 31);
            last_timestamp[31] = '\0';
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", count);
    printf("Error occurrences: %d\n", total_errors);
    printf("Warning entries: %d\n", warning_count);
    printf("Info entries: %d\n", info_count);
    printf("Time range: %s to %s\n", first_timestamp, last_timestamp);
    
    double error_rate = (count > 0) ? (double)total_errors / count * 100.0 : 0.0;
    printf("Error rate: %.2f%%\n", error_rate);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    const char* sample_logs[] = {
        "2024-01-15 10:30:25 INFO System started successfully",
        "2024-01-15 10:31:10 WARNING High memory usage detected",
        "2024-01-15 10:32:45 ERROR Database connection failed",
        "2024-01-15 10:33:20 INFO Backup completed",
        "2024-01-15 10:34:05 ERROR File not found",
        "2024-01-15 10:35:30 INFO User login successful",
        "2024-01-15 10:36:15 WARNING Slow response time",
        "2024-01-15 10:37:40 INFO Cache cleared",
        "2024-01-15 10:38:25 ERROR Permission denied",
        "2024-01-15 10:39:10 INFO System shutdown initiated"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            if (is_valid_timestamp(entries[entry_count].timestamp)) {
                entry_count++;
            }
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log
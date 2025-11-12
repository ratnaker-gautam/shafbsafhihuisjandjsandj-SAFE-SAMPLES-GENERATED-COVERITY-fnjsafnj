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
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->error_count = 0;
    
    return 1;
}

void analyze_log_patterns(LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (count > 0) {
        printf("Error percentage: %.2f%%\n", (error_count * 100.0) / count);
        printf("Warning percentage: %.2f%%\n", (warning_count * 100.0) / count);
    }
}

void generate_sample_logs(LogEntry* entries, int* count) {
    if (entries == NULL || count == NULL) return;
    
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login detected",
        "Database connection established",
        "Memory usage high",
        "Disk space running low",
        "Network timeout occurred",
        "Critical system failure",
        "Backup completed",
        "Security alert triggered"
    };
    
    *count = 5000;
    if (*count > MAX_ENTRIES) *count = MAX_ENTRIES;
    
    time_t base_time = time(NULL);
    
    for (int i = 0; i < *count; i++) {
        time_t log_time = base_time - (rand() % 86400);
        struct tm* timeinfo = localtime(&log_time);
        
        strftime(entries[i].timestamp, sizeof(entries[i].timestamp), 
                "%Y-%m-%d %H:%M:%S", timeinfo);
        
        int level_idx = rand() % 3;
        strcpy(entries[i].level, levels[level_idx]);
        
        int msg_idx = rand() % 9;
        strcpy(entries[i].message, messages[msg_idx]);
        
        entries[i].error_count = 0;
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - CPU Bound Analysis\n");
    printf("Generating sample log data...\n");
    
    srand((unsigned int)time(NULL));
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("Error: No log entries generated\n");
        return 1;
    }
    
    printf("Analyzing %d log entries...\n", entry_count);
    
    clock_t start_time = clock();
    
    analyze_log_patterns(entries, entry_count);
    
    int error_sequences = 0;
    for (int i = 1; i < entry_count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0 && 
            strcmp(entries[i-1].level, "ERROR") == 0) {
            error_sequences++;
        }
    }
    
    printf("Consecutive error sequences: %d\n", error_sequences);
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("Analysis completed in %.4f seconds\n", cpu_time);
    printf("Processing rate: %.2f entries/second\n", entry_count / cpu_time);
    
    return 0;
}
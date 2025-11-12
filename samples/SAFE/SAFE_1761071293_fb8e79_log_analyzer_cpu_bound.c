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
        printf("Error rate: %.2f%%\n", (error_count * 100.0) / count);
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
        "Authentication failed",
        "Data corruption detected"
    };
    
    srand((unsigned int)time(NULL));
    int num_entries = 500 + rand() % 500;
    
    if (num_entries > MAX_ENTRIES) num_entries = MAX_ENTRIES;
    
    for (int i = 0; i < num_entries; i++) {
        int level_idx = rand() % 3;
        int msg_idx = rand() % 8;
        
        snprintf(entries[i].timestamp, sizeof(entries[i].timestamp), 
                "2024-%02d-%02d %02d:%02d:%02d",
                (rand() % 12) + 1, (rand() % 28) + 1,
                rand() % 24, rand() % 60, rand() % 60);
        
        strcpy(entries[i].level, levels[level_idx]);
        strcpy(entries[i].message, messages[msg_idx]);
        entries[i].error_count = 0;
    }
    
    *count = num_entries;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Generating sample log data...\n");
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries generated.\n");
        return 1;
    }
    
    printf("Analyzing %d log entries...\n", entry_count);
    
    clock_t start_time = clock();
    
    analyze_log_patterns(entries, entry_count);
    
    int error_sequences = 0;
    for (int i = 0; i < entry_count - 1; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0 && 
            strcmp(entries[i + 1].level, "ERROR") == 0) {
            error_sequences++;
        }
    }
    
    printf("Consecutive error sequences: %d\n", error_sequences);
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("Analysis completed in %.6f seconds\n", cpu_time);
    
    return 0;
}
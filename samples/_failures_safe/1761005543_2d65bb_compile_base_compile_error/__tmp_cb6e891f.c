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
        printf("Warning rate: %.2f%%\n", (warning_count * 100.0) / count);
    }
}

void generate_sample_logs(LogEntry* entries, int* count) {
    if (entries == NULL || count == NULL) return;
    
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login attempt",
        "Database connection established",
        "Memory usage high",
        "Disk space low",
        "Network timeout",
        "Authentication failed",
        "Service unavailable",
        "Configuration updated",
        "Backup completed"
    };
    
    *count = 0;
    for (int i = 0; i < 50 && *count < MAX_ENTRIES; i++) {
        int level_idx = rand() % 3;
        int msg_idx = rand() % 10;
        
        snprintf(entries[*count].timestamp, sizeof(entries[*count].timestamp), 
                "2024-%02d-%02dT%02d:%02d:%02dZ", 
                (rand() % 12) + 1, (rand() % 28) + 1,
                rand() % 24, rand() % 60, rand() % 60);
        
        strcpy(entries[*count].level, levels[level_idx]);
        strncpy(entries[*count].message, messages[msg_idx], 
                sizeof(entries[*count].message) - 1);
        entries[*count].message[sizeof(entries[*count].message) - 1] = '\0';
        
        (*count)++;
    }
}

int main() {
    srand((unsigned int)time(NULL));
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Generating sample log data...\n");
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries generated.\n");
        return 1;
    }
    
    printf("Parsed %d log entries.\n", entry_count);
    
    clock_t start_time = clock();
    
    analyze_log_patterns(entries, entry_count);
    
    int total_errors = 0;
    for (int i = 0; i < entry_count; i++) {
        for (int j = 0; j < strlen(entries[i].message); j++) {
            if (entries[i].message[j] == 'e' || entries[i].message[j] == 'E') {
                entries[i].error_count++;
            }
        }
        total_errors += entries[i].error_count;
    }
    
    printf("Total 'e' characters in all messages: %d\n", total_errors);
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("CPU time used: %.6f seconds\n
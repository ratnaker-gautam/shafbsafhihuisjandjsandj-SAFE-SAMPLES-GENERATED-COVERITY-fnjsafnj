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
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* token = strtok(temp_line, " ");
    if (token == NULL) return 0;
    
    strncpy(entry->timestamp, token, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    
    strncpy(entry->level, token, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    
    strncpy(entry->message, token, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int count_log_level(const LogEntry* entries, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void generate_sample_logs(LogEntry* entries, int* count) {
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    const char* messages[] = {
        "User login successful",
        "Database connection established",
        "File not found",
        "Memory allocation failed",
        "Network timeout",
        "Configuration loaded",
        "Invalid input detected",
        "Backup completed"
    };
    
    *count = 5000;
    if (*count > MAX_ENTRIES) *count = MAX_ENTRIES;
    
    for (int i = 0; i < *count; i++) {
        snprintf(entries[i].timestamp, sizeof(entries[i].timestamp), 
                "2024-%02d-%02dT%02d:%02d:%02dZ",
                (i % 12) + 1, (i % 28) + 1, 
                i % 24, (i * 3) % 60, (i * 7) % 60);
        
        strncpy(entries[i].level, levels[i % 4], sizeof(entries[i].level) - 1);
        entries[i].level[sizeof(entries[i].level) - 1] = '\0';
        
        strncpy(entries[i].message, messages[i % 8], sizeof(entries[i].message) - 1);
        entries[i].message[sizeof(entries[i].message) - 1] = '\0';
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Generating sample log data...\n");
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 1;
    }
    
    printf("Analyzing %d log entries...\n", entry_count);
    
    clock_t start_time = clock();
    
    int info_count = count_log_level(entries, entry_count, "INFO");
    int warning_count = count_log_level(entries, entry_count, "WARNING");
    int error_count = count_log_level(entries, entry_count, "ERROR");
    int debug_count = count_log_level(entries, entry_count, "DEBUG");
    
    int total_processed = info_count + warning_count + error_count + debug_count;
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("Log Analysis Results:\n");
    printf("INFO:    %d entries\n", info_count);
    printf("WARNING: %d entries\n", warning_count);
    printf("ERROR:   %d entries\n", error_count);
    printf("DEBUG:   %d entries\n", debug_count);
    printf("Total processed: %d entries\n", total_processed);
    printf("Analysis completed in %.6f seconds\n", cpu_time);
    
    if (total_processed != entry_count) {
        printf("Warning: Some entries may not have been categorized.\n");
    }
    
    return 0;
}
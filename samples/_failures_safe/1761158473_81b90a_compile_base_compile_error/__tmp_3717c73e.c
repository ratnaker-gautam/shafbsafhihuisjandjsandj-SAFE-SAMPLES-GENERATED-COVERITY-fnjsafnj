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
        "Backup completed",
        "Security alert triggered",
        "Service started"
    };
    
    *count = 0;
    for (int i = 0; i < 500 && *count < MAX_ENTRIES; i++) {
        int level_idx = rand() % 4;
        int msg_idx = rand() % 10;
        
        snprintf(entries[*count].timestamp, sizeof(entries[*count].timestamp), 
                "2024-%02d-%02dT%02d:%02d:%02d", 
                (rand() % 12) + 1, (rand() % 28) + 1,
                rand() % 24, rand() % 60, rand() % 60);
        
        strncpy(entries[*count].level, levels[level_idx], sizeof(entries[*count].level) - 1);
        entries[*count].level[sizeof(entries[*count].level) - 1] = '\0';
        
        strncpy(entries[*count].message, messages[msg_idx], sizeof(entries[*count].message) - 1);
        entries[*count].message[sizeof(entries[*count].message) - 1] = '\0';
        
        (*count)++;
    }
}

int main(void) {
    srand((unsigned int)time(NULL));
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Generating sample log data...\n");
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries generated.\n");
        return 1;
    }
    
    printf("Analyzing %d log entries...\n\n", entry_count);
    
    int info_count = count_log_level(entries, entry_count, "INFO");
    int warning_count = count_log_level(entries, entry_count, "WARNING");
    int error_count = count_log_level(entries, entry_count, "ERROR");
    int debug_count = count_log_level(entries, entry_count, "DEBUG");
    
    printf("Log Level Statistics:\n");
    printf("INFO:    %d entries (%.1f%%)\n", info_count, (info_count * 100.0) / entry_count);
    printf("WARNING: %d entries (%.1f%%)\n", warning_count, (warning_count * 100.0) / entry_count);
    printf("ERROR:   %d entries (%.1f%%)\n", error_count, (error_count * 100.0) / entry_count);
    printf("DEBUG:   %d entries (%.1f%%)\n", debug_count, (debug_count * 100.0) / entry_count);
    
    int error_entries = 0;
    printf("\nError entries:\n");
    for (int i = 0; i < entry_count && error_entries < 10; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            error_entries++;
        }
    }
    
    if (error_count > 10) {
        printf("... and %
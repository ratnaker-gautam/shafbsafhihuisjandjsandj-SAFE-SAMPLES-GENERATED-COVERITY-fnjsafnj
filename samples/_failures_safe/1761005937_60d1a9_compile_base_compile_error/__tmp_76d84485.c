//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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
    
    *count = 5000;
    for (int i = 0; i < *count; i++) {
        snprintf(entries[i].timestamp, sizeof(entries[i].timestamp), 
                "2024-%02d-%02dT%02d:%02d:%02d",
                (i % 12) + 1, (i % 28) + 1, 
                i % 24, (i * 3) % 60, (i * 7) % 60);
        
        strncpy(entries[i].level, levels[i % 4], sizeof(entries[i].level) - 1);
        entries[i].level[sizeof(entries[i].level) - 1] = '\0';
        
        strncpy(entries[i].message, messages[i % 10], sizeof(entries[i].message) - 1);
        entries[i].message[sizeof(entries[i].message) - 1] = '\0';
    }
}

int search_logs(const LogEntry* entries, int count, const char* search_term) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, search_term) != NULL) {
            printf("Found: [%s] %s - %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found++;
        }
    }
    return found;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count <= 0 || entry_count > MAX_ENTRIES) {
        fprintf(stderr, "Invalid entry count\n");
        return 1;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    
    int info_count = count_log_level(entries, entry_count, "INFO");
    int warning_count = count_log_level(entries, entry_count, "WARNING");
    int error_count = count_log_level(entries, entry_count, "ERROR");
    int debug_count = count_log_level(entries, entry_count, "DEBUG");
    
    printf("INFO entries: %d (%.1f%%)\n", info_count, (info_count * 100.0) / entry_count);
    printf("WARNING entries: %d (%.1f%%)\n", warning_count, (warning_count * 100.0) / entry_count);
    printf("ERROR entries: %d (%.1f%%)\n", error_count, (error_count * 100.0) / entry_count);
    printf("DEBUG entries: %d (%.1f%%)\n", debug_count, (debug_count * 100.0) / entry_count);
    
    int error_per_hour[24] = {0};
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            int hour;
            if (sscanf(entries[i].timestamp, "202
//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[64];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 64 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
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
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
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
    
    time_t now = time(NULL);
    *count = 5000 + (rand() % 5000);
    
    if (*count > MAX_ENTRIES) *count = MAX_ENTRIES;
    
    for (int i = 0; i < *count; i++) {
        time_t log_time = now - (rand() % 86400);
        struct tm* tm_info = localtime(&log_time);
        
        strftime(entries[i].timestamp, 64, "%Y-%m-%d %H:%M:%S", tm_info);
        
        int level_idx = rand() % 4;
        strcpy(entries[i].level, levels[level_idx]);
        
        int msg_idx = rand() % 8;
        strcpy(entries[i].message, messages[msg_idx]);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    srand((unsigned int)time(NULL));
    
    printf("Generating sample log data...\n");
    generate_sample_logs(entries, &entry_count);
    
    printf("Analyzing %d log entries...\n\n", entry_count);
    
    int info_count = count_log_level(entries, entry_count, "INFO");
    int warn_count = count_log_level(entries, entry_count, "WARN");
    int error_count = count_log_level(entries, entry_count, "ERROR");
    int debug_count = count_log_level(entries, entry_count, "DEBUG");
    
    printf("Log Level Distribution:\n");
    printf("INFO:  %d entries (%.1f%%)\n", info_count, (info_count * 100.0) / entry_count);
    printf("WARN:  %d entries (%.1f%%)\n", warn_count, (warn_count * 100.0) / entry_count);
    printf("ERROR: %d entries (%.1f%%)\n", error_count, (error_count * 100.0) / entry_count);
    printf("DEBUG: %d entries (%.1f%%)\n", debug_count, (debug_count * 100.0) / entry_count);
    
    printf("\nSearching for error patterns...\n");
    int critical_errors = 0;
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            if (strstr(entries[i].message, "failed") != NULL || 
                strstr(entries[i].message, "timeout") != NULL ||
                strstr(entries[i].message, "memory") != NULL) {
                critical_errors++;
                printf("Critical error found: %s - %s\n", entries[i].timestamp, entries[i].message);
            }
        }
    }
    
    printf("\nTotal critical errors: %d\n", critical_errors);
    
    return 0;
}
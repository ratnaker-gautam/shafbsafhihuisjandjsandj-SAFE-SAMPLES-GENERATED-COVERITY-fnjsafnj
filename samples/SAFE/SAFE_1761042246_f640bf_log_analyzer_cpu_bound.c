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
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    if (timestamp == NULL) return 0;
    
    char* level = strtok(NULL, " ");
    if (level == NULL) return 0;
    
    char* message = strtok(NULL, "\n");
    if (message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
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
    for (int i = 0; i < *count; i++) {
        int level_idx = rand() % 4;
        int msg_idx = rand() % 8;
        
        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);
        strftime(entries[i].timestamp, sizeof(entries[i].timestamp), 
                "%Y-%m-%d %H:%M:%S", tm_info);
        
        strcpy(entries[i].level, levels[level_idx]);
        strcpy(entries[i].message, messages[msg_idx]);
    }
}

int main(void) {
    srand((unsigned int)time(NULL));
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries to analyze\n");
        return 1;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    int info_count = count_log_level(entries, entry_count, "INFO");
    int warning_count = count_log_level(entries, entry_count, "WARNING");
    int error_count = count_log_level(entries, entry_count, "ERROR");
    int debug_count = count_log_level(entries, entry_count, "DEBUG");
    
    printf("INFO: %d (%.1f%%)\n", info_count, (info_count * 100.0) / entry_count);
    printf("WARNING: %d (%.1f%%)\n", warning_count, (warning_count * 100.0) / entry_count);
    printf("ERROR: %d (%.1f%%)\n", error_count, (error_count * 100.0) / entry_count);
    printf("DEBUG: %d (%.1f%%)\n", debug_count, (debug_count * 100.0) / entry_count);
    
    int error_entries = 0;
    printf("\nRecent ERROR entries:\n");
    for (int i = entry_count - 1; i >= 0 && error_entries < 5; i--) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            error_entries++;
        }
    }
    
    if (error_entries == 0) {
        printf("No ERROR entries found\n");
    }
    
    return 0;
}
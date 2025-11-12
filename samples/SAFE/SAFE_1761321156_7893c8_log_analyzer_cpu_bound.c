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
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp_start = strchr(temp_line, '[');
    if (!timestamp_start) return 0;
    
    char* timestamp_end = strchr(timestamp_start + 1, ']');
    if (!timestamp_end) return 0;
    
    size_t timestamp_len = timestamp_end - timestamp_start - 1;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    
    strncpy(entry->timestamp, timestamp_start + 1, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    char* level_start = strchr(timestamp_end + 1, '[');
    if (!level_start) return 0;
    
    char* level_end = strchr(level_start + 1, ']');
    if (!level_end) return 0;
    
    size_t level_len = level_end - level_start - 1;
    if (level_len >= sizeof(entry->level)) return 0;
    
    strncpy(entry->level, level_start + 1, level_len);
    entry->level[level_len] = '\0';
    
    char* message_start = level_end + 1;
    while (*message_start && isspace(*message_start)) message_start++;
    
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    return 1;
}

int count_log_levels(LogEntry* entries, int count, const char* level) {
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
    
    time_t now = time(NULL);
    *count = 0;
    
    for (int i = 0; i < 50 && *count < MAX_ENTRIES; i++) {
        time_t log_time = now - (rand() % 86400);
        struct tm* tm_info = localtime(&log_time);
        
        LogEntry entry;
        strftime(entry.timestamp, sizeof(entry.timestamp), 
                "%Y-%m-%d %H:%M:%S", tm_info);
        
        strncpy(entry.level, levels[rand() % 4], sizeof(entry.level) - 1);
        entry.level[sizeof(entry.level) - 1] = '\0';
        
        strncpy(entry.message, messages[rand() % 10], sizeof(entry.message) - 1);
        entry.message[sizeof(entry.message) - 1] = '\0';
        
        entries[*count] = entry;
        (*count)++;
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Processing sample log data...\n");
    
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 1;
    }
    
    printf("Analyzing %d log entries...\n", entry_count);
    
    int info_count = count_log_levels(entries, entry_count, "INFO");
    int warning_count = count_log_levels(entries, entry_count, "WARNING");
    int error_count = count_log_levels(entries, entry_count, "ERROR");
    int debug_count = count_log_levels(entries, entry_count, "DEBUG");
    
    printf("Log Level Summary:\n");
    printf("INFO:    %d entries\n", info_count);
    printf("WARNING: %d entries\n", warning_count);
    printf("ERROR:   %d entries\n", error_count);
    printf("DEBUG:   %d entries\n", debug_count);
    
    int total_processed = info_count + warning_count + error_count + debug_count;
    if (total_processed != entry_count) {
        printf("Warning: Some entries could not be categorized.\n");
    }
    
    printf("Analysis complete. Processed %d total entries.\n", total_processed);
    
    return 0;
}
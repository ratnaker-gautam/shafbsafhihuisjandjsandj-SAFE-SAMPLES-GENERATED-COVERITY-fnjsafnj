//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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
    
    char* timestamp_start = strchr(temp_line, '[');
    if (timestamp_start == NULL) return 0;
    
    char* timestamp_end = strchr(timestamp_start + 1, ']');
    if (timestamp_end == NULL) return 0;
    
    size_t timestamp_len = timestamp_end - timestamp_start - 1;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    
    strncpy(entry->timestamp, timestamp_start + 1, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    char* level_start = strchr(timestamp_end + 1, '[');
    if (level_start == NULL) return 0;
    
    char* level_end = strchr(level_start + 1, ']');
    if (level_end == NULL) return 0;
    
    size_t level_len = level_end - level_start - 1;
    if (level_len >= sizeof(entry->level)) return 0;
    
    strncpy(entry->level, level_start + 1, level_len);
    entry->level[level_len] = '\0';
    
    char* message_start = level_end + 1;
    while (*message_start == ' ' || *message_start == '\t') message_start++;
    
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    char* newline = strchr(entry->message, '\n');
    if (newline != NULL) *newline = '\0';
    
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
        "File upload completed",
        "Memory usage high",
        "Network timeout occurred",
        "Configuration loaded",
        "Backup process started",
        "Security scan completed"
    };
    
    time_t base_time = time(NULL);
    *count = 0;
    
    for (int i = 0; i < 500 && *count < MAX_ENTRIES; i++) {
        LogEntry entry;
        
        time_t log_time = base_time - (rand() % 86400);
        struct tm* tm_info = localtime(&log_time);
        if (tm_info != NULL) {
            strftime(entry.timestamp, sizeof(entry.timestamp), 
                    "%Y-%m-%d %H:%M:%S", tm_info);
        } else {
            snprintf(entry.timestamp, sizeof(entry.timestamp), 
                    "2024-01-01 00:00:00");
        }
        
        int level_idx = rand() % 4;
        strncpy(entry.level, levels[level_idx], sizeof(entry.level) - 1);
        entry.level[sizeof(entry.level) - 1] = '\0';
        
        int msg_idx = rand() % 8;
        strncpy(entry.message, messages[msg_idx], sizeof(entry.message) - 1);
        entry.message[sizeof(entry.message) - 1] = '\0';
        
        entries[*count] = entry;
        (*count)++;
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    srand(42);
    generate_sample_logs(entries, &entry_count);
    
    printf("Log Analysis Results:\n");
    printf("Total log entries: %d\n", entry_count);
    
    int info_count = count_log_level(entries, entry_count, "INFO");
    int warning_count = count_log_level(entries, entry_count, "WARNING");
    int error_count = count_log_level(entries, entry_count, "ERROR");
    int debug_count = count_log_level(entries, entry_count, "DEBUG");
    
    printf("INFO entries: %d (%.1f%%)\n", info_count, 
           entry_count > 0 ? (info_count * 100.0) / entry_count : 0.0);
    printf("WARNING entries: %d (%.1f%%)\n", warning_count, 
           entry_count > 0 ? (warning_count * 100.0) / entry_count : 0.0
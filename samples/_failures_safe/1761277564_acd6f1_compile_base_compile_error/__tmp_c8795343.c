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

int is_error_level(const char* level) {
    if (level == NULL) return 0;
    return strcmp(level, "ERROR") == 0 || strcmp(level, "FATAL") == 0;
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
    printf("INFO entries: %d\n", info_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("ERROR entries: %d\n", error_count);
    printf("Error percentage: %.2f%%\n", count > 0 ? (error_count * 100.0) / count : 0.0);
}

void generate_sample_logs(LogEntry* entries, int* count) {
    if (entries == NULL || count == NULL) return;
    
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login attempted",
        "Database connection established",
        "Memory usage high",
        "Disk space low",
        "Network timeout occurred",
        "Invalid input detected",
        "Backup completed",
        "Security alert triggered",
        "Service restarted"
    };
    
    int num_levels = sizeof(levels) / sizeof(levels[0]);
    int num_messages = sizeof(messages) / sizeof(messages[0]);
    
    *count = 0;
    for (int i = 0; i < 50 && *count < MAX_ENTRIES; i++) {
        LogEntry entry;
        
        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);
        if (tm_info == NULL) continue;
        
        strftime(entry.timestamp, sizeof(entry.timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
        
        int level_idx = rand() % num_levels;
        strcpy(entry.level, levels[level_idx]);
        
        int msg_idx = rand() % num_messages;
        strcpy(entry.message, messages[msg_idx]);
        
        entries[*count] = entry;
        (*count)++;
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    srand((unsigned int)time(NULL));
    
    printf("Generating sample log data...\n");
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 1;
    }
    
    printf("Processing %d log entries...\n", entry_count);
    
    clock_t start = clock();
    
    for (int i = 0; i < entry_count; i++) {
        char line_buffer[MAX_LINE_LENGTH];
        snprintf(line_buffer, sizeof(line_buffer), "%s %s %s", 
                 entries[i].timestamp, entries[i].level, entries[i].message);
        
        if (!parse_log_line(line_buffer, &entries[i])) {
            printf("Failed to parse log entry %d\n", i);
            return 1;
        }
    }
    
    analyze_log_patterns(entries, entry_count);
    
    int error_sequences = 0;
    for (int i = 1; i < entry_count; i++) {
        if (is_error_level(entries[i].level) && is_error_level(entries[i-1].level
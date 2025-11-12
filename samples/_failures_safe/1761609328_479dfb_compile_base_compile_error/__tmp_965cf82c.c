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
        printf("Error percentage: %.2f%%\n", (error_count * 100.0) / count);
        printf("Warning percentage: %.2f%%\n", (warning_count * 100.0) / count);
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
        "Disk space low",
        "Network timeout occurred",
        "Authentication failed",
        "Service unavailable",
        "Configuration updated",
        "Backup completed"
    };
    
    int num_levels = sizeof(levels) / sizeof(levels[0]);
    int num_messages = sizeof(messages) / sizeof(messages[0]);
    
    *count = 0;
    for (int i = 0; i < 50 && *count < MAX_ENTRIES; i++) {
        int level_idx = rand() % num_levels;
        int msg_idx = rand() % num_messages;
        
        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);
        
        LogEntry entry;
        strftime(entry.timestamp, sizeof(entry.timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
        strcpy(entry.level, levels[level_idx]);
        strcpy(entry.message, messages[msg_idx]);
        entry.error_count = 0;
        
        entries[*count] = entry;
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
    
    printf("Parsing %d log entries...\n", entry_count);
    
    LogEntry parsed_entries[MAX_ENTRIES];
    int parsed_count = 0;
    
    for (int i = 0; i < entry_count && parsed_count < MAX_ENTRIES; i++) {
        char line[MAX_LINE_LENGTH];
        snprintf(line, sizeof(line), "%s %s %s", 
                entries[i].timestamp, entries[i].level, entries[i].message);
        
        if (parse_log_line(line, &parsed_entries[parsed_count])) {
            parsed_count++;
        }
    }
    
    printf("Successfully parsed %d log entries.\n", parsed_count);
    
    if (parsed_count > 0) {
        analyze_log_patterns(parsed_entries, parsed_count);
        
        printf("\nSample log entries:\n");
        int display_count = (parsed_count < 5) ? parsed_count : 5;
        for (int i = 0
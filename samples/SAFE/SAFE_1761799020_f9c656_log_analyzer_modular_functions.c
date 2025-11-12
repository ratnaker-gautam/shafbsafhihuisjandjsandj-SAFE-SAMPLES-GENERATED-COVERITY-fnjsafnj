//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    if (timestamp == NULL) return 0;
    
    char* level = strtok(NULL, " ");
    if (level == NULL) return 0;
    
    char* message = strtok(NULL, "\n");
    if (message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int count_log_level(const struct LogEntry* entries, int count, const char* level) {
    if (entries == NULL || level == NULL || count <= 0) return 0;
    
    int level_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            level_count++;
        }
    }
    return level_count;
}

void print_summary(const struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", count_log_level(entries, count, "INFO"));
    printf("WARNING entries: %d\n", count_log_level(entries, count, "WARNING"));
    printf("ERROR entries: %d\n", count_log_level(entries, count, "ERROR"));
    printf("DEBUG entries: %d\n", count_log_level(entries, count, "DEBUG"));
}

void generate_sample_logs(struct LogEntry* entries, int* count) {
    if (entries == NULL || count == NULL) return;
    
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    const char* messages[] = {
        "System started successfully",
        "User login detected",
        "Database connection established",
        "High memory usage detected",
        "Network timeout occurred",
        "Backup completed",
        "Security scan finished",
        "Configuration updated"
    };
    
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    *count = 8;
    for (int i = 0; i < *count; i++) {
        char timestamp[32];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
        strcpy(entries[i].timestamp, timestamp);
        
        int level_idx = i % 4;
        strcpy(entries[i].level, levels[level_idx]);
        
        int msg_idx = i % 8;
        strcpy(entries[i].message, messages[msg_idx]);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer\n");
    printf("Generating sample log data...\n");
    
    generate_sample_logs(entries, &entry_count);
    
    if (entry_count > 0) {
        printf("Successfully loaded %d log entries\n", entry_count);
        print_summary(entries, entry_count);
        
        printf("\nFirst 3 log entries:\n");
        int display_count = (entry_count < 3) ? entry_count : 3;
        for (int i = 0; i < display_count; i++) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    } else {
        printf("No log entries to analyze\n");
    }
    
    return 0;
}
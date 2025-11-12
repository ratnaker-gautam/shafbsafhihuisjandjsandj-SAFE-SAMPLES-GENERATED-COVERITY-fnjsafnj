//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, timestamp);
    
    char* level = strtok(NULL, " ");
    if (level == NULL) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, level);
    
    char* message = strtok(NULL, "\n");
    if (message == NULL) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || 
           strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0 ||
           strcmp(level, "DEBUG") == 0;
}

void generate_sample_logs() {
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    const char* messages[] = {
        "System started successfully",
        "User login detected",
        "Database connection established",
        "High memory usage detected",
        "Failed to open file",
        "Backup completed",
        "Network timeout occurred",
        "Configuration updated"
    };
    
    FILE* file = fopen("sample.log", "w");
    if (file == NULL) return;
    
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    for (int i = 0; i < 20; i++) {
        char timestamp[32];
        tm_info->tm_hour = (tm_info->tm_hour + i) % 24;
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
        
        int level_idx = rand() % 4;
        int msg_idx = rand() % 8;
        
        fprintf(file, "%s %s %s\n", timestamp, levels[level_idx], messages[msg_idx]);
    }
    
    fclose(file);
}

int main() {
    printf("Log Analyzer\n");
    printf("============\n\n");
    
    printf("Generating sample log file...\n");
    generate_sample_logs();
    
    FILE* file = fopen("sample.log", "r");
    if (file == NULL) {
        printf("Error: Cannot open log file\n");
        return 1;
    }
    
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                entries[entry_count] = entry;
                entry_count++;
                
                if (strcmp(entry.level, "INFO") == 0) info_count++;
                else if (strcmp(entry.level, "WARN") == 0) warn_count++;
                else if (strcmp(entry.level, "ERROR") == 0) error_count++;
                else if (strcmp(entry.level, "DEBUG") == 0) debug_count++;
            }
        }
    }
    
    fclose(file);
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("INFO:  %d\n", info_count);
    printf("WARN:  %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    printf("\nRecent log entries:\n");
    printf("==================\n");
    int display_count = (entry_count < 5) ? entry_count : 5;
    for (int i = 0; i < display_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    if (error_count > 0) {
        printf("\nWarning: %d error(s) detected in the log!\n", error_count);
    }
    
    return 0;
}
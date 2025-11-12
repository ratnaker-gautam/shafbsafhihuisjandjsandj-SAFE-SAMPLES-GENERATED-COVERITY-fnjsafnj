//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[64];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 64 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    if (level == NULL) return 0;
    return strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (count > 0) {
        printf("\nFirst entry: %s %s %s\n", entries[0].timestamp, entries[0].level, entries[0].message);
        printf("Last entry: %s %s %s\n", entries[count-1].timestamp, entries[count-1].level, entries[count-1].message);
    }
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    if (filename == NULL || entries == NULL || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < max_entries) {
        line[strcspn(line, "\n")] = 0;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry) && is_valid_level(entry.level)) {
            entries[entry_count] = entry;
            entry_count++;
        }
    }
    
    fclose(file);
    return entry_count;
}

void generate_sample_log(const char* filename) {
    if (filename == NULL) return;
    
    FILE* file = fopen(filename, "w");
    if (file == NULL) return;
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    const char* messages[] = {
        "System started successfully",
        "User login attempted",
        "Database connection established",
        "Low disk space warning",
        "Authentication failed",
        "Backup completed",
        "Network timeout occurred",
        "Configuration updated"
    };
    
    time_t current_time = time(NULL);
    
    for (int i = 0; i < 20; i++) {
        time_t log_time = current_time - (i * 3600);
        struct tm* timeinfo = localtime(&log_time);
        
        char timestamp[64];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H:%M:%S", timeinfo);
        
        int level_idx = i % 4;
        int msg_idx = i % 8;
        
        fprintf(file, "%s %s %s\n", timestamp, levels[level_idx], messages[msg_idx]);
    }
    
    fclose(file);
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    const char* filename = "sample.log";
    
    printf("Log Analyzer\n");
    printf("Generating sample log file...\n");
    generate_sample_log(filename);
    
    printf("Reading and analyzing log file...\n");
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (entry_count > 0) {
        analyze_logs(
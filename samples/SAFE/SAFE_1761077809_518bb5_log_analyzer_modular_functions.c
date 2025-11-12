//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
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
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 31) return 0;
    if (strlen(level) >= 15) return 0;
    if (strlen(message) >= 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    if (level == NULL) return 0;
    return (strcmp(level, "INFO") == 0 || 
            strcmp(level, "WARN") == 0 || 
            strcmp(level, "ERROR") == 0);
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int info_count = 0, warn_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    printf("Total entries: %d\n", count);
}

int read_log_file(const char* filename, struct LogEntry* entries) {
    if (filename == NULL || entries == NULL) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < MAX_ENTRIES) {
        line[strcspn(line, "\n")] = '\0';
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry) && is_valid_level(entry.level)) {
            entries[count] = entry;
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void generate_sample_log(const char* filename) {
    if (filename == NULL) return;
    
    FILE* file = fopen(filename, "w");
    if (file == NULL) return;
    
    const char* levels[] = {"INFO", "WARN", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login from 192.168.1.100",
        "Database connection established",
        "High memory usage detected",
        "Failed authentication attempt",
        "Backup completed",
        "Network latency above threshold",
        "Configuration file updated"
    };
    
    for (int i = 0; i < 20; i++) {
        int level_idx = rand() % 3;
        int msg_idx = rand() % 8;
        fprintf(file, "2024-01-%02dT%02d:%02d:%02dZ %s %s\n",
                (rand() % 28) + 1, rand() % 24, rand() % 60, rand() % 60,
                levels[level_idx], messages[msg_idx]);
    }
    
    fclose(file);
}

int main() {
    srand((unsigned int)time(NULL));
    
    const char* filename = "sample.log";
    
    printf("Generating sample log file...\n");
    generate_sample_log(filename);
    
    struct LogEntry entries[MAX_ENTRIES];
    int count = read_log_file(filename, entries);
    
    if (count == 0) {
        printf("No valid log entries found or file could not be read.\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    printf("\nFirst 5 log entries:\n");
    for (int i = 0; i < 5 && i < count; i++) {
        printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}
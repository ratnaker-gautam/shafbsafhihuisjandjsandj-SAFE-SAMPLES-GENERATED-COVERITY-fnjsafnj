//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
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
    return (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || 
            strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0);
}

void analyze_logs(struct LogEntry* logs, int count) {
    if (logs == NULL || count <= 0) return;
    
    int info_count = 0;
    int warn_count = 0;
    int error_count = 0;
    int debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(logs[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    printf("DEBUG entries: %d\n", debug_count);
    printf("Total entries: %d\n", count);
}

int read_log_file(const char* filename, struct LogEntry* logs, int max_entries) {
    if (filename == NULL || logs == NULL || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        line[strcspn(line, "\n")] = 0;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry) && is_valid_level(entry.level)) {
            logs[count] = entry;
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
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    const char* messages[] = {
        "System started successfully",
        "User login attempted",
        "Database connection established",
        "High memory usage detected",
        "Failed to open file",
        "Configuration loaded",
        "Network timeout occurred",
        "Backup completed"
    };
    
    for (int i = 0; i < 20; i++) {
        int level_idx = rand() % 4;
        int msg_idx = rand() % 8;
        fprintf(file, "2024-01-%02dT%02d:%02d:%02d %s %s\n",
                15 + (i % 5), 10 + (i % 10), 20 + (i % 40), 30 + (i % 30),
                levels[level_idx], messages[msg_idx]);
    }
    
    fclose(file);
}

int main() {
    srand((unsigned int)time(NULL));
    
    const char* filename = "sample.log";
    
    printf("Generating sample log file...\n");
    generate_sample_log(filename);
    
    struct LogEntry logs[MAX_ENTRIES];
    int entry_count = read_log_file(filename, logs, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found or error reading file.\n");
        return 1;
    }
    
    printf("Successfully read %d log entries.\n", entry_count);
    analyze_logs(logs, entry_count);
    
    printf("\nFirst 5 log entries:\n");
    for (int i = 0; i < 5 && i < entry_count; i++) {
        printf("%s %s %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }
    
    return
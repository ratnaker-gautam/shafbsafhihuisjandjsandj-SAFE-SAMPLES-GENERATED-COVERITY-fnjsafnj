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
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
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

void display_statistics(const struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", count_log_level(entries, count, "INFO"));
    printf("WARNING entries: %d\n", count_log_level(entries, count, "WARNING"));
    printf("ERROR entries: %d\n", count_log_level(entries, count, "ERROR"));
    printf("DEBUG entries: %d\n", count_log_level(entries, count, "DEBUG"));
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    if (filename == NULL || entries == NULL || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        }
    }
    
    fclose(file);
    return entry_count;
}

int generate_sample_log(const char* filename) {
    if (filename == NULL) return 0;
    
    FILE* file = fopen(filename, "w");
    if (file == NULL) return 0;
    
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    const char* messages[] = {
        "System started successfully",
        "User login detected",
        "Database connection established",
        "Memory usage high",
        "Network timeout occurred",
        "Configuration file loaded",
        "Backup completed",
        "Security scan finished"
    };
    
    time_t current_time = time(NULL);
    
    for (int i = 0; i < 20; i++) {
        struct tm* time_info = localtime(&current_time);
        char timestamp[32];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H:%M:%S", time_info);
        
        const char* level = levels[i % 4];
        const char* message = messages[i % 8];
        
        fprintf(file, "%s %s %s\n", timestamp, level, message);
        
        current_time -= 3600;
    }
    
    fclose(file);
    return 1;
}

int main() {
    const char* filename = "sample.log";
    
    printf("Generating sample log file...\n");
    if (!generate_sample_log(filename)) {
        printf("Failed to generate sample log file.\n");
        return 1;
    }
    
    printf("Reading log file...\n");
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No log entries found or error reading file.\n");
        return 1;
    }
    
    printf("Successfully read %d log entries.\n", entry_count);
    display_statistics(entries, entry_count);
    
    printf("\nFirst 5 log entries:\n");
    for (int i = 0; i < 5 && i < entry_count; i++) {
        printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}
//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
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
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) - 1) return 0;
    if (strlen(level) >= sizeof(entry->level) - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int count_log_levels(const struct LogEntry* entries, int count, const char* level) {
    if (entries == NULL || level == NULL || count <= 0) return 0;
    
    int level_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            level_count++;
        }
    }
    return level_count;
}

void print_analysis(const struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_counts[4] = {0};
    
    for (int i = 0; i < 4; i++) {
        level_counts[i] = count_log_levels(entries, count, levels[i]);
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
    
    int error_count = count_log_levels(entries, count, "ERROR");
    if (count > 0) {
        double error_percentage = (double)error_count / count * 100.0;
        printf("Error percentage: %.2f%%\n", error_percentage);
    }
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    if (filename == NULL || entries == NULL || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) > 0) {
            if (parse_log_line(line, &entries[entry_count])) {
                entry_count++;
            }
        }
    }
    
    fclose(file);
    return entry_count;
}

int validate_filename(const char* filename) {
    if (filename == NULL) return 0;
    if (strlen(filename) == 0) return 0;
    
    for (size_t i = 0; i < strlen(filename); i++) {
        if (!isalnum(filename[i]) && filename[i] != '.' && filename[i] != '_' && filename[i] != '-') {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    char filename[256];
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading filename\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = '\0';
    
    if (!validate_filename(filename)) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found or file cannot be read\n");
        return 1;
    }
    
    print_analysis(entries, entry_count);
    
    return 0;
}
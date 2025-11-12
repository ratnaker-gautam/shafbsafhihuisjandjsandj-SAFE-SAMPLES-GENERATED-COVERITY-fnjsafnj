//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

struct LogEntry {
    char timestamp[20];
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return LOG_DEBUG;
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "CRITICAL") == 0) return LOG_CRITICAL;
    return -1;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[512];
    char level_str[16];
    
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    if (sscanf(line, "%19s %15s %255[^\n]", 
               entry->timestamp, level_str, entry->message) != 3) {
        return -1;
    }
    
    int level = parse_log_level(level_str);
    if (level == -1) {
        return -1;
    }
    entry->level = (enum LogLevel)level;
    
    return 1;
}

void analyze_log_level(struct LogEntry* entries, int count, enum LogLevel target_level) {
    int occurrences = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            occurrences++;
        }
    }
    
    printf("Found %d occurrences of %s level\n", occurrences, level_to_string(target_level));
}

void print_log_summary(struct LogEntry* entries, int count) {
    int level_counts[5] = {0};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= LOG_DEBUG && entries[i].level <= LOG_CRITICAL) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Summary:\n");
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        printf("%s: %d entries\n", level_to_string(level), level_counts[level]);
    }
}

int main(void) {
    struct LogEntry entries[1000];
    int entry_count = 0;
    char filename[256];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Filename cannot be empty\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    while (entry_count < 1000) {
        int result = read_log_entry(file, &entries[entry_count]);
        if (result == 0) {
            break;
        } else if (result == 1) {
            entry_count++;
        }
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    printf("Successfully read %d log entries\n", entry_count);
    
    print_log_summary(entries, entry_count);
    
    printf("\nAnalyze specific log level (DEBUG/INFO/WARNING/ERROR/CRITICAL): ");
    char level_choice[16];
    if (fgets(level_choice, sizeof(level_choice), stdin) == NULL) {
        return 1;
    }
    
    len = strlen(level_choice);
    if (len > 0 && level_choice[len - 1] == '\n') {
        level_choice[len - 1] = '\0';
    }
    
    for (int i = 0; level_choice[i]; i++) {
        level_choice[i] = toupper(level_choice[i]);
    }
    
    int target_level = parse_log_level(level_choice);
    if (target_level == -1) {
        printf("Invalid log level specified\n");
        return 1;
    }
    
    analyze_log_level(entries, entry_count, (enum LogLevel)target_level);
    
    return 0;
}
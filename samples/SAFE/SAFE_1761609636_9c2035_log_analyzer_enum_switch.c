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
    
    entry->level = level;
    return 1;
}

void analyze_log_level(enum LogLevel level, int* counts) {
    if (level >= LOG_DEBUG && level <= LOG_CRITICAL) {
        counts[level]++;
    }
}

void print_analysis(int* counts) {
    printf("\nLog Level Analysis:\n");
    printf("==================\n");
    
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        printf("%-8s: %d entries\n", level_to_string(level), counts[level]);
    }
    
    int total = 0;
    for (int i = 0; i <= LOG_CRITICAL; i++) {
        total += counts[i];
    }
    
    if (total > 0) {
        printf("\nTotal entries: %d\n", total);
        printf("Error rate: %.1f%%\n", 
               (float)(counts[LOG_ERROR] + counts[LOG_CRITICAL]) * 100.0 / total);
    }
}

int main(void) {
    struct LogEntry entry;
    int level_counts[LOG_CRITICAL + 1] = {0};
    char filename[256];
    FILE* file;
    
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
    
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    printf("Reading log file: %s\n", filename);
    
    int entry_count = 0;
    int valid_entries = 0;
    
    while (1) {
        int result = read_log_entry(file, &entry);
        if (result == 0) break;
        
        entry_count++;
        
        if (result == 1) {
            valid_entries++;
            analyze_log_level(entry.level, level_counts);
            
            switch (entry.level) {
                case LOG_ERROR:
                case LOG_CRITICAL:
                    printf("ALERT: %s - %s\n", entry.timestamp, entry.message);
                    break;
                case LOG_WARNING:
                    printf("Warning: %s - %s\n", entry.timestamp, entry.message);
                    break;
                default:
                    break;
            }
        } else {
            fprintf(stderr, "Invalid log entry at line %d\n", entry_count);
        }
    }
    
    fclose(file);
    
    if (valid_entries == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    print_analysis(level_counts);
    
    return 0;
}
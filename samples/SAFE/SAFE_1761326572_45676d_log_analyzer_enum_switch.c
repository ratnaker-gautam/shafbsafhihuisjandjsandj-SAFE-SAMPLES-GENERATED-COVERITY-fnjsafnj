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
    switch (level) {
        case LOG_DEBUG:
            counts[0]++;
            break;
        case LOG_INFO:
            counts[1]++;
            break;
        case LOG_WARNING:
            counts[2]++;
            break;
        case LOG_ERROR:
            counts[3]++;
            break;
        case LOG_CRITICAL:
            counts[4]++;
            break;
        default:
            break;
    }
}

void print_analysis(int* counts) {
    printf("Log Analysis Summary:\n");
    printf("DEBUG: %d entries\n", counts[0]);
    printf("INFO: %d entries\n", counts[1]);
    printf("WARNING: %d entries\n", counts[2]);
    printf("ERROR: %d entries\n", counts[3]);
    printf("CRITICAL: %d entries\n", counts[4]);
    
    int total = counts[0] + counts[1] + counts[2] + counts[3] + counts[4];
    if (total > 0) {
        printf("Total entries: %d\n", total);
    }
}

int main(void) {
    char filename[256];
    printf("Enter log file name: ");
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }
    
    int level_counts[5] = {0};
    struct LogEntry entry;
    int valid_entries = 0;
    int invalid_entries = 0;
    
    while (1) {
        int result = read_log_entry(file, &entry);
        if (result == 0) break;
        if (result == -1) {
            invalid_entries++;
            continue;
        }
        
        analyze_log_level(entry.level, level_counts);
        valid_entries++;
    }
    
    fclose(file);
    
    print_analysis(level_counts);
    
    if (invalid_entries > 0) {
        printf("Skipped %d invalid entries\n", invalid_entries);
    }
    
    if (valid_entries == 0) {
        printf("No valid log entries found\n");
    }
    
    return 0;
}
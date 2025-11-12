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

void analyze_log_levels(int counts[5]) {
    printf("\nLog Level Analysis:\n");
    printf("==================\n");
    
    for (int i = 0; i < 5; i++) {
        const char* level_name = level_to_string((enum LogLevel)i);
        printf("%-8s: %d entries\n", level_name, counts[i]);
    }
    
    int total = 0;
    for (int i = 0; i < 5; i++) {
        total += counts[i];
    }
    
    if (total > 0) {
        printf("\nDistribution:\n");
        for (int i = 0; i < 5; i++) {
            float percentage = (counts[i] * 100.0f) / total;
            const char* level_name = level_to_string((enum LogLevel)i);
            printf("%-8s: %.1f%%\n", level_name, percentage);
        }
    }
}

void process_log_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file '%s'\n", filename);
        return;
    }
    
    int level_counts[5] = {0};
    struct LogEntry entry;
    int entry_count = 0;
    int error_count = 0;
    
    printf("Processing log file: %s\n", filename);
    printf("====================\n");
    
    while (1) {
        int result = read_log_entry(file, &entry);
        if (result == 0) break;
        
        if (result == -1) {
            error_count++;
            continue;
        }
        
        entry_count++;
        
        switch (entry.level) {
            case LOG_DEBUG:
                level_counts[LOG_DEBUG]++;
                break;
            case LOG_INFO:
                level_counts[LOG_INFO]++;
                break;
            case LOG_WARNING:
                level_counts[LOG_WARNING]++;
                printf("Warning: %s - %s\n", entry.timestamp, entry.message);
                break;
            case LOG_ERROR:
                level_counts[LOG_ERROR]++;
                printf("Error: %s - %s\n", entry.timestamp, entry.message);
                break;
            case LOG_CRITICAL:
                level_counts[LOG_CRITICAL]++;
                printf("CRITICAL: %s - %s\n", entry.timestamp, entry.message);
                break;
            default:
                break;
        }
    }
    
    fclose(file);
    
    printf("\nSummary:\n");
    printf("========\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Malformed entries: %d\n", error_count);
    
    analyze_log_levels(level_counts);
}

int main() {
    char filename[256];
    
    printf("Log File Analyzer\n");
    printf("=================\n");
    printf("Enter log file name: ");
    
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Error: No filename provided\n");
        return 1;
    }
    
    process_log_file(filename);
    
    return 0;
}
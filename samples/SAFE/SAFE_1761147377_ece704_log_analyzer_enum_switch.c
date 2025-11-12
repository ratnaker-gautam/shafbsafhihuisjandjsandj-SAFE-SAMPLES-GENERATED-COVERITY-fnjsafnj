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
    const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    printf("Log Level Analysis:\n");
    printf("===================\n");
    
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d entries\n", level_names[i], counts[i]);
    }
    
    int total = 0;
    for (int i = 0; i < 5; i++) {
        total += counts[i];
    }
    printf("Total   : %d entries\n", total);
}

int main(void) {
    char filename[256];
    FILE* file;
    struct LogEntry entry;
    int counts[5] = {0};
    int entry_count = 0;
    
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
    
    while (1) {
        int result = read_log_entry(file, &entry);
        if (result == 0) {
            break;
        }
        if (result == -1) {
            fprintf(stderr, "Error parsing log entry at line %d\n", entry_count + 1);
            fclose(file);
            return 1;
        }
        
        analyze_log_level(entry.level, counts);
        entry_count++;
        
        if (entry_count >= 1000) {
            fprintf(stderr, "Too many log entries (max 1000)\n");
            break;
        }
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    print_analysis(counts);
    
    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_UNKNOWN
};

struct LogEntry {
    char timestamp[20];
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "DEBUG") == 0) return LOG_DEBUG;
    return LOG_UNKNOWN;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_DEBUG: return "DEBUG";
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
        return 0;
    }
    
    entry->level = parse_log_level(level_str);
    return 1;
}

int main(void) {
    FILE* file;
    char filename[256];
    struct LogEntry entry;
    int counts[5] = {0};
    int total_entries = 0;
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }
    
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    while (read_log_entry(file, &entry)) {
        if (entry.level >= LOG_INFO && entry.level <= LOG_UNKNOWN) {
            counts[entry.level]++;
            total_entries++;
        }
    }
    
    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
        fclose(file);
        return 1;
    }
    
    fclose(file);
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", total_entries);
    printf("\nBreakdown by level:\n");
    
    for (enum LogLevel level = LOG_INFO; level <= LOG_UNKNOWN; level++) {
        const char* level_name = level_to_string(level);
        int count = counts[level];
        double percentage = total_entries > 0 ? (count * 100.0) / total_entries : 0.0;
        
        printf("%-8s: %3d entries (%5.1f%%)\n", level_name, count, percentage);
    }
    
    printf("\nDetailed analysis:\n");
    for (enum LogLevel level = LOG_INFO; level <= LOG_UNKNOWN; level++) {
        switch (level) {
            case LOG_INFO:
                printf("INFO messages: General operational information\n");
                break;
            case LOG_WARNING:
                printf("WARNING messages: Potential issues that don't stop execution\n");
                break;
            case LOG_ERROR:
                printf("ERROR messages: Serious problems that affect functionality\n");
                break;
            case LOG_DEBUG:
                printf("DEBUG messages: Detailed information for troubleshooting\n");
                break;
            case LOG_UNKNOWN:
                printf("UNKNOWN messages: Unrecognized log levels\n");
                break;
        }
    }
    
    if (counts[LOG_ERROR] > 0) {
        printf("\nAlert: %d error(s) detected in the log!\n", counts[LOG_ERROR]);
    }
    
    if (counts[LOG_WARNING] > 10) {
        printf("Note: High number of warnings (%d) may indicate issues\n", counts[LOG_WARNING]);
    }
    
    return 0;
}
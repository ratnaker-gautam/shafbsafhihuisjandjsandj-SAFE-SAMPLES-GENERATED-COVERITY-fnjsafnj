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

int is_valid_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[512];
    char level_str[16];
    
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    if (sscanf(line, "%19s %15s %255[^\n]", 
               entry->timestamp, level_str, entry->message) != 3) {
        return 0;
    }
    
    if (!is_valid_timestamp(entry->timestamp)) return 0;
    
    int level = parse_log_level(level_str);
    if (level == -1) return 0;
    
    entry->level = level;
    return 1;
}

void analyze_log_levels(int counts[5]) {
    printf("\nLog Level Analysis:\n");
    printf("==================\n");
    
    for (int i = 0; i < 5; i++) {
        const char* level_name = level_to_string(i);
        printf("%-8s: %d entries\n", level_name, counts[i]);
    }
}

void process_log_entry(const struct LogEntry* entry, int counts[5]) {
    counts[entry->level]++;
    
    switch (entry->level) {
        case LOG_ERROR:
        case LOG_CRITICAL:
            printf("ALERT: %s - %s: %s\n", 
                   entry->timestamp, level_to_string(entry->level), entry->message);
            break;
        case LOG_WARNING:
            printf("NOTICE: %s - %s: %s\n", 
                   entry->timestamp, level_to_string(entry->level), entry->message);
            break;
        case LOG_INFO:
        case LOG_DEBUG:
        default:
            break;
    }
}

int main(void) {
    char filename[256];
    FILE* file;
    struct LogEntry entry;
    int level_counts[5] = {0};
    int total_entries = 0;
    
    printf("Enter log file name: ");
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }
    
    printf("Processing log file: %s\n", filename);
    printf("==================\n");
    
    while (read_log_entry(file, &entry)) {
        process_log_entry(&entry, level_counts);
        total_entries++;
        
        if (total_entries >= 1000) {
            printf("Warning: Reached maximum of 1000 entries\n");
            break;
        }
    }
    
    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
        fclose(file);
        return 1;
    }
    
    fclose(file);
    
    printf("\nTotal entries processed: %d\n", total_entries);
    analyze_log_levels(level_counts);
    
    return 0;
}
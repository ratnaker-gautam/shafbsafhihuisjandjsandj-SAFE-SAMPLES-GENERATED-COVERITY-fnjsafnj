//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
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

void analyze_log_level(FILE* file, enum LogLevel filter_level) {
    struct LogEntry entry;
    int count = 0;
    int total = 0;
    
    rewind(file);
    
    while (read_log_entry(file, &entry) > 0) {
        total++;
        if (entry.level == filter_level) {
            count++;
        }
    }
    
    if (total > 0) {
        double percentage = (double)count / total * 100.0;
        printf("Level %s: %d entries (%.1f%% of total)\n", 
               level_to_string(filter_level), count, percentage);
    } else {
        printf("Level %s: 0 entries (0.0%% of total)\n", 
               level_to_string(filter_level));
    }
}

void process_log_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file '%s'\n", filename);
        return;
    }
    
    printf("Log Analysis for: %s\n", filename);
    printf("====================\n");
    
    analyze_log_level(file, LOG_DEBUG);
    analyze_log_level(file, LOG_INFO);
    analyze_log_level(file, LOG_WARNING);
    analyze_log_level(file, LOG_ERROR);
    analyze_log_level(file, LOG_CRITICAL);
    
    fclose(file);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    if (strlen(argv[1]) == 0) {
        printf("Error: Empty filename\n");
        return 1;
    }
    
    process_log_file(argv[1]);
    return 0;
}
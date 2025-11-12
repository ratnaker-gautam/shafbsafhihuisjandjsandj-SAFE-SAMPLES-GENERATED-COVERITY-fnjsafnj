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

void analyze_logs(const char* filename, enum LogLevel min_level) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return;
    }
    
    struct LogEntry entry;
    int total_entries = 0;
    int level_counts[5] = {0};
    int filtered_count = 0;
    
    while (1) {
        int result = read_log_entry(file, &entry);
        if (result == 0) break;
        if (result == -1) continue;
        
        total_entries++;
        
        if (entry.level < 0 || entry.level > LOG_CRITICAL) {
            continue;
        }
        
        level_counts[entry.level]++;
        
        if (entry.level >= min_level) {
            filtered_count++;
            printf("%s [%s] %s\n", 
                   entry.timestamp, 
                   level_to_string(entry.level), 
                   entry.message);
        }
    }
    
    fclose(file);
    
    printf("\nSummary:\n");
    printf("Total entries processed: %d\n", total_entries);
    printf("Entries at or above %s level: %d\n", 
           level_to_string(min_level), filtered_count);
    printf("Breakdown by level:\n");
    
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        printf("  %s: %d\n", level_to_string(level), level_counts[level]);
    }
}

int main() {
    char filename[256];
    char level_input[16];
    enum LogLevel min_level;
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Error: Filename cannot be empty\n");
        return 1;
    }
    
    printf("Enter minimum log level (DEBUG/INFO/WARNING/ERROR/CRITICAL): ");
    if (fgets(level_input, sizeof(level_input), stdin) == NULL) {
        printf("Error reading log level\n");
        return 1;
    }
    
    len = strlen(level_input);
    if (len > 0 && level_input[len-1] == '\n') {
        level_input[len-1] = '\0';
    }
    
    for (int i = 0; level_input[i]; i++) {
        level_input[i] = toupper(level_input[i]);
    }
    
    int level = parse_log_level(level_input);
    if (level == -1) {
        printf("Error: Invalid log level\n");
        return 1;
    }
    min_level = (enum LogLevel)level;
    
    analyze_logs(filename, min_level);
    
    return 0;
}
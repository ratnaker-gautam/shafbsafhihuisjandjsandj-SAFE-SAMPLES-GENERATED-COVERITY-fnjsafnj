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

void analyze_log_level(enum LogLevel level, int* counts) {
    if (level >= LOG_DEBUG && level <= LOG_CRITICAL) {
        counts[level]++;
    }
}

void print_statistics(int* counts, int total) {
    printf("\nLog Statistics:\n");
    printf("Total entries: %d\n", total);
    
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        double percentage = total > 0 ? (counts[level] * 100.0) / total : 0.0;
        printf("%-8s: %d (%.1f%%)\n", level_to_string(level), counts[level], percentage);
    }
}

int main(void) {
    struct LogEntry entries[1000];
    int counts[5] = {0};
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
    
    printf("Reading log entries...\n");
    
    while (entry_count < 1000) {
        struct LogEntry entry;
        int result = read_log_entry(file, &entry);
        
        if (result == 0) {
            break;
        }
        
        if (result == -1) {
            fprintf(stderr, "Error parsing log entry %d\n", entry_count + 1);
            continue;
        }
        
        entries[entry_count] = entry;
        analyze_log_level(entry.level, counts);
        entry_count++;
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    print_statistics(counts, entry_count);
    
    enum LogLevel filter_level;
    char filter_str[16];
    
    printf("\nEnter level to filter (DEBUG/INFO/WARNING/ERROR/CRITICAL): ");
    if (fgets(filter_str, sizeof(filter_str), stdin) == NULL) {
        return 1;
    }
    
    len = strlen(filter_str);
    if (len > 0 && filter_str[len - 1] == '\n') {
        filter_str[len - 1] = '\0';
    }
    
    int filter = parse_log_level(filter_str);
    if (filter == -1) {
        printf("Invalid log level\n");
        return 1;
    }
    
    filter_level = (enum LogLevel)filter;
    
    printf("\nEntries with level %s:\n", level_to_string(filter_level));
    int found = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == filter_level) {
            printf("%s %s %s\n", entries[i].timestamp, 
                   level_to_string(entries[i].level), entries[i].message);
            found++;
        }
    }
    
    if (found == 0
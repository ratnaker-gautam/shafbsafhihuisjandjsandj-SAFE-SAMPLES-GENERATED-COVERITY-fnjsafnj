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
        const char* level_name = level_to_string(level);
        printf("%-8s: %d entries\n", level_name, counts[level]);
    }
}

int main(void) {
    struct LogEntry entries[1000];
    int counts[5] = {0};
    int entry_count = 0;
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
    
    while (entry_count < 1000) {
        struct LogEntry entry;
        int result = read_log_entry(file, &entry);
        
        if (result == 0) {
            break;
        }
        
        if (result == -1) {
            fprintf(stderr, "Error parsing log entry at line %d\n", entry_count + 1);
            fclose(file);
            return 1;
        }
        
        entries[entry_count] = entry;
        analyze_log_level(entry.level, counts);
        entry_count++;
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    printf("Successfully processed %d log entries.\n", entry_count);
    
    print_analysis(counts);
    
    enum LogLevel filter_level;
    char filter_input[16];
    
    printf("\nEnter log level to filter (DEBUG/INFO/WARNING/ERROR/CRITICAL): ");
    if (scanf("%15s", filter_input) != 1) {
        fprintf(stderr, "Error reading filter level\n");
        return 1;
    }
    
    filter_level = parse_log_level(filter_input);
    if (filter_level == -1) {
        fprintf(stderr, "Invalid log level: %s\n", filter_input);
        return 1;
    }
    
    printf("\nEntries with level %s:\n", level_to_string(filter_level));
    printf("====================\n");
    
    int found = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == filter_level) {
            printf("%s %s %s\n", entries[i].timestamp, 
                   level_to_string(entries[i].level), entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No entries found with specified level.\n");
    }
    
    return 0;
}
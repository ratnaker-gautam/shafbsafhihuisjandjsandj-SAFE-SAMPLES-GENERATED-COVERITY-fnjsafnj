//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[64];
    char level[16];
    char message[256];
};

int parse_timestamp(const char* ts_str, struct tm* tm) {
    return strptime(ts_str, "%Y-%m-%d %H:%M:%S", tm) != NULL;
}

int validate_log_level(const char* level) {
    const char* valid_levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    for (size_t i = 0; i < 5; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
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
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        struct LogEntry entry;
        struct tm time_info;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            continue;
        }
        
        int parsed = sscanf(line, "%63s %15s %255[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed != 3) {
            continue;
        }
        
        if (!parse_timestamp(entry.timestamp, &time_info)) {
            continue;
        }
        
        if (!validate_log_level(entry.level)) {
            continue;
        }
        
        if (strlen(entry.message) == 0) {
            continue;
        }
        
        logs[entry_count] = entry;
        entry_count++;
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    int level_counts[5] = {0};
    const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < entry_count; i++) {
        for (int j = 0; j < 5; j++) {
            if (strcmp(logs[i].level, level_names[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("\nBreakdown by level:\n");
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d\n", level_names[i], level_counts[i]);
    }
    
    char search_level[16];
    printf("\nEnter log level to search for (or 'all' for all entries): ");
    if (fgets(search_level, sizeof(search_level), stdin) == NULL) {
        return 1;
    }
    
    search_level[strcspn(search_level, "\n")] = '\0';
    
    printf("\nMatching entries:\n");
    int found = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(search_level, "all") == 0 || 
            strcmp(logs[i].level, search_level) == 0) {
            printf("[%s] %s: %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No matching entries found\n");
    }
    
    return 0;
}
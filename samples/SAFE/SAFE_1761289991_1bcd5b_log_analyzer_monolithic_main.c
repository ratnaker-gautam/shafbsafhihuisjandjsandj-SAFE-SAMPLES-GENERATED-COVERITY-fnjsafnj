//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char* parts[3];
    parts[0] = strtok(temp, "|");
    parts[1] = strtok(NULL, "|");
    parts[2] = strtok(NULL, "\n");
    
    if (parts[0] == NULL || parts[1] == NULL || parts[2] == NULL) return 0;
    
    if (strlen(parts[0]) >= 32 || strlen(parts[1]) >= 16 || strlen(parts[2]) >= 256) return 0;
    
    strcpy(entry->timestamp, parts[0]);
    strcpy(entry->level, parts[1]);
    strcpy(entry->message, parts[2]);
    
    return 1;
}

int is_valid_level(const char* level) {
    if (level == NULL) return 0;
    return (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || 
            strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char filename[256];
    char filter_level[16] = "";
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading filename\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    printf("Filter by level (INFO/WARN/ERROR/DEBUG or empty for all): ");
    if (fgets(filter_level, sizeof(filter_level), stdin) == NULL) {
        printf("Error reading filter\n");
        return 1;
    }
    filter_level[strcspn(filter_level, "\n")] = '\0';
    
    if (strlen(filter_level) > 0 && !is_valid_level(filter_level)) {
        printf("Invalid log level filter\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return 1;
    }
    
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                entries[entry_count] = entry;
                entry_count++;
            }
        }
    }
    
    fclose(file);
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (strlen(filter_level) > 0) {
        printf("\nFiltered entries (%s):\n", filter_level);
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, filter_level) == 0) {
                printf("%s | %s | %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
    } else {
        printf("\nAll entries:\n");
        for (int i = 0; i < entry_count; i++) {
            printf("%s | %s | %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    return 0;
}
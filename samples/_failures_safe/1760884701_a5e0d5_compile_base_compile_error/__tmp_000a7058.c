//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
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

int parse_timestamp(const char* ts_str, struct tm* tm) {
    return sscanf(ts_str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 ||
           strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    FILE* file;
    char filename[256];
    int choice;
    int i;
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
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
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        struct LogEntry entry;
        struct tm tm;
        char level_buf[16];
        char message_buf[256];
        char timestamp_buf[32];
        
        if (sscanf(line, "%31s %15s %255[^\n]", timestamp_buf, level_buf, message_buf) != 3) {
            continue;
        }
        
        if (!parse_timestamp(timestamp_buf, &tm)) {
            continue;
        }
        
        if (!is_valid_level(level_buf)) {
            continue;
        }
        
        if (strlen(message_buf) == 0) {
            continue;
        }
        
        strncpy(entry.timestamp, timestamp_buf, sizeof(entry.timestamp)-1);
        entry.timestamp[sizeof(entry.timestamp)-1] = '\0';
        strncpy(entry.level, level_buf, sizeof(entry.level)-1);
        entry.level[sizeof(entry.level)-1] = '\0';
        strncpy(entry.message, message_buf, sizeof(entry.message)-1);
        entry.message[sizeof(entry.message)-1] = '\0';
        
        entries[entry_count] = entry;
        entry_count++;
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    for (i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    printf("\n1. Show all entries\n");
    printf("2. Show errors only\n");
    printf("3. Show warnings and errors\n");
    printf("Enter choice: ");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    printf("\n");
    for (i = 0; i < entry_count; i++) {
        int show = 0;
        
        switch (choice) {
            case 1: show = 1; break;
            case 2: show = (strcmp(entries[i].level, "ERROR") == 0); break;
            case 3:
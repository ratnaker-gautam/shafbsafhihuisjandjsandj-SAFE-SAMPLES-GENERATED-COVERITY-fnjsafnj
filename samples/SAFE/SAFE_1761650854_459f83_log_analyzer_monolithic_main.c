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
    char message[MAX_LINE_LEN];
};

int parse_timestamp(const char* ts_str, struct tm* tm) {
    return sscanf(ts_str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int compare_entries(const void* a, const void* b) {
    const struct LogEntry* ea = (const struct LogEntry*)a;
    const struct LogEntry* eb = (const struct LogEntry*)b;
    struct tm ta, tb;
    
    if (!parse_timestamp(ea->timestamp, &ta) || !parse_timestamp(eb->timestamp, &tb)) {
        return 0;
    }
    
    ta.tm_year -= 1900;
    ta.tm_mon -= 1;
    tb.tm_year -= 1900;
    tb.tm_mon -= 1;
    
    time_t time_a = mktime(&ta);
    time_t time_b = mktime(&tb);
    
    if (time_a < time_b) return -1;
    if (time_a > time_b) return 1;
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
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
        char level[16];
        char timestamp[32];
        char message[MAX_LINE_LEN];
        
        if (sscanf(line, "%31s %15s %1023[^\n]", timestamp, level, message) == 3) {
            if (strlen(timestamp) < sizeof(entry.timestamp) &&
                strlen(level) < sizeof(entry.level) &&
                strlen(message) < sizeof(entry.message)) {
                
                strcpy(entry.timestamp, timestamp);
                strcpy(entry.level, level);
                strcpy(entry.message, message);
                
                entries[entry_count] = entry;
                entry_count++;
            }
        }
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    qsort(entries, entry_count, sizeof(struct LogEntry), compare_entries);
    
    printf("\nSorted log entries:\n");
    printf("===================\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("\nSummary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
    
    return 0;
}
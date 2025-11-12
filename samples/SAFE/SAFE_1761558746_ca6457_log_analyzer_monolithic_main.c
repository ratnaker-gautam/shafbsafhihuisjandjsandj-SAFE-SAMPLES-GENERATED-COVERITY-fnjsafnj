//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
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

int compare_entries(const void* a, const void* b) {
    const struct LogEntry* entry_a = (const struct LogEntry*)a;
    const struct LogEntry* entry_b = (const struct LogEntry*)b;
    struct tm tm_a, tm_b;
    
    if (parse_timestamp(entry_a->timestamp, &tm_a) && parse_timestamp(entry_b->timestamp, &tm_b)) {
        time_t time_a = mktime(&tm_a);
        time_t time_b = mktime(&tm_b);
        if (time_a < time_b) return -1;
        if (time_a > time_b) return 1;
    }
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    char filename[256];
    FILE* file;
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
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
        fprintf(stderr, "Empty filename\n");
        return 1;
    }
    
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        struct LogEntry entry;
        char ts[32], lvl[16], msg[256];
        
        if (sscanf(line, "%31[^|]|%15[^|]|%255[^\n]", ts, lvl, msg) == 3) {
            if (strlen(ts) < sizeof(entry.timestamp) && 
                strlen(lvl) < sizeof(entry.level) && 
                strlen(msg) < sizeof(entry.message)) {
                
                strcpy(entry.timestamp, ts);
                strcpy(entry.level, lvl);
                strcpy(entry.message, msg);
                
                if (is_valid_level(entry.level)) {
                    entries[entry_count++] = entry;
                    
                    if (strcmp(entry.level, "INFO") == 0) info_count++;
                    else if (strcmp(entry.level, "WARN") == 0) warn_count++;
                    else if (strcmp(entry.level, "ERROR") == 0) error_count++;
                    else if (strcmp(entry.level, "DEBUG") == 0) debug_count++;
                }
            }
        }
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    qsort(entries, entry_count, sizeof(struct LogEntry), compare_entries);
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO:  %d\n", info_count);
    printf("WARN:  %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    printf("\nSorted Log Entries:\n");
    printf("==================\n");
    for (int i = 0; i < entry_count; i++) {
        printf("%s | %s | %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}
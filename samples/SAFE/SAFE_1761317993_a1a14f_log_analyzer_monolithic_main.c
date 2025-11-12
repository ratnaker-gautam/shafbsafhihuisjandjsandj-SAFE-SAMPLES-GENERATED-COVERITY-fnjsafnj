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
    
    if (time_a == (time_t)-1 || time_b == (time_t)-1) {
        return 0;
    }
    
    if (time_a < time_b) return -1;
    if (time_a > time_b) return 1;
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\\n");
    printf("Enter 'END' on a separate line to finish input\\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\\n' || line[3] == '\\0')) {
            break;
        }
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\\n]", timestamp, level, message);
        
        if (parsed < 3) {
            printf("Invalid format. Skipping line.\\n");
            continue;
        }
        
        if (strlen(timestamp) != 19 || timestamp[4] != '-' || timestamp[7] != '-' ||
            timestamp[10] != ' ' || timestamp[13] != ':' || timestamp[16] != ':') {
            printf("Invalid timestamp format. Skipping.\\n");
            continue;
        }
        
        struct tm tm_check;
        if (!parse_timestamp(timestamp, &tm_check)) {
            printf("Invalid timestamp values. Skipping.\\n");
            continue;
        }
        
        if (strcmp(level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(level, "INFO") == 0) {
            info_count++;
        } else {
            printf("Unknown log level: %s. Skipping.\\n", level);
            continue;
        }
        
        strncpy(entries[entry_count].timestamp, timestamp, sizeof(entries[entry_count].timestamp) - 1);
        strncpy(entries[entry_count].level, level, sizeof(entries[entry_count].level) - 1);
        strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\\n");
        return 0;
    }
    
    qsort(entries, entry_count, sizeof(struct LogEntry), compare_entries);
    
    printf("\\n=== Log Analysis Results ===\\n");
    printf("Total entries: %d\\n", entry_count);
    printf("ERROR entries: %d\\n", error_count);
    printf("WARNING entries: %d\\n", warning_count);
    printf("INFO entries: %d\\n", info_count);
    
    printf("\\n=== Sorted Log Entries ===\\n");
    for (int i = 0; i < entry_count; i++) {
        printf("%s %s %s\\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    if (error_count > 0) {
        printf("\\n=== ERROR Entries Summary ===\\n");
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("%s: %s\\n", entries[i].timestamp, entries[i].message);
            }
        }
    }
    
    return 0;
}
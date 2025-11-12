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
    const struct LogEntry* entry_a = (const struct LogEntry*)a;
    const struct LogEntry* entry_b = (const struct LogEntry*)b;
    struct tm tm_a, tm_b;
    
    if (!parse_timestamp(entry_a->timestamp, &tm_a)) return 0;
    if (!parse_timestamp(entry_b->timestamp, &tm_b)) return 0;
    
    time_t time_a = mktime(&tm_a);
    time_t time_b = mktime(&tm_b);
    
    if (time_a < time_b) return -1;
    if (time_a > time_b) return 1;
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL Message)\\n");
    printf("Enter 'END' on a separate line to finish input\\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry* entry = &entries[entry_count];
        char level[16];
        char message[MAX_LINE_LEN];
        int year, month, day, hour, min, sec;
        
        int parsed = sscanf(line, "%d-%d-%d %d:%d:%d %15s %1023[^\n]",
                           &year, &month, &day, &hour, &min, &sec,
                           level, message);
        
        if (parsed < 7) {
            printf("Invalid format. Skipping.\\n");
            continue;
        }
        
        if (year < 2000 || year > 2100 || month < 1 || month > 12 ||
            day < 1 || day > 31 || hour < 0 || hour > 23 ||
            min < 0 || min > 59 || sec < 0 || sec > 59) {
            printf("Invalid timestamp. Skipping.\\n");
            continue;
        }
        
        snprintf(entry->timestamp, sizeof(entry->timestamp),
                "%04d-%02d-%02d %02d:%02d:%02d",
                year, month, day, hour, min, sec);
        
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\\0';
        
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\\0';
        
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\\n");
        return 0;
    }
    
    qsort(entries, entry_count, sizeof(struct LogEntry), compare_entries);
    
    printf("\\nSorted log entries:\\n");
    printf("====================\\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        printf("%s [%s] %s\\n", entries[i].timestamp, entries[i].level, entries[i].message);
        
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("\\nSummary:\\n");
    printf("Total entries: %d\\n", entry_count);
    printf("ERROR: %d\\n", error_count);
    printf("WARNING: %d\\n", warning_count);
    printf("INFO: %d\\n", info_count);
    
    return 0;
}
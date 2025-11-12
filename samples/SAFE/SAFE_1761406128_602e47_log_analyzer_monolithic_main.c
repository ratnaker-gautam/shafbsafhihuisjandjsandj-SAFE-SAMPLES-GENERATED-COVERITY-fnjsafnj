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
    char message[MAX_LINE_LENGTH];
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
    if (!parse_timestamp(entry_a->timestamp, &tm_a) ||
        !parse_timestamp(entry_b->timestamp, &tm_b)) {
        return 0;
    }
    
    time_t time_a = mktime(&tm_a);
    time_t time_b = mktime(&tm_b);
    
    if (time_a < time_b) return -1;
    if (time_a > time_b) return 1;
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry* entry = &entries[entry_count];
        int scanned = sscanf(line, "%31s %15s %1023[^\n]",
                           entry->timestamp, entry->level, entry->message);
        
        if (scanned != 3) {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
            continue;
        }
        
        struct tm tm_check;
        if (!parse_timestamp(entry->timestamp, &tm_check)) {
            printf("Invalid timestamp format\n");
            continue;
        }
        
        if (tm_check.tm_year < 2000 || tm_check.tm_year > 2100 ||
            tm_check.tm_mon < 1 || tm_check.tm_mon > 12 ||
            tm_check.tm_mday < 1 || tm_check.tm_mday > 31 ||
            tm_check.tm_hour < 0 || tm_check.tm_hour > 23 ||
            tm_check.tm_min < 0 || tm_check.tm_min > 59 ||
            tm_check.tm_sec < 0 || tm_check.tm_sec > 59) {
            printf("Invalid timestamp values\n");
            continue;
        }
        
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze\n");
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
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (entry_count > 0) {
        struct tm first_tm, last_tm;
        if (parse_timestamp(entries[0].timestamp, &first_tm) &&
            parse_timestamp(entries[entry_count-1].timestamp, &last_tm)) {
            time_t first_time = mktime(&first_tm);
            time_t last_time = mktime(&last_tm);
            double hours = difftime(last_time, first_time) / 3600.0;
            printf("Time span: %.2f hours\n", hours);
        }
    }
    
    return 0;
}
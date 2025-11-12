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
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\\n");
    printf("Enter 'END' on a separate line to finish input\\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        int parsed = sscanf(line, "%31s %15s %1023[^\n]",
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed != 3) {
            printf("Invalid format. Skipping line.\\n");
            continue;
        }
        
        struct tm tm_check;
        if (!parse_timestamp(entry.timestamp, &tm_check)) {
            printf("Invalid timestamp format. Skipping line.\\n");
            continue;
        }
        
        if (strlen(entry.level) == 0 || strlen(entry.message) == 0) {
            printf("Missing level or message. Skipping line.\\n");
            continue;
        }
        
        entries[entry_count] = entry;
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to process.\\n");
        return 0;
    }
    
    qsort(entries, entry_count, sizeof(struct LogEntry), compare_entries);
    
    printf("\\nSorted log entries:\\n");
    printf("===================\\n");
    
    for (int i = 0; i < entry_count; i++) {
        printf("%s %s %s\\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("\\nSummary:\\n");
    printf("========\\n");
    printf("Total entries: %d\\n", entry_count);
    printf("ERROR entries: %d\\n", error_count);
    printf("WARNING entries: %d\\n", warning_count);
    printf("INFO entries: %d\\n", info_count);
    
    return 0;
}
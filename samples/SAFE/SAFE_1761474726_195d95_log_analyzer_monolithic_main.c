//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || 
           strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    int info_count = 0, warn_count = 0, error_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\\n");
    printf("Enter 'END' on a separate line to finish input\\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\\n' || line[3] == '\\0')) {
            break;
        }
        
        line[strcspn(line, "\\n")] = 0;
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        struct tm timestamp;
        
        int parsed = sscanf(line, "%31s %15s %1023[^\\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed != 3) {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL message\\n");
            continue;
        }
        
        if (!parse_timestamp(entry.timestamp, &timestamp)) {
            printf("Invalid timestamp format\\n");
            continue;
        }
        
        if (!is_valid_level(entry.level)) {
            printf("Invalid log level. Use INFO, WARN, or ERROR\\n");
            continue;
        }
        
        if (strlen(entry.message) == 0) {
            printf("Message cannot be empty\\n");
            continue;
        }
        
        entries[entry_count] = entry;
        entry_count++;
        
        if (strcmp(entry.level, "INFO") == 0) info_count++;
        else if (strcmp(entry.level, "WARN") == 0) warn_count++;
        else if (strcmp(entry.level, "ERROR") == 0) error_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided\\n");
        return 1;
    }
    
    printf("\\n=== Log Analysis Results ===\\n");
    printf("Total entries: %d\\n", entry_count);
    printf("INFO: %d\\n", info_count);
    printf("WARN: %d\\n", warn_count);
    printf("ERROR: %d\\n", error_count);
    
    if (error_count > 0) {
        printf("\\n=== ERROR Entries ===\\n");
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("%s %s %s\\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
    }
    
    time_t earliest = 0, latest = 0;
    for (int i = 0; i < entry_count; i++) {
        struct tm tm;
        if (parse_timestamp(entries[i].timestamp, &tm)) {
            tm.tm_year -= 1900;
            tm.tm_mon -= 1;
            time_t t = mktime(&tm);
            if (t != (time_t)-1) {
                if (i == 0 || t < earliest) earliest = t;
                if (i == 0 || t > latest) latest = t;
            }
        }
    }
    
    if (earliest != 0 && latest != 0) {
        double span_seconds = difftime(latest, earliest);
        printf("\\nTime span: %.0f seconds\\n", span_seconds);
    }
    
    return 0;
}
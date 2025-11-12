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
    
    if (time_a == -1 || time_b == -1) {
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
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE)\\n");
    printf("Enter 'END' on a separate line to finish input\\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\\n' || line[3] == '\\0')) {
            break;
        }
        
        line[strcspn(line, "\\n")] = '\\0';
        
        if (strlen(line) == 0) {
            continue;
        }
        
        char timestamp[32];
        char level[16];
        char message[256];
        
        int parsed = sscanf(line, "%31s %15s %255[^\\n]", timestamp, level, message);
        
        if (parsed < 3) {
            printf("Invalid format. Skipping.\\n");
            continue;
        }
        
        struct tm tm_check;
        if (!parse_timestamp(timestamp, &tm_check)) {
            printf("Invalid timestamp format. Skipping.\\n");
            continue;
        }
        
        if (strlen(level) >= sizeof(entries[entry_count].level) ||
            strlen(message) >= sizeof(entries[entry_count].message)) {
            printf("Entry too long. Skipping.\\n");
            continue;
        }
        
        strcpy(entries[entry_count].timestamp, timestamp);
        strcpy(entries[entry_count].level, level);
        strcpy(entries[entry_count].message, message);
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\\n");
        return 1;
    }
    
    qsort(entries, entry_count, sizeof(struct LogEntry), compare_entries);
    
    printf("\\nSorted log entries:\\n");
    printf("===================\\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        printf("%s %s %s\\n", entries[i].timestamp, entries[i].level, entries[i].message);
        
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
    
    if (entry_count > 0) {
        printf("\\nFirst entry: %s\\n", entries[0].timestamp);
        printf("Last entry: %s\\n", entries[entry_count-1].timestamp);
    }
    
    return 0;
}
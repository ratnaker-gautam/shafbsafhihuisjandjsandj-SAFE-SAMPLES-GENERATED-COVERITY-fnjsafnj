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

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 ||
           strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    FILE* file;
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    time_t earliest = 0, latest = 0;
    int first = 1;

    file = fopen("sample.log", "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open log file\n");
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        struct LogEntry entry;
        struct tm tm = {0};
        char* ptr = line;
        int field = 0;
        
        while (*ptr && field < 3) {
            char* start = ptr;
            while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') ptr++;
            if (ptr > start) {
                size_t len = ptr - start;
                if (field == 0 && len < sizeof(entry.timestamp)) {
                    strncpy(entry.timestamp, start, len);
                    entry.timestamp[len] = '\0';
                } else if (field == 1 && len < sizeof(entry.level)) {
                    strncpy(entry.level, start, len);
                    entry.level[len] = '\0';
                } else if (field == 2 && len < sizeof(entry.message)) {
                    strncpy(entry.message, start, len);
                    entry.message[len] = '\0';
                }
                field++;
            }
            if (*ptr) ptr++;
        }

        if (field >= 3 && parse_timestamp(entry.timestamp, &tm) && is_valid_level(entry.level)) {
            tm.tm_year -= 1900;
            tm.tm_mon -= 1;
            time_t entry_time = mktime(&tm);
            if (entry_time != -1) {
                if (first || entry_time < earliest) earliest = entry_time;
                if (first || entry_time > latest) latest = entry_time;
                first = 0;
                
                if (strcmp(entry.level, "INFO") == 0) info_count++;
                else if (strcmp(entry.level, "WARN") == 0) warn_count++;
                else if (strcmp(entry.level, "ERROR") == 0) error_count++;
                else if (strcmp(entry.level, "DEBUG") == 0) debug_count++;
                
                entries[entry_count] = entry;
                entry_count++;
            }
        }
    }

    fclose(file);

    printf("Log Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO:  %d\n", info_count);
    printf("WARN:  %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (!first) {
        char buf[32];
        struct tm* tm_early = localtime(&earliest);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_early);
        printf("Earliest: %s\n", buf);
        
        struct tm* tm_late = localtime(&latest);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_late);
        printf("Latest:   %s\n", buf);
        
        double span = difftime(latest, earliest);
        printf("Time span: %.0f seconds\n", span);
    }

    printf("\nRecent entries:\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }

    return 0;
}
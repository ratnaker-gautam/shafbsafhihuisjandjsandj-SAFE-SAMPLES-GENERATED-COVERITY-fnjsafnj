//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define IS_VALID_LEVEL(level) \
    (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || \
     strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0)

#define PARSE_TIMESTAMP(str, tm) \
    (sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d", \
            &(tm)->tm_year, &(tm)->tm_mon, &(tm)->tm_mday, \
            &(tm)->tm_hour, &(tm)->tm_min, &(tm)->tm_sec) == 6)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[8];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

int read_log_file(const char* filename, LogEntry entries[], int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) >= sizeof(line) - 1) {
            continue;
        }
        
        char timestamp[TIMESTAMP_LEN] = {0};
        char level[8] = {0};
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10] = {0};
        
        if (sscanf(line, "%19s %7s %255[^\n]", 
                   timestamp, level, message) == 3) {
            if (strlen(timestamp) == 19 && IS_VALID_LEVEL(level)) {
                strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
                strncpy(entries[count].level, level, 7);
                strncpy(entries[count].message, message, 
                        sizeof(entries[count].message) - 1);
                entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
                entries[count].level[7] = '\0';
                entries[count].message[sizeof(entries[count].message) - 1] = '\0';
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

void analyze_logs(LogEntry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < LOG_LEVELS; j++) {
            if (strcmp(entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
    
    if (count > 0) {
        struct tm first_tm = {0};
        struct tm last_tm = {0};
        
        if (PARSE_TIMESTAMP(entries[0].timestamp, &first_tm) &&
            PARSE_TIMESTAMP(entries[count-1].timestamp, &last_tm)) {
            first_tm.tm_year -= 1900;
            first_tm.tm_mon -= 1;
            last_tm.tm_year -= 1900;
            last_tm.tm_mon -= 1;
            time_t first = mktime(&first_tm);
            time_t last = mktime(&last_tm);
            if (first != -1 && last != -1) {
                double hours = difftime(last, first) / 3600.0;
                if (hours >= 0) {
                    printf("Time span: %.2f hours\n", hours);
                }
            }
        }
    }
}

void search_logs(LogEntry entries[], int count, const char* keyword) {
    if (!keyword || strlen(keyword) == 0) {
        return;
    }
    
    printf("Search results for '%s':\n", keyword);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, keyword) != NULL) {
            printf("[%s] %s: %s\n", entries[i].timestamp, 
                   entries[i].level, entries[i].message);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No matches found.\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        printf("Usage: %s <logfile> [search_term]\n", argv[0]);
        return 1;
    }
    
    if (strlen(argv[1]) == 0) {
        printf("Error:
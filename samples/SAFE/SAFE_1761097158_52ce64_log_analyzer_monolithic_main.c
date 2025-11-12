//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
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

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    FILE* file;
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    time_t earliest = 0, latest = 0;
    int first_entry = 1;
    
    file = fopen("sample.log", "r");
    if (file == NULL) {
        printf("Error: Cannot open log file\n");
        return 1;
    }
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        struct LogEntry entry;
        struct tm time_info;
        char* token;
        int field_count = 0;
        
        if (strlen(line) >= sizeof(line) - 1) {
            continue;
        }
        
        token = strtok(line, " ");
        while (token != NULL && field_count < 3) {
            switch (field_count) {
                case 0:
                    if (strlen(token) < sizeof(entry.timestamp)) {
                        strcpy(entry.timestamp, token);
                    }
                    break;
                case 1:
                    if (strlen(token) < sizeof(entry.level)) {
                        strcpy(entry.level, token);
                    }
                    break;
                case 2:
                    if (strlen(token) < sizeof(entry.message)) {
                        strcpy(entry.message, token);
                    }
                    break;
            }
            token = strtok(NULL, " ");
            field_count++;
        }
        
        if (field_count >= 3 && parse_timestamp(entry.timestamp, &time_info) && 
            is_valid_level(entry.level)) {
            
            time_info.tm_year -= 1900;
            time_info.tm_mon -= 1;
            time_t entry_time = mktime(&time_info);
            
            if (entry_time != (time_t)-1) {
                logs[entry_count] = entry;
                entry_count++;
                
                if (strcmp(entry.level, "INFO") == 0) info_count++;
                else if (strcmp(entry.level, "WARN") == 0) warn_count++;
                else if (strcmp(entry.level, "ERROR") == 0) error_count++;
                else if (strcmp(entry.level, "DEBUG") == 0) debug_count++;
                
                if (first_entry) {
                    earliest = entry_time;
                    latest = entry_time;
                    first_entry = 0;
                } else {
                    if (entry_time < earliest) earliest = entry_time;
                    if (entry_time > latest) latest = entry_time;
                }
            }
        }
    }
    
    fclose(file);
    
    printf("Log Analysis Results:\n");
    printf("=====================\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    printf("DEBUG entries: %d\n", debug_count);
    
    if (entry_count > 0) {
        double time_span = difftime(latest, earliest);
        int hours = (int)(time_span / 3600);
        int minutes = (int)((time_span - hours * 3600) / 60);
        printf("Time span: %d hours, %d minutes\n", hours, minutes);
        
        printf("\nRecent entries:\n");
        int display_count = (entry_count < 5) ? entry_count : 5;
        for (int i = entry_count - display_count; i < entry_count; i++) {
            printf("%s %s %s", logs[i].timestamp, logs[i].level, logs[i].message);
        }
    }
    
    return 0;
}
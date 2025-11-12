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
    char filename[256];
    FILE* file;
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Filename cannot be empty\n");
        return 1;
    }
    
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        struct LogEntry* entry = &entries[entry_count];
        char* newline = strchr(line, '\n');
        if (newline) *newline = '\0';
        
        char* first_space = strchr(line, ' ');
        char* second_space = first_space ? strchr(first_space + 1, ' ') : NULL;
        
        if (first_space && second_space) {
            size_t ts_len = first_space - line;
            if (ts_len < sizeof(entry->timestamp)) {
                strncpy(entry->timestamp, line, ts_len);
                entry->timestamp[ts_len] = '\0';
            } else {
                continue;
            }
            
            size_t level_len = second_space - (first_space + 1);
            if (level_len < sizeof(entry->level)) {
                strncpy(entry->level, first_space + 1, level_len);
                entry->level[level_len] = '\0';
            } else {
                continue;
            }
            
            const char* msg_start = second_space + 1;
            size_t msg_len = strlen(msg_start);
            if (msg_len < sizeof(entry->message)) {
                strncpy(entry->message, msg_start, sizeof(entry->message) - 1);
                entry->message[sizeof(entry->message) - 1] = '\0';
            } else {
                continue;
            }
            
            entry_count++;
        }
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    qsort(entries, entry_count, sizeof(struct LogEntry), compare_entries);
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    printf("\nSorted log entries:\n");
    printf("===================\n");
    
    for (int i = 0; i < entry_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        
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
    
    return 0;
}
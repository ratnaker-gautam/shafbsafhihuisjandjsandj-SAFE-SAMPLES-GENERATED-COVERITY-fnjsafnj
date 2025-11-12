//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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

int read_log_file(const char* filename, struct LogEntry* entries, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LEN];
    *count = 0;
    
    while (fgets(line, sizeof(line), file) && *count < MAX_ENTRIES) {
        if (strlen(line) >= sizeof(line) - 1) {
            printf("Warning: Line too long, skipping\n");
            continue;
        }
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[MAX_LINE_LEN] = {0};
        
        int parsed = sscanf(line, "%31s %15s %1023[^\n]", timestamp, level, message);
        if (parsed >= 2) {
            struct LogEntry* entry = &entries[*count];
            strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
            strncpy(entry->level, level, sizeof(entry->level) - 1);
            if (parsed == 3) {
                strncpy(entry->message, message, sizeof(entry->message) - 1);
            } else {
                entry->message[0] = '\0';
            }
            (*count)++;
        }
    }
    
    if (ferror(file)) {
        printf("Error reading file\n");
        fclose(file);
        return 0;
    }
    
    fclose(file);
    return 1;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (count > 0) {
        printf("\nFirst 5 entries:\n");
        for (int i = 0; i < 5 && i < count; i++) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    if (strlen(argv[1]) == 0) {
        printf("Error: Empty filename\n");
        return 1;
    }
    
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    if (!read_log_file(argv[1], entries, &entry_count)) {
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    qsort(entries, entry_count, sizeof(struct LogEntry), compare_entries);
    
    analyze_logs(entries, entry_count);
    
    return 0;
}
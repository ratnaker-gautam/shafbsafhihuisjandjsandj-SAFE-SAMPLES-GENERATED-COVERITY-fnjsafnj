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

int is_valid_level(const char* level) {
    const char* valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (size_t i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[MAX_LINE_LEN];
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    line[strcspn(line, "\n")] = '\0';
    
    char ts[32], lvl[16], msg[MAX_LINE_LEN];
    if (sscanf(line, "%31s %15s %1023[^\n]", ts, lvl, msg) != 3) {
        return 0;
    }
    
    struct tm tm;
    if (!parse_timestamp(ts, &tm)) {
        return 0;
    }
    
    if (!is_valid_level(lvl)) {
        return 0;
    }
    
    strncpy(entry->timestamp, ts, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, lvl, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, msg, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int level_counts[4] = {0};
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < 4; i++) {
        printf("%s: %d (%.1f%%)\n", levels[i], level_counts[i],
               count > 0 ? (level_counts[i] * 100.0) / count : 0.0);
    }
}

int main(void) {
    char filename[256];
    printf("Enter log file name: ");
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }
    
    struct LogEntry entries[MAX_ENTRIES];
    int count = 0;
    
    while (count < MAX_ENTRIES) {
        if (!read_log_entry(file, &entries[count])) {
            break;
        }
        count++;
    }
    
    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
        fclose(file);
        return 1;
    }
    
    fclose(file);
    
    if (count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    printf("Successfully read %d log entries\n", count);
    
    analyze_logs(entries, count);
    
    printf("\nFirst 5 entries:\n");
    printf("================\n");
    for (int i = 0; i < 5 && i < count; i++) {
        printf("%d: [%s] %s: %s\n", i + 1, entries[i].timestamp,
               entries[i].level, entries[i].message);
    }
    
    return 0;
}
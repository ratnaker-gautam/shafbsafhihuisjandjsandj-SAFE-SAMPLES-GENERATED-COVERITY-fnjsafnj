//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
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

int parse_log_line(char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char *ptr = line;
    while (*ptr && isspace(*ptr)) ptr++;
    if (!*ptr) return 0;
    
    char *timestamp_end = strchr(ptr, ' ');
    if (!timestamp_end || timestamp_end - ptr >= 32) return 0;
    strncpy(entry->timestamp, ptr, timestamp_end - ptr);
    entry->timestamp[timestamp_end - ptr] = '\0';
    
    ptr = timestamp_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    if (!*ptr) return 0;
    
    char *level_end = strchr(ptr, ' ');
    if (!level_end || level_end - ptr >= 16) return 0;
    strncpy(entry->level, ptr, level_end - ptr);
    entry->level[level_end - ptr] = '\0';
    
    ptr = level_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    if (!*ptr) return 0;
    
    size_t msg_len = strlen(ptr);
    if (msg_len >= MAX_LINE_LEN) return 0;
    strcpy(entry->message, ptr);
    
    return 1;
}

int count_level_occurrences(struct LogEntry *entries, int count, const char *level) {
    int occurrences = 0;
    struct LogEntry *end = entries + count;
    for (struct LogEntry *e = entries; e < end; e++) {
        if (strcmp(e->level, level) == 0) {
            occurrences++;
        }
    }
    return occurrences;
}

void find_earliest_latest(struct LogEntry *entries, int count, 
                         char *earliest, char *latest) {
    if (count <= 0) {
        earliest[0] = '\0';
        latest[0] = '\0';
        return;
    }
    
    strcpy(earliest, entries[0].timestamp);
    strcpy(latest, entries[0].timestamp);
    
    struct LogEntry *end = entries + count;
    for (struct LogEntry *e = entries + 1; e < end; e++) {
        if (strcmp(e->timestamp, earliest) < 0) {
            strcpy(earliest, e->timestamp);
        }
        if (strcmp(e->timestamp, latest) > 0) {
            strcpy(latest, e->timestamp);
        }
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter empty line to finish.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(buffer, sizeof(buffer), stdin)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (buffer[0] == '\0') {
            break;
        }
        
        if (parse_log_line(buffer, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", buffer);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    char earliest[32], latest[32];
    find_earliest_latest(entries, entry_count, earliest, latest);
    printf("Time range: %s to %s\n", earliest, latest);
    
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        int count = count_level_occurrences(entries, entry_count, levels[i]);
        printf("%s: %d\n", levels[i], count);
    }
    
    printf("\nSample entries:\n");
    int samples = (entry_count < 3) ? entry_count : 3;
    for (int i = 0; i < samples; i++) {
        printf("%d: [%s] %s: %s\n", i + 1, 
               entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}
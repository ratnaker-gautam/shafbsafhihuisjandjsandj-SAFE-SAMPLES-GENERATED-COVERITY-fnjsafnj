//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
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
    char message[256];
};

int parse_timestamp(const char *str, struct tm *tm) {
    return sscanf(str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_level(const char *level) {
    const char *valid[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (size_t i = 0; i < 4; i++) {
        if (strcmp(level, valid[i]) == 0) return 1;
    }
    return 0;
}

int read_log_entry(FILE *file, struct LogEntry *entry) {
    char line[MAX_LINE_LEN];
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    char *ptr = line;
    while (*ptr && isspace(*ptr)) ptr++;
    
    char *timestamp_end = strchr(ptr, ' ');
    if (!timestamp_end || (timestamp_end - ptr) >= 32) return 0;
    strncpy(entry->timestamp, ptr, timestamp_end - ptr);
    entry->timestamp[timestamp_end - ptr] = '\0';
    
    ptr = timestamp_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    
    char *level_end = strchr(ptr, ' ');
    if (!level_end || (level_end - ptr) >= 16) return 0;
    strncpy(entry->level, ptr, level_end - ptr);
    entry->level[level_end - ptr] = '\0';
    
    if (!is_valid_level(entry->level)) return 0;
    
    ptr = level_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    
    char *message_end = strchr(ptr, '\n');
    if (message_end) *message_end = '\0';
    if (strlen(ptr) >= 256) return 0;
    strcpy(entry->message, ptr);
    
    return 1;
}

int compare_entries(const void *a, const void *b) {
    const struct LogEntry *entry_a = (const struct LogEntry*)a;
    const struct LogEntry *entry_b = (const struct LogEntry*)b;
    
    struct tm tm_a, tm_b;
    if (!parse_timestamp(entry_a->timestamp, &tm_a)) return 0;
    if (!parse_timestamp(entry_b->timestamp, &tm_b)) return 0;
    
    time_t time_a = mktime(&tm_a);
    time_t time_b = mktime(&tm_b);
    
    if (time_a < time_b) return -1;
    if (time_a > time_b) return 1;
    return 0;
}

void count_by_level(struct LogEntry *entries, int count) {
    int info = 0, warn = 0, error = 0, debug = 0;
    
    struct LogEntry *ptr = entries;
    for (int i = 0; i < count; i++, ptr++) {
        if (strcmp(ptr->level, "INFO") == 0) info++;
        else if (strcmp(ptr->level, "WARN") == 0) warn++;
        else if (strcmp(ptr->level, "ERROR") == 0) error++;
        else if (strcmp(ptr->level, "DEBUG") == 0) debug++;
    }
    
    printf("Log level summary:\n");
    printf("INFO:  %d\n", info);
    printf("WARN:  %d\n", warn);
    printf("ERROR: %d\n", error);
    printf("DEBUG: %d\n", debug);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter empty line to finish.\n");
    
    char buffer[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        if (buffer[0] == '\n') break;
        
        FILE *temp = tmpfile();
        if (!temp) {
            fprintf(stderr, "Error creating temporary file\n");
            return 1;
        }
        
        fputs(buffer, temp);
        rewind(temp);
        
        if (read_log_entry(temp, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log entry format. Skipping.\n");
        }
        
        fclose(temp);
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 0;
    }
    
    qsort(entries, entry_count, sizeof(struct LogEntry), compare_entries);
    
    printf("\nSorted log entries:\n");
    struct LogEntry *ptr = entries;
    for (int i = 0; i < entry_count; i++, ptr++) {
        printf("%s
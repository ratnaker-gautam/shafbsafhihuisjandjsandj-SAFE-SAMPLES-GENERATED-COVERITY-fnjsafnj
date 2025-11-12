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
    char message[256];
};

int parse_timestamp(const char *str, struct tm *tm) {
    return sscanf(str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_level(const char *level) {
    const char *valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (size_t i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int read_log_entry(FILE *file, struct LogEntry *entry) {
    char line[MAX_LINE_LEN];
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }

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

    size_t msg_len = strlen(ptr);
    if (msg_len == 0 || msg_len >= 256) return 0;
    if (ptr[msg_len - 1] == '\n') ptr[msg_len - 1] = '\0';
    strncpy(entry->message, ptr, 255);
    entry->message[255] = '\0';

    return 1;
}

int compare_entries(const void *a, const void *b) {
    const struct LogEntry *entry_a = (const struct LogEntry *)a;
    const struct LogEntry *entry_b = (const struct LogEntry *)b;
    
    struct tm tm_a, tm_b;
    if (!parse_timestamp(entry_a->timestamp, &tm_a)) return 0;
    if (!parse_timestamp(entry_b->timestamp, &tm_b)) return 0;
    
    tm_a.tm_year -= 1900;
    tm_a.tm_mon -= 1;
    tm_b.tm_year -= 1900;
    tm_b.tm_mon -= 1;
    
    time_t time_a = mktime(&tm_a);
    time_t time_b = mktime(&tm_b);
    
    if (time_a == (time_t)-1 || time_b == (time_t)-1) return 0;
    
    if (time_a < time_b) return -1;
    if (time_a > time_b) return 1;
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    size_t entry_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter empty line to finish.\n");
    
    char input[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        if (input[0] == '\n') break;
        
        FILE *temp = tmpfile();
        if (!temp) continue;
        
        fputs(input, temp);
        rewind(temp);
        
        if (read_log_entry(temp, entries + entry_count)) {
            entry_count++;
        }
        
        fclose(temp);
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    qsort(entries, entry_count, sizeof(struct LogEntry), compare_entries);
    
    printf("\nSorted log entries:\n");
    printf("===================\n");
    
    struct LogEntry *current = entries;
    struct LogEntry *end = entries + entry_count;
    
    while (current < end) {
        printf("%s %s %s\n", current->timestamp, current->level, current->message);
        current++;
    }
    
    int info_count = 0, warning_count = 0, error_count = 0, debug_count = 0;
    current = entries;
    
    while (current < end) {
        char *level_ptr = current->level;
        if (strcmp(level_ptr, "INFO") == 0) info_count++;
        else if (strcmp(level_ptr, "WARNING") == 0) warning_count++;
        else if (strcmp(level_ptr, "ERROR")
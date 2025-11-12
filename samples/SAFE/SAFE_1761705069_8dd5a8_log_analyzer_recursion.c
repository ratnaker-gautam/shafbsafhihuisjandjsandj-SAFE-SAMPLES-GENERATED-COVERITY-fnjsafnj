//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[256];
    int timestamp;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level_buf[16];
    char message_buf[256];
    int timestamp;
    
    int parsed = sscanf(line, "%d %15s %255[^\n]", &timestamp, level_buf, message_buf);
    if (parsed != 3) return 0;
    
    if (timestamp < 0) return 0;
    
    for (int i = 0; level_buf[i]; i++) {
        if (!isalpha(level_buf[i])) return 0;
    }
    
    strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->timestamp = timestamp;
    
    return 1;
}

void analyze_log_recursive(FILE* file, int depth, int* error_count, int* warning_count, int* info_count) {
    if (depth >= MAX_LEVELS || file == NULL || error_count == NULL || 
        warning_count == NULL || info_count == NULL) {
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) {
        return;
    }
    
    line[strcspn(line, "\n")] = '\0';
    
    if (strlen(line) == 0) {
        analyze_log_recursive(file, depth + 1, error_count, warning_count, info_count);
        return;
    }
    
    struct LogEntry entry;
    if (parse_log_line(line, &entry)) {
        if (strcmp(entry.level, "ERROR") == 0) {
            (*error_count)++;
        } else if (strcmp(entry.level, "WARNING") == 0) {
            (*warning_count)++;
        } else if (strcmp(entry.level, "INFO") == 0) {
            (*info_count)++;
        }
    }
    
    analyze_log_recursive(file, depth + 1, error_count, warning_count, info_count);
}

void generate_sample_log(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) return;
    
    fprintf(file, "1000 INFO System started\n");
    fprintf(file, "1005 WARNING Low memory\n");
    fprintf(file, "1010 ERROR Disk full\n");
    fprintf(file, "1015 INFO Backup completed\n");
    fprintf(file, "1020 WARNING High CPU usage\n");
    fprintf(file, "1025 ERROR Network timeout\n");
    fprintf(file, "1030 INFO User login\n");
    
    fclose(file);
}

int main(void) {
    const char* filename = "sample.log";
    generate_sample_log(filename);
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open log file\n");
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    analyze_log_recursive(file, 0, &error_count, &warning_count, &info_count);
    fclose(file);
    
    printf("Log Analysis Results:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries: %d\n", error_count + warning_count + info_count);
    
    remove(filename);
    
    return 0;
}
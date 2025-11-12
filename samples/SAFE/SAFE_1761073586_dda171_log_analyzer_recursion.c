//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    int timestamp_end = -1;
    int level_end = -1;
    
    for (int i = 0; line[i] && i < 19; i++) {
        if (line[i] == ' ') {
            timestamp_end = i;
            break;
        }
        if (!isdigit(line[i]) && line[i] != '-' && line[i] != ':' && line[i] != '.') {
            return 0;
        }
    }
    
    if (timestamp_end == -1) return 0;
    
    for (int i = timestamp_end + 1; line[i] && i < timestamp_end + 10; i++) {
        if (line[i] == ' ') {
            level_end = i;
            break;
        }
        if (!isalpha(line[i])) {
            return 0;
        }
    }
    
    if (level_end == -1) return 0;
    
    strncpy(entry->timestamp, line, timestamp_end);
    entry->timestamp[timestamp_end] = '\0';
    
    strncpy(entry->level, line + timestamp_end + 1, level_end - timestamp_end - 1);
    entry->level[level_end - timestamp_end - 1] = '\0';
    
    const char* message_start = line + level_end + 1;
    if (strlen(message_start) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, message_start);
    
    return 1;
}

void analyze_log_recursive(FILE* file, int depth, int* error_count, int* warning_count, int* info_count) {
    if (depth >= MAX_LEVELS) return;
    if (!file || !error_count || !warning_count || !info_count) return;
    
    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) return;
    
    struct LogEntry entry;
    if (parse_log_line(buffer, &entry)) {
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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
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
    
    return 0;
}
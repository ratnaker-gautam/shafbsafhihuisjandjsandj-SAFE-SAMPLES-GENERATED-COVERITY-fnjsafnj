//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
}

int is_valid_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_file(const char* filename, LogEntry* entries, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    *count = 0;

    while (fgets(line, sizeof(line), file) && *count < MAX_ENTRIES) {
        if (strlen(line) >= sizeof(line) - 1) {
            printf("Warning: Line too long, skipping\n");
            continue;
        }

        char timestamp[32], level[16], message[256];
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed == 3 && is_valid_timestamp(timestamp)) {
            int severity = parse_log_level(level);
            if (severity > 0) {
                strncpy(entries[*count].timestamp, timestamp, sizeof(entries[*count].timestamp) - 1);
                strncpy(entries[*count].level, level, sizeof(entries[*count].level) - 1);
                strncpy(entries[*count].message, message, sizeof(entries[*count].message) - 1);
                entries[*count].severity = severity;
                entries[*count].timestamp[31] = '\0';
                entries[*count].level[15] = '\0';
                entries[*count].message[255] = '\0';
                (*count)++;
            }
        }
    }

    fclose(file);
    return 1;
}

void analyze_logs(LogEntry* entries, int count) {
    int level_counts[6] = {0};
    int total_severity = 0;
    char most_common_level[16] = "";
    int max_level_count = 0;
    int level_totals[6] = {0};

    for (int i = 0; i < count; i++) {
        int severity = entries[i].severity;
        if (severity >= 1 && severity <= 5) {
            level_counts[severity]++;
            total_severity += severity;
            level_totals[severity] += severity;
        }
    }

    for (int i = 1; i <= 5; i++) {
        if (level_counts[i] > max_level_count) {
            max_level_count = level_counts[i];
            switch(i) {
                case 1: strcpy(most_common_level, "DEBUG"); break;
                case 2: strcpy(most_common_level, "INFO"); break;
                case 3: strcpy(most_common_level, "WARNING"); break;
                case 4: strcpy(most_common_level, "ERROR"); break;
                case 5: strcpy(most_common_level, "CRITICAL"); break;
            }
        }
    }

    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("DEBUG: %d\n", level_counts[1]);
    printf("INFO: %d\n", level_counts[2]);
    printf("WARNING: %d\n", level_counts[3]);
    printf("ERROR: %d\n", level_counts[4]);
    printf("CRITICAL: %d\n", level_counts[5]);
    printf("Most common level: %s\n", most_common_level);
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("Average severity: %.2f\n", avg_severity);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    LogEntry entries[MAX_ENTRIES];
    int count = 0;

    if (!read_log_file(argv[1], entries, &count)) {
        return 1;
    }

    if (count == 0) {
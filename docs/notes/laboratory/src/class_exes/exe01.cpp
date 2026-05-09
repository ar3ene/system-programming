#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define STR_MAX 30

typedef struct record_s {
    int id;
    long reg_no;
    char surname[STR_MAX + 1];
    char name[STR_MAX + 1];
    int mark;
    struct record_s* next;
} record_t;

static record_t* head = NULL;

static char skip_spaces(int fd) {
    char c;
    while (1) {
        ssize_t n = read(fd, &c, 1);
        if (n == 1) {
            if (!isspace((unsigned char)c)) {
                return c;
            }
            continue;
        }
        if (n == 0) {
            return '\0';  // EOF
        }
        if (n < 0 && errno == EINTR) {
            continue;
        }
        perror("read");
        return '\0';
    }
}

static int read_token(int fd, char* buf, size_t capacity) {
    if (capacity == 0) {
        return -1;
    }

    char first = skip_spaces(fd);
    if (first == '\0') {
        return 0;  // no more tokens
    }

    size_t idx = 0;
    buf[idx++] = first;

    while (idx + 1 < capacity) {
        char c;
        ssize_t n = read(fd, &c, 1);
        if (n == 0) {
            break;  // EOF
        }
        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("read");
            return -1;
        }
        if (isspace((unsigned char)c)) {
            break;
        }
        buf[idx++] = c;
    }

    if (idx + 1 == capacity) {
        // buffer full, discard the rest of the token
        char c;
        while (read(fd, &c, 1) == 1) {
            if (isspace((unsigned char)c)) {
                break;
            }
        }
        fprintf(stderr, "token too long (capacity %zu)\n", capacity);
        return -1;
    }

    buf[idx] = '\0';
    return 1;
}

static int parse_int(const char* token, int* out) {
    char* endptr = NULL;
    errno = 0;
    long val = strtol(token, &endptr, 10);
    if (errno != 0 || endptr == token || *endptr != '\0') {
        fprintf(stderr, "invalid integer token '%s'\n", token);
        return -1;
    }
    *out = (int)val;
    return 0;
}

static int parse_long(const char* token, long* out) {
    char* endptr = NULL;
    errno = 0;
    long val = strtol(token, &endptr, 10);
    if (errno != 0 || endptr == token || *endptr != '\0') {
        fprintf(stderr, "invalid long token '%s'\n", token);
        return -1;
    }
    *out = val;
    return 0;
}

static void free_list(record_t* node) {
    while (node != NULL) {
        record_t* next = node->next;
        free(node);
        node = next;
    }
}

int main(void) {
    int fd = open("exe01.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    char id_buf[16];
    char reg_buf[32];
    char surname_buf[STR_MAX + 1];
    char name_buf[STR_MAX + 1];
    char mark_buf[16];

    while (1) {
        int status = read_token(fd, id_buf, sizeof(id_buf));
        if (status <= 0) {
            if (status < 0) {
                fprintf(stderr, "failed to read id field\n");
            }
            break;
        }

        if (read_token(fd, reg_buf, sizeof(reg_buf)) <= 0 ||
            read_token(fd, surname_buf, sizeof(surname_buf)) <= 0 ||
            read_token(fd, name_buf, sizeof(name_buf)) <= 0 ||
            read_token(fd, mark_buf, sizeof(mark_buf)) <= 0) {
            fprintf(stderr, "incomplete record in input file\n");
            break;
        }

        int id = 0;
        long reg_no = 0;
        int mark = 0;
        if (parse_int(id_buf, &id) < 0 ||
            parse_long(reg_buf, &reg_no) < 0 ||
            parse_int(mark_buf, &mark) < 0) {
            fprintf(stderr, "skipping malformed record\n");
            continue;
        }

        record_t* node = (record_t*)malloc(sizeof(record_t));
        if (node == NULL) {
            perror("malloc");
            break;
        }
        node->id = id;
        node->reg_no = reg_no;
        strncpy(node->surname, surname_buf, STR_MAX);
        node->surname[STR_MAX] = '\0';
        strncpy(node->name, name_buf, STR_MAX);
        node->name[STR_MAX] = '\0';
        node->mark = mark;
        node->next = head;
        head = node;
    }

    close(fd);

    for (record_t* node = head; node != NULL; node = node->next) {
        printf("%d %ld %s %s %d\n",
               node->id,
               node->reg_no,
               node->surname,
               node->name,
               node->mark);
    }

    free_list(head);
    head = NULL;
    return 0;
}

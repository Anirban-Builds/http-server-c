#include "stdlib.h"
#include "string.h"
#include "ctype.h"

#ifdef _WIN32
    #define STRCASESTR strcasestr_custom
#else
    #define STRCASESTR strcasestr
#endif

char* strcasestr_custom(char* str, char* substr) {
    if (!*substr) return (char*)str;
    for (; *str; str++) {
        const char *h = str, *n = substr;
        while (*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n)) {
            h++; n++;
        }
        if (!*n) return (char*)str;
    }
    return NULL;
}

int is_allowed_origin(char* request)
{
    char* origin_start = STRCASESTR(request, "Origin: ");
    if (!origin_start) {
        return 0;
    }
    origin_start += 8;

    char origin[256] = {0};
    int i = 0;
    while (origin_start[i] != '\r' && origin_start[i] != '\n' && i < 255) {
        origin[i] = origin_start[i];
        i++;
    }
    origin[i] = '\0';

    if (strcmp(origin, getenv("CORS")) == 0){
        return 1;   // Allowed
    }

    return 0;   // Blocked
}

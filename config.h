#ifndef CONFIG_H
#define CONFIG_H

#define MAX_NAME_LEN 64  // 姓名最大长度

#ifdef DEBUG
    #define DEBUG_PRINT(fmt,...) fprintf(stderr,"[DEBUG] "fmt "\n", ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt,...)
#endif

#endif
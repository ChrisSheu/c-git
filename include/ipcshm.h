//--------------
//share memory
//--------------
#define SHM_KEY         5000
#define MSG_LEN         64
#define PACKAGE_LEN     5
#define DEFAULT_SEC     3

typedef struct _PACKAGE{
    time_t ticks;
    int id;
    char msg[MSG_LEN];
} PACKAGE;

typedef struct _SHARED_MEMORY_CONTENT {
    PACKAGE p[PACKAGE_LEN];
} SHARED_MEMORY_CONTENT;


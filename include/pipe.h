//--------------
//pipe structure
//--------------
#define MSG_LEN         64
#define PACKAGE_LEN     5
#define DEFAULT_SEC     3
enum{
    INIT = 1,
    SET,
    SHOW
};

typedef struct _PACKAGE{
    int cmd;
    time_t ticks;
    char msg[MSG_LEN];
} PACKAGE;

typedef struct _SHARED_MEMORY_CONTENT {
    PACKAGE p[PACKAGE_LEN];
} SHARED_MEMORY_CONTENT;

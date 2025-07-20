#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_t tid1, tid2;

typedef struct {
    char name[30];
    char msg[30];
} thr_data_t;

static void *thr_handler(void *args) {

    pthread_t tid = pthread_self();

    if (pthread_equal(tid, tid1)) {
        printf("Hi i am thread handler1\n");
    } else {
        thr_data_t *data = (thr_data_t *) args;
        printf("Hi i am thread handler2\n");
        printf("Hello %s, welcome to join %s\n", data->name, data->msg);
    }

    return NULL;
}

// static void *thr_handler1(void *args) {
//     printf("Hi i am thread handler1\n");
//     return NULL;
// }

// static void *thr_handler2(void *args) {
//     thr_data_t *data = (thr_data_t *) args;
//     printf("Hi i am thread handler2\n");
//     printf("Hello %s, welcome to join %s\n", data->name, data->msg);
//     return NULL;
// }

int main(int argc, char const *agrv[]) {
    int ret;
    thr_data_t data = {0};

    strncpy(data.name, "MrGonf", sizeof(data.name));
    strncpy(data.msg, "Linux programming\n", sizeof(data.msg));

    // Create thread 1
    if ((ret = pthread_create(&tid1, NULL, &thr_handler, NULL)) != 0) {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    sleep(1);

    if ((ret = pthread_create(&tid2, NULL, &thr_handler, &data)) != 0) {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    sleep(1);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("Hi i am main\n");

    return 0;

}

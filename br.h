#ifndef __BR_91823__
#define __BR_91823__

#ifdef __cplusplus
extern "C" {
#endif

//struct br_broadcaster;
//struct br_receiver;

typedef struct br_broadcaster br_broadcaster;
typedef struct br_receiver br_receiver;

br_broadcaster* br_new_broadcaster(void);
br_receiver* br_new_receiver(void);
void br_free_broadcaster(br_broadcaster* b);
void br_free_receiver(br_receiver* r);

int br_broadcast(br_broadcaster* b, const char* data, int size);
int br_receive(br_receiver* r, char* data, int size);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif

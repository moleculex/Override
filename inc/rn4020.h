#ifndef __RN4020_H
#define __RN4020_H

typedef struct{
	char buffer[128];
	char *buffer_ptr;
}t_rn4020;

t_rn4020 _rn4020;

void rn4020_init(void);
void rn4020_publish(void);
void rn4020_write(char *buffer, char length);

#endif

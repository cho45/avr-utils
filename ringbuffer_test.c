//#!gcc -O1 -g3 -gdwarf-2 -Wall ringbuffer_test.c -o /tmp/a.out && /tmp/a.out

#include <stdio.h>
#define uint8_t unsigned char

#include "ringbuffer.h";

#define is(flag, message) { \
	num++; \
	if (flag) { \
		printf("ok %d %s:%d %s\n", num, __FILE__, __LINE__, message); \
	} else { \
		printf("not ok %d %s:%d %s\n", num, __FILE__, __LINE__, message); \
	} \
}

int num = 0;

void dump (ringbuffer* buffer) {
	printf("write_index=%d, read_index=%d, size=%d, capacity=%d\n",
		buffer->write_index,
		buffer->read_index,
		buffer->size,
		buffer->capacity
	);

	int i;
	for (i = 0; i < buffer->capacity; i++) {
		printf("\tdata[%02d] = %02x\n", i, buffer->data[i]);
	}
}

int main () {
	ringbuffer buffer;
	uint8_t data[5];
	ringbuffer_init(&buffer, data, 5);

	is(buffer.size == 0, "set");

	ringbuffer_put(&buffer, 1);
	is(buffer.size == 1, "set");
	is(data[0] == 1, "memory");

	ringbuffer_put(&buffer, 2);
	is(buffer.size == 2, "set");
	ringbuffer_put(&buffer, 3);
	is(buffer.size == 3, "set");
	ringbuffer_put(&buffer, 4);
	is(buffer.size == 4, "set");
	ringbuffer_put(&buffer, 5);
	is(buffer.size == 5, "set");
	ringbuffer_put(&buffer, 6);
	is(buffer.size == 5, "set");

	dump(&buffer);

	is(data[0] == 6, "memory");
	is(data[1] == 2, "memory");
	is(data[2] == 3, "memory");
	is(data[3] == 4, "memory");
	is(data[4] == 5, "memory");

	is(ringbuffer_get(&buffer) == 2, "get");
	is(buffer.size == 4, "size");
	is(ringbuffer_get(&buffer) == 3, "get");
	is(buffer.size == 3, "size");
	is(ringbuffer_get(&buffer) == 4, "get");
	is(buffer.size == 2, "size");
	is(ringbuffer_get(&buffer) == 5, "get");
	is(buffer.size == 1, "size");
	is(ringbuffer_get(&buffer) == 6, "get");
	is(buffer.size == 0, "size");
	is(ringbuffer_get(&buffer) == 0, "get");
	is(buffer.size == 0, "size");

	ringbuffer_put(&buffer, 0);
	ringbuffer_put(&buffer, 1);
	ringbuffer_put(&buffer, 2);
	ringbuffer_put(&buffer, 3);
	ringbuffer_put(&buffer, 4);

	dump(&buffer);

	is(ringbuffer_get(&buffer) == 0, "get");
	is(ringbuffer_get(&buffer) == 1, "get");
	is(ringbuffer_get(&buffer) == 2, "get");
	is(ringbuffer_get(&buffer) == 3, "get");
	is(ringbuffer_get(&buffer) == 4, "get");

	return 0;
};

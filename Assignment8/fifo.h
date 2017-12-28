//fee fifo fum
#ifndef FIFO_H
#define FIFO_H


#define MY_FIFO_BUFSIZ 1024
#include "cv.h"

typedef struct fifo{
	unsigned long fifo_buf[MY_FIFO_BUFSIZ];
	int next_write, next_read;
	int item_count;
	cv full, empty;
	account fifo_mutex_lock;
}fifo;


void fifo_init(struct fifo *f);
	/* initialize the shared memory FIFO *f including any required underlying
	 * initializations (such as calling cv_init). The FIFO will have a static  
	 * fifo length of MY_FIFO_BUFSIZ elements. #define this in fifo.h.
	 * a value of 1K is reasonable
	 */


void fifo_wr(struct fifo *f, unsigned long d);
	/* Enqueue the data word d into the FIFO, clocking unless AND until the
	 * FIFO has room to accept it. (i.e.) block until !full)
	 */

unsigned long fifo_rd(struct fifo *f);
	/* Dequeue the next data word from the FIFO and return it. Block unless
	 * AND until there are available words. (i.e. block until !empty);
	 */

#endif

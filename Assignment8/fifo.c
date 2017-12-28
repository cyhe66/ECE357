//maintains a FIFO of unsigned londs using a shared memor data structure protected and coordinated with the spinlokc and the condition variable modules developed [previously]. 


#include "fifo.h"


/*
 * fifo->mutex = PTHREAD_MUTEX_INITIALIZER	x 
 * fifo->full  = PTHREAD_COND_INITIALIZER	x
 * fifo->empty = PTHREAD_COND_INITIALIZER	x
 * fifo->next_write=0;						x
 * fifo->next_read =0;						x
 * fifo->item_count=0;						x
 *  [[ unsigned long fifo bufff []   ]]]	x 
 */
void fifo_init(struct fifo *f){
	cv_init(&f->full);
	cv_init(&f->empty);
	f->next_write = 0;
	f->next_read = 0;
	f->item_count = 0;
	f->fifo_mutex_lock.lock = 0;
	for (int ii = 0; ii < MY_FIFO_BUFSIZ; ii++){
		f->fifo_buf[ii] = 0;
	}
}

/* pthread_mutex_lock(&fifo->mutex)						x
 * while (fifo->item_count>=BSIZE)  FULL				x
 *		pthread_cond_wait(&fifo->full, &fifo->mutex);	x
 * fifo->buf[fifo->next_write++]=c;						x
 * fifo->next_write %= BSIZE;							x
 * fifo->item_count++;									x
 * pthread_cond_signal(&fifo->empty)					x
 * pthread_mutex_unlock(&fifo->mutex);					x
 */
void fifo_wr(struct fifo *f, unsigned long d){
	spin_lock(&f->fifo_mutex_lock);
	while(f->item_count >= MY_FIFO_BUFSIZ){
		cv_wait(&f->full, &f->fifo_mutex_lock);
	}
	f->fifo_buf[f->next_write++] = d;
	f->next_write %= MY_FIFO_BUFSIZ;
	f->item_count++;
	cv_signal(&f->empty);
	spin_unlock(&f->fifo_mutex_lock);
}


/* char c;												x
 * pthread_mutex_lock(&fifo->mutex);					x
 * while (fifo->item_count <= 0)						x
 *		pthread_cond_wait(&fifo->empty, &fifo->mutex);	x
 * c = fifo->buf[fifo->next_read++];					x
 * fifo->next_read %= BSIZE;							x
 * fifo->item_count--;									x
 * pthread_cond_signal(&fifo->full)						x
 * pthread_mutex_unlock(&fifo->mutex);					x
 * return c;											x
 */
unsigned long fifo_rd(struct fifo *f){
	unsigned long d;
	spin_lock(&f->fifo_mutex_lock);
	while (f->item_count <= 0){
		cv_wait(&f->empty, &f->fifo_mutex_lock);
	}
	d = f->fifo_buf[f->next_read++];
	f->next_read %= MY_FIFO_BUFSIZ;
	f->item_count--;
	cv_signal(&f->full);
	spin_unlock(&f->fifo_mutex_lock);
	return d;

}





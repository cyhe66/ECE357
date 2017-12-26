CXX = gcc

all: spintest

spintest: spintest.o spinlock.o
		${CXX} spintest.o spinlock.o -o $@

spinlock.o: spinlock.c spinlock.h tas64.S
		${CXX} -c $<

spintest.o: spintest.c spinlock.h
		${CXX} -c $<

run: spintest
		./spintest

clean:
		rm *o spintest



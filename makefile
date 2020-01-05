OBJS=weblet.o webletfun.o thread_pool.o 
CC=gcc
weblet: ${OBJS}
	${CC} -o $@ $^ -Bstatic -L. -lwrapper -lpthread 	
weblet.o: weblet.c
	${CC} -c $< -o $@ 
thread_pool.o: thread_pool.c
	${CC} -c $< -o $@  
webletfunc.o: webletfun.c
	${CC} -c $< -o $@ 

.PHONY:clean
clean:
	rm *.o weblet

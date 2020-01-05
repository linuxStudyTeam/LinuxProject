OBJS=weblet.o webletfun.o 
CC=gcc
weblet: ${OBJS}
	${CC} -o $@ $^
weblet.o: weblet.c
	${CC} -c $< -o $@
webletfunc.o: webletfun.c
	${CC} -c $< -o $@
.PHONY:clean
clean:
	rm *.o weblet

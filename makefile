all: weblet service
.PHONY: all

OBJS=weblet.o webletfun.o thread_pool.o
SERVICE=service.o servicefun.o

CC=gcc


weblet: ${OBJS}
	${CC} -o $@ $^ -Bstatic -L. -lwrapper -lpthread 	

${OBJS}: %.o: %.c
	${CC} -c $< -o $@ 



service: ${SERVICE}
	${CC} -o $@ $^ -Bstatic -L. -lwrapper
${SERVICE}: %.o: ./serviceCore/%.c
	${CC} -c $< -o $@ 



.PHONY:clean
clean:
	rm *.o weblet service

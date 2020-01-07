all: weblet loginService
.PHONY: all

OBJS=weblet.o webletfun.o thread_pool.o
LOGINSERVICE=loginService.o servicefun.o

CC=gcc


weblet: ${OBJS}
	${CC} -o $@ $^ -Bstatic -L./tools -lwrapper -lpthread 	

${OBJS}: %.o: ./projectEntry/%.c
	${CC} -c $< -o $@ 



loginService: ${LOGINSERVICE}
	${CC} -o $@ $^ -Bstatic -L./tools -lwrapper
${LOGINSERVICE}: %.o: ./serviceCore/%.c
	${CC} -c $< -o $@ 



.PHONY:clean
clean:
	rm *.o weblet loginService

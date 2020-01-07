all: weblet loginService calService
.PHONY: all

OBJS=weblet.o webletfun.o thread_pool.o
LOGINSERVICE=loginService.o servicefun.o
CALSERVICE=calService.o servicefun.o
CC=gcc


weblet: ${OBJS}
	${CC} -o $@ $^ -Bstatic -L./tools -lwrapper -lpthread 	

${OBJS}: %.o: ./projectEntry/%.c
	${CC} -c $< -o $@ 



loginService: ${LOGINSERVICE}
	${CC} -o $@ $^ -Bstatic -L./tools -lwrapper
${LOGINSERVICE}: %.o: ./serviceCore/%.c
	${CC} -c $< -o $@ 

calService: ${CALSERVICE}
	${CC} -o $@ $^ -Bstatic -L./tools -lwrapper
${CALSERVICE}: %.o: ./serviceCore/%.c
	${CC} -c $< -o $@ 

.PHONY:clean
clean:
	rm *.o weblet loginService calService

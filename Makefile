SRCS = *.c
NAME = fftVisualizer
CC = clang
CFLAGS = -Wall -Wextra -g -lm -O3

RM = rm -f
OBJS = $(SRCS:.c=.o)
INCS = raylib/src/libraylib.a

RUN = ./$(NAME)
all: ${NAME}

$(NAME):runraylib $(OBJS)
		${CC} ${CFLAGS} -o  ${NAME} ${OBJS} ${INCS}
		# $(RUN)

ignore:
	${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${INCS}

clean:
	${RM} ${OBJS}

fclean:clean 
	${RM} ${NAME}

re:fclean all

runraylib:
	make  PLATFORM=PLATFORM_DESKTOP -C raylib/src

.PHONY:all re clean fclean

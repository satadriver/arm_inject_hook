SRCS = $(wildcard *.c *.s *.cpp)
OBJS = $(SRCS:.c = .o .cpp = .o .s = .o)
CC = arm-linux-gnueabihf-gcc
#INCLUDES = -I/×××
#LIBS = -L/×××
CCFLAGS =-mthumb -g -Wall -O0
LDFLAGS = -shared -fPIC
injected.so: $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)
%.o : %.c %.s %.cpp
	$(CC) -c $< $(CCFLAGS)
clean:
	rm *.o ./injected.so
.PHONY:clean

CFLAGS += -Wall  -fno-stack-protector -g 
APP=ver_tool

SRC += main.c

all: $(APP)

$(APP): $(SRC)
	gcc $(CFLAGS)  $^ -o $@



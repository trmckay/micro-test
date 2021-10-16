CC := gcc
AR := ar
RL := ranlib

ARCHIVE := utest.a
SO := utest.so

TARGETS := $(ARCHIVE) $(SO)

CFLAGS := -g -Wall -Werror -fPIC

ifeq ($(NO_HEAP),1)
	CFLAGS += -D PRINTF_NO_HEAP
endif

LDFLAGS := -shared

OBJ = utest.o

.PHONY: all
all: $(TARGETS)

$(ARCHIVE): $(OBJ)
	$(AR) r $@ $^
	$(RL) $@

$(SO): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGETS)

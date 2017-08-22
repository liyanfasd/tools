VERSION = 1.0
CC ?= $(CROSS_COMPILE)gcc  
AR ?= $(CROSS_COMPILE)ar rcs  

TARGET		:= mdio
	
SRCS 		:= $(TARGET).c


OBJS 		= $(SRCS:.c=.o)

LIBS		=
CFLAGS 		= -Wall -Wno-nonnull
CPPFLAGS 	= 
LDFLAGS 	=

ifndef DEBUG
	CFLAGS	+= -O2
else
	CFLAGS	+= -O0 -g
endif
  

.PHONY: all install clean help 
all: $(TARGET)

  
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)  
  
%.o:: %.c  
	$(CC) $(CFLAGS) -o $@ -c $<  
  
%.a::  
	$(AR) $@ $^  


install::
	@echo "install by user"	


clean:  
	$(RM) -v *.o *.a $(TARGET)


help:
	@echo ""
	@echo " useful Makefile targets:"
	@echo ""
	@echo "    all       :  build all (default)"
	@echo "    install   :  install to local filesystem"
	@echo "    clean     :  remove object files"
	@echo "    help      :  print this message"


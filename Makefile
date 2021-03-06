# Copyright (c) 2013 ASMlover. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list ofconditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materialsprovided with the
#    distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

OUT_LIB	= libelibpp.a
OUT_BIN	= elibpp
OUT	= $(OUT_LIB) $(OUT_BIN)
RM	= rm
RD	= rm -rf
CC	= g++
AR	= ar
CFLAGS	= -g -O2 -Wall -std=c++0x
LDFLAGS	= -L. -lelibpp -lpthread -lrt
LIBOBJS	= $(patsubst %.cc, %.o, $(wildcard ./posix/*.cc ./linux/*.cc *.cc))
BINOBJS	= $(patsubst %.cc, %.o, $(wildcard ./test/*.cc))
OBJS	= $(LIBOBJS) $(BINOBJS)

all: $(OUT)
rebuild: clean all
clean:
	$(RM) $(OUT) $(OBJS)
clean_all:
	$(RM) $(OUT) $(OBJS) *.log
clean_log:
	$(RD) logging

$(OUT_LIB): $(LIBOBJS)
	$(AR) -cru $@ $^
$(OUT_BIN): $(BINOBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
$(OBJS): %.o:%.cc
	$(CC) -o $*.o -c $(CFLAGS) $^

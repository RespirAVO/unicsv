# RespirAVO/unicsv
# Copyright 2022 3C A. Avogadro & RespirAVO Contributors

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#     http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


CXX = gcc
CC  = $(CROSS_COMPILE)$(CXX)

INCDIR  = include
CFLAGS  = -O3 -g -I $(INCDIR)/intf -I "src/intf"
LFLAGS  = -lm

SRCDIR     = src
OBJDIR     = obj
BINDIR     = bin
OUTFILE    = unicsv
OUTFILEEXT = 

DIR_SEP    = /
rwildcard = $(foreach d, $(wildcard $(1:=/*)), $(call rwildcard ,$d, $2) $(filter $(subst *, %, $2), $d))

INC = $(call rwildcard, $(INCDIR)/impl, *.c)
SRC = $(call rwildcard, $(SRCDIR), *.c)

OBJS  = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(INCDIR)/impl/%.c, $(OBJDIR)/%.o, $(INC))

ifeq ($(OS), Windows_NT)
	OUTFILEEXT = .exe
	DIR_SEP    = \\
endif

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/%.o: $(INCDIR)/impl/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

all: $(OBJS) link

link:
	$(CC) -o $(BINDIR)/$(OUTFILE)$(OUTFILEEXT) $(OBJS) $(LFLAGS)

setup:
	@ mkdir $(BINDIR)
	@ mkdir $(OBJDIR)
	@ mkdir $(OBJDIR)$(DIR_SEP)impl

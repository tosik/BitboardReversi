#
# Makefile
#  author: Hirooka
#  desc: Makefile for bitboard reversi
#

SRCDIR = src
BINDIR = bin

# general settings
CC 	= g++
# CFLAGS 	= -Wall -O2 -ggdb
# CFLAGS	= -Wall -O4
CFLAGS	= -Wall -Winline -O9
LDFLAGS	= -O9
# CALFLAGS= -pg
INCLUDES=
LIBS	=
OBJS	= $(SRCDIR)/main.o $(SRCDIR)/bitboard_reversi.o $(SRCDIR)/bitboard_reversi_functions.o $(SRCDIR)/move_ordering.o

# default settings
ifdef SIZE_X
SIZE_X_ = $(SIZE_X)
else
SIZE_X_ = 4
endif

ifdef SIZE_Y
SIZE_Y_ = $(SIZE_Y)
else
SIZE_Y_ = 4
endif

ifdef CSV
CSV_ = -DVIEW_RESULT_BY_CSV
else
CSV_ = 
endif

ifdef VIEW
VIEW_ = -DVIEW_KIFU_BOARD
else
VIEW_ = 
endif

SIZE	= -DSIZE_X=$(SIZE_X_) -DSIZE_Y=$(SIZE_Y_)
DEFS	= $(SIZE) -DDEPTH_START=$(DEPTH_START) -DDEPTH_END=$(DEPTH_END) -DALGORITHM_NAME=$(ALGORITHM_NAME) $(CSV_) $(VIEW_)


# commands

.PHONY:	all
all:	negamax negaalpha better_negaalpha open_theory_negaalpha value_by_negaalpha_negaalpha expectation_method_negaalpha mnm_negaalpha random_negaalpha ctags docs
#all:	negamax ctags docs

negamax: clean tables
	make clean
	make bbnegamax TARGET=bbnegamax ALGS=-DNEGAMAX SIZE_X=$(SIZE_X_) SIZE_Y=$(SIZE_Y_)

negaalpha: clean tables
	make clean
	make bbnegaalpha TARGET=bbnegaalpha ALGS=-DNEGAALPHA SIZE_X=$(SIZE_X_) SIZE_Y=$(SIZE_Y_)

better_negaalpha: clean tables
	make clean
	make bbbetter_negaalpha TARGET=bbbetter_negaalpha ALGS=-DBETTER_NEGAALPHA SIZE_X=$(SIZE_X_) SIZE_Y=$(SIZE_Y_)

open_theory_negaalpha: clean tables
	make clean
	make bbopen_theory_negaalpha TARGET=bbopen_theory_negaalpha ALGS=-DOPEN_THEORY_NEGAALPHA SIZE_X=$(SIZE_X_) SIZE_Y=$(SIZE_Y_)

value_by_negaalpha_negaalpha: clean tables
	make clean
	make bbvalue_by_negaalpha_negaalpha TARGET=bbvalue_by_negaalpha_negaalpha ALGS=-DVALUE_BY_NEGAALPHA_NEGAALPHA SIZE_X=$(SIZE_X_) SIZE_Y=$(SIZE_Y_)

expectation_method_negaalpha: clean tables
	make clean
	make bbexpectation_method_negaalpha TARGET=bbexpectation_method_negaalpha ALGS=-DEXPECTATION_METHOD_NEGAALPHA SIZE_X=$(SIZE_X_) SIZE_Y=$(SIZE_Y_)

mnm_negaalpha: clean tables
	make clean
	make bbmnm_negaalpha TARGET=bbmnm_negaalpha ALGS=-DMNM_NEGAALPHA SIZE_X=$(SIZE_X_) SIZE_Y=$(SIZE_Y_)

random_negaalpha: clean tables
	make clean
	make bbrandom_negaalpha TARGET=bbrandom_negaalpha ALGS=-DRANDOM_NEGAALPHA SIZE_X=$(SIZE_X_) SIZE_Y=$(SIZE_Y_)

algorithm_definition: clean tables
	make clean
	make bbalgorithm_definition TARGET=bbalgorithm_definition ALGS=-DALGORITHM_DEFINITION SIZE_X=$(SIZE_X_) SIZE_Y=$(SIZE_Y_) DEPTH_START=$(DEPTH_START) DEPTH_END=$(DEPTH_END) CSV=TRUE

tables: $(SRCDIR)/create_tables.rb
	ruby $(SRCDIR)/create_tables.rb > $(SRCDIR)/tables

$(TARGET): $(OBJS)
	@if [ ! -d $(BINDIR) ]; then \
		mkdir $(BINDIR); \
	fi
	$(CC) $(LDFLAGS) $(CALFLAGS) $(ALGS) $(DEFS) -o $(BINDIR)/$@ $(OBJS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(CALFLAGS) $(INCLUDES) $(ALGS) $(DEFS) -c $< -o $@

# header file ‚ÌˆË‘¶ŠÖŒWiŽè“®‚Å‘‚«Š·‚¦‚éj
$(OBJS): $(SRCDIR)/setting.h
$(SRCDIR)/setting.h: $(SRCDIR)/validator.h
$(SRCDIR)/bitboard_reversi_functions.o: $(SRCDIR)/bitboard_reversi_functions.h
$(SRCDIR)/bitboard_reversi.h: $(SRCDIR)/bitboard_reversi_functions.h
$(SRCDIR)/bitboard_reversi.o: $(SRCDIR)/bitboard_reversi.h
$(SRCDIR)/move_ordering.o: $(SRCDIR)/move_ordering.h

.PHONY:	clean
clean:
	rm -f $(OBJS) .nfs* *~ \#* core *.core *.exe

.PHONY:	ctags
ctags:
	ctags -R

.PHONY:	docs
docs:
	doxygen


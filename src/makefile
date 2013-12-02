
#######################################################################
# Tester
#
# Makefile 
#######################################################################

CC=g++
AR=/usr/bin/ar
RL=/urs/bin/ranlib
LD=/usr/bin/ld

SRCDIR := .
INCDIR := .
OBJDIR := ~/Temp/evobj

SRC :=  $(SRCDIR)/main.cpp \
	$(SRCDIR)/event_q.cpp \
	$(SRCDIR)/event.cpp \
	$(SRCDIR)/event_req.cpp \
	$(SRCDIR)/event_res.cpp \
	$(SRCDIR)/event_err.cpp \
	$(SRCDIR)/resolve_manager.cpp \
	$(SRCDIR)/ui.cpp \
	$(SRCDIR)/input_handler.cpp \
	$(SRCDIR)/event_dispatcher.cpp \
	$(SRCDIR)/logger.cpp \
	$(SRCDIR)/resolve_thread.cpp \



EXE := ed
OBJ := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))

CFLAGS := -c -g -O2 $(addprefix -I, $(INCDIR)) -Wall -pg -D_REENTRANT
LDFLAGS := -lboost_thread \
	   -lboost_regex \
	   #-ltcmalloc -ltcmalloc_minimal #-lboost_regex #-lsnmp #-lcrypto

all: $(EXE)

build: clean all
install: clean all

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
		$(CC) $(CFLAGS) -MD -o $@ $<
		@cp $(OBJDIR)/$*.d $(OBJDIR)/$*.P; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(OBJDIR)/$*.d >> $(OBJDIR)/$*.P; \
		rm -f $(OBJDIR)/$*.d


$(EXE) : $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJDIR)/*
	rm -f ed
	rm -f callgrind* massif*

-include $(OBJ:.o=.P)



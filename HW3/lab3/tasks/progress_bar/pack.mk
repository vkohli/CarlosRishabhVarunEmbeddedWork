PROGS_BAR_OBJS := progress_bar.o
PROGS_BAR_OBJS := $(PROGS_BAR_OBJS:%=$(TDIR)/progress_bar/%)
ALL_OBJS += $(PROGS_BAR_OBJS)

$(TDIR)/bin/progress_bar : $(TSTART) $(PROGS_BAR_OBJS) $(TLIBC)


.PHONY : all clean depend

all: $(TARGET)

CLEAN_OBJECTS=$(OBJECTS) $(TARGET)
clean:
	-@echo -@rm $(CLEAN_OBJECTS)
	-@rm $(CLEAN_OBJECTS)

#.deps:
#	mkdir .deps
#
#%.d: %.c:.deps
#	$(CC) $(CFLAGS) -MM $<  >  .deps/$@
#
#%.d: %.cpp:.deps
#	$(CXX) $(CXXFLAGS) -MM $<  >  .deps/$@
#%.d: %.c
#        @set -e; $(CC) -MM $(CPPFLAGS) $< \
#	                | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
#			                [ -s $@ ] || rm -f $@
#

include buildTools/config.mk

SRCDIRS = Util \
	Decoders \
	IO \
	Processors \
	Management \
	Applications

all: 
	@echo Preparing library links...
	mkdir -p lib;\
	 cd lib;\
	 ln -sf ../Util/libORUtil.$(SOSUFFIX) \
		../Decoders/libORDecoders.$(SOSUFFIX) \
		../IO/libORIO.$(SOSUFFIX) \
		../Processors/libORProcessors.$(SOSUFFIX) \
		../Management/libORManagement.$(SOSUFFIX) \
		.;
	@for i in $(SRCDIRS); do (echo Entering directory $$i; $(MAKE) -C $$i) || exit $$?; done

clean: 
	rm -rf lib 
	@for i in $(SRCDIRS); do $(MAKE) -C $$i clean || exit $$?; done


########################################################################	
##
## --------------------
##   Makefile for LEDA
## --------------------
##
## (Version Information)
##   Version: 0.1
##   Release: 1
##
## (Update Record)
##    2005/02/10  K.Fujii	Separated from Satellites
##
## (Description)
##   In order to use this package you should first set some
##   environmental variables. Take a look at README in this
##   directory.
##
## (Targets)
##	all       	: creates libraries in lib.
##	Makefiles 	: creates Makefiles.
##	clean     	: deletes *.o ...
##	distclean 	: deletes even lib/*.
## 
########################################################################	

SHELL	= bash
MFLAGS	=
CURRDIR	= .

SUBDIRS	= src

all: dirs
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS); do \
	(cd $$i; echo ``making'' all ``in $(CURRDIR)/$$i...''; \
	$(MAKE) $(MFLAGS)); \
	done

dirs:
	mkdir -p include
	mkdir -p lib

Makefiles:
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS); do \
	(cd $$i; echo ``making'' Makefiles ``in $(CURRDIR)/$$i...''; \
	$(MAKE) $(MFLAGS) Makefiles); \
	done

clean:
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS); do \
	(cd $$i; echo ``making'' clean ``in $(CURRDIR)/$$i...''; \
	$(MAKE) $(MFLAGS) clean); \
	done

distclean: Makefiles
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS); do \
	(cd $$i; echo ``making'' distclean ``in $(CURRDIR)/$$i...''; \
	$(MAKE) $(MFLAGS) distclean); \
	done
	rm -rf lib include



CC=gcc
LATEXC=pdflatex
DOCC=doxygen
CFLAGS=-g -Wall -std=c99 

REFDIR=.
SRCDIR=$(REFDIR)/src
BINDIR=$(REFDIR)/bin
DOCDIR=$(REFDIR)/doc
REPORTDIR=$(REFDIR)/rapport
LIBDIR=$(REFDIR)/lib

LATEXSOURCE=$(wildcard $(REPORTDIR)/*.tex)
CSOURCEAPPLY=$(wildcard $(SRCDIR)/applyPatch.c);
CSOURCECOMPUTE=$(wildcard $(SRCDIR)/computePatchOpt.c $(SRCDIR)/patchTable.c $(SRCDIR)/stringList.c);
PDF=$(LATEXSOURCE:.tex=.pdf)

JLIBS=$(LIBDIR)/commons-io-2.4/commons-io-2.4.jar


all: binary report doc 

binary: computePatchOpt bin


$(BINDIR)/applyPatch: $(CSOURCEAPPLY)
	$(CC) $(CFLAGS)  $^ -o $@

$(BINDIR)/computePatchOpt: $(CSOURCECOMPUTE)
	$(CC) $(CFLAGS)  $^ -o $@

%.pdf: $(LATEXSOURCE)
	$(LATEXC) -output-directory $(REPORTDIR) $^ 

$(DOCDIR)/index.html: $(SRCDIR)/Doxyfile $(CSOURCE) 
	$(DOCC) $(SRCDIR)/Doxyfile

computePatchOpt: $(BINDIR)/computePatchOpt
	
bin: $(BINDIR)/applyPatch

report: $(PDF) 

doc: $(DOCDIR)/index.html

initScript:
	./initScript.sh

clean:
	rm -rf $(DOCDIR) $(BINDIR)/* $(REPORTDIR)/*.aux $(REPORTDIR)/*.log  $(REPORTDIR)/rapport.pdf 


.PHONY: all doc binary report 

# Author: Ian Gudger, igudger@ucsc.edu
# Adapted from Makefile used in CMPS 12B taught by Wesley Mackey

MKFILE    = Makefile
NOINCLUDE = ci clean spotless
NEEDINCL  = ${filter ${NOINCLUDE}, ${MAKECMDGOALS}}
GMAKE     = gmake --no-print-directory
LEX       = flex

COMPILER  = cc

CSOURCE   = shell.c ${LCSOURCE}
LSOURCE   = lex.c
LCSOURCE  = ${LSOURCE:.c=.yy.c}
CHEADER   = 
OBJECTS   = ${CSOURCE:.c=.o}
EXECBIN   = shell

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${COMPILER} -o $@ ${OBJECTS}

%.o : %.c
	${COMPILER} -c $<

${LCSOURCE} :
	flex ${LSOURCE}

clean :
	- rm ${OBJECTS} ${LCSOURCE}

spotless : clean
	- rm ${EXECBIN}

again :
	${GMAKE} spotless ci all


# Top-level makefile for doorlib for Unix.
# $Id$

include Makefile.global

OTHER		= ChangeLog COPYING FILE_ID.DIZ.in INSTALL.in Makefile \
		  Makefile.global.in README aclocal.m4 \
		  config.h.in configure configure.in door.pc.in install-sh
TARFILE		= ${PACKAGE}-${VERSION}.tar.gz

###############################################################################


all depend:
		for d in ${SUBDIRS}; do (cd $$d && ${MAKE} $@) || exit; done; \

install:
		for d in ${SUBDIRS}; do (cd $$d && ${MAKE} $@) || exit; done
		mkdir -p ${LIBDIR}
		cp door.pc ${LIBDIR}/pkgconfig

uninstall:
		for d in ${SUBDIRS}; do (cd $$d && ${MAKE} $@) || exit; done
		rm -f ${LIBDIR}/pkgconfig/door.pc

dist tar:	${TARFILE}

clean:
		rm -f .filelist core ${TARFILE} config.h
		for d in ${SUBDIRS}; do (cd $$d && ${MAKE} $@) || exit; done;

${TARFILE}:	.filelist
		cd ..; rm -f ${TARFILE}; \
		${TAR} cvTf ./${PACKAGE}-${VERSION}/.filelist - | gzip >${TARFILE}

.filelist filelist:
		(for f in ${OTHER} ;do echo ${PACKAGE}-${VERSION}/$$f; done) >.filelist
		for d in ${SUBDIRS}; do (cd $$d && ${MAKE} filelist && cat filelist >>../.filelist) || exit; done;


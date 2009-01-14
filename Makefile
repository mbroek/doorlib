# Top-level makefile for doorlib for Unix.
# $Id$

include Makefile.global

OTHER		= ChangeLog COPYING FILE_ID.DIZ.in INSTALL.in Makefile \
		  Makefile.global.in README aclocal.m4 \
		  config.h.in configure configure.in door.pc.in install-sh
TARFILE		= ${PACKAGE}-${VERSION}.tar.bz2

###############################################################################


all depend:
		for d in ${SUBDIRS}; do (cd $$d && ${MAKE} $@) || exit; done

install:
		@if [ "`id -un`" != "root" ] ; then \
			echo; echo " Must be root to install!"; echo; exit 3; \
		fi
		for d in ${SUBDIRS}; do (cd $$d && ${MAKE} $@) || exit; done
		mkdir -p ${DESTDIR}${PKGDIR}
		cp door.pc ${DESTDIR}${PKGDIR}

uninstall:
		for d in ${SUBDIRS}; do (cd $$d && ${MAKE} $@) || exit; done
		rm -f ${PKGDIR}/door.pc

dist tar:	${TARFILE}

clean:
		rm -f .filelist core ${TARFILE} config.h
		for d in ${SUBDIRS}; do (cd $$d && ${MAKE} $@) || exit; done;

${TARFILE}:	.filelist
		cd ..; ln -s ${PACKAGE} ${PACKAGE}-${VERSION} ; rm -f ${TARFILE}; \
		${TAR} cvTf ./${PACKAGE}-${VERSION}/.filelist - | bzip2 >${TARFILE} ; \
		rm -f ${PACKAGE}-${VERSION}

.filelist filelist:
		(for f in ${OTHER} ;do echo ${PACKAGE}-${VERSION}/$$f; done) >.filelist
		for d in ${SUBDIRS}; do (cd $$d && ${MAKE} filelist && cat filelist >>../.filelist) || exit; done;


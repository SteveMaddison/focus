#
# This Makefile is used in all directories in which targets
# must be made. It is copied into these directories before
# running make in them from the parent. It is used only to
# link to the global "Makefile.config" file, which defines
# most targets automatically.
#
# Make any local (directory specific) changes or extansions
# in the file "Makefile.dir", which will be included by
# "Makefile.config", should it exist.
#

# Locate the top level directory, which contains "Makefile.config"
TOPDIR := $(shell /bin/sh -c \
'function findup { [ "$$1" = "/" ] || if [ -f $$1/$$2 ] ; then echo $$1 ; \
else findup `dirname $$1` $$2 ; fi }; findup `pwd` Makefile.config;')

# Now we know where it is, include the file
include $(TOPDIR)/Makefile.config


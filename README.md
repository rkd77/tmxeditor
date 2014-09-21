tmxeditor
=========

Text editor for Timex 2048/2068.

Requirements:
* zcc from z88dk (CVS or nightly build)
* zmakebas

Optional:
* check for tests

Build:
* autoreconf --install
* ./configure
* make check
* make -C src -f Makefile.zcc

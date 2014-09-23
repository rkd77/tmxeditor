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

Special keystrokes:
* CAPS SHIFT 1 - Polish letters on/off
* CAPS SHIFT 2 - Caps Lock
* CAPS SHIFT 3 - Page Down
* CAPS SHIFT 4 - Page Up
* CAPS SHIFT 9 - Delete
* SYMBOL SHIFT q - Return to Basic

In Polish letters mode:
* SYMBOL SHIFT a - ą and so on

Encoding is ISO-8859-2 with the newline character code equal 10.

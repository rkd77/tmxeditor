SECTION rodata_user
PUBLIC _fgetc_cons_13
PUBLIC font
PUBLIC _font
PUBLIC _udg

_fgetc_cons_13:
	xor	a
	ld	(23560),a
.getkey1
	ld	a,(23560)
	and	a
	jr	z,getkey1
	ld	l,a
	ld	h,0
	ret

font:
_font:
	INCBIN "F8_32_127.BIN"
_udg:
	INCBIN "F8_128_255.BIN"

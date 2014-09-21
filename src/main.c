#include <stdio.h>
#include "tmxeditor.h"
#include "tmxeditor.c"

unsigned int ybegin = 0;

unsigned char polish;
unsigned char *capital = (unsigned char *)23658;

void
move_cursor(unsigned int y, unsigned int x)
{
	printf("\033[%u;%uH", (y + 1), (x + 1));
} 

unsigned char
polish_letter(unsigned char ch, unsigned char capital)
{
	if (capital)
	{
		switch (ch)
		{
		case 226: //'A':
			return 161;
		case 63://'C':
			return 198;
		case 200://'E':
			return 202;
		case 61://'L':
			return 163;
		case 44://'N':
			return 209;
		case 59://'O':
			return 211;
		case 195://'S':
			return 166;
		case 96://'ziet':
			return 172;
		case 58://'żet':
			return 175;
		default:
			return ch;
		}
	}
	switch (ch)
	{
	case 226://'a':
		return 177;
	case 63://'c':
		return 230;
	case 200://'e':
		return 234;
	case 61://'l':
		return 179;
	case 44://'n':
		return 241;
	case 59://'o':
		return 243;
	case 195://'s':
		return 182;
	case 96://'z':
		return 188;
	case 58://'Z':
		return 191;
	default:
		return ch;
	}
}

unsigned char *
print_line(unsigned char *line, unsigned char begin)
{
	unsigned char i;

	line += begin;

	for (i = begin; i < LINE_LENGTH; ++i)
	{
		if (*line == 10 || *line == '\0')
		{
			break;
		}
		fputc_cons(*line++);
	}
	if (begin && i < (LINE_LENGTH-2))
	{
		i = (LINE_LENGTH-2);
	}
	for (; i < LINE_LENGTH; ++i)
	{
		fputc_cons(' ');
	}
	return *line ? line + 1 : line;
}

void
print_lines(unsigned int from, unsigned int to, unsigned char *text)
{
	unsigned int y;

	for (y = from; y <= to; ++y)
	{
		move_cursor(y, 0);
		text = print_line(text, 0);
	}
}

void
print_status(void)
{
	move_cursor(0, 0);
	printf("\033[7m[%c%c] %u L:[%u+%u %u/%u] *(%u/%u) %u\033[27m    ",
	*capital ? 'C' : 'L', polish ? 'P' : '-', cx, ybegin+1, y - ybegin, y, yend, line + cx - text, end, line[cx]);
	move_cursor((y - ybegin), cx);
	printf("\033[4m%c\033[24m", line[cx] < ' ' ? ' ' : line[cx]);
}

int
main(int argc, char **argv)
{
	unsigned char oldchar = ' ';
	unsigned char oldcx = 0;
	unsigned int oldy = 1;
	unsigned char k;

	printf("\033[2J");
	line = text;
	cx = 0;
	cxend = 0;
	y = 1;
	end = strchr(text, '\0') - text;
	find_nth_line(text, 50000, &yend);
	cursor_position();
	changed_lines = 1;
	changed_one_line = 0;
	changed_pos = 0;

	ybegin = 1;

	while (1)
	{
		if (changed_lines || changed_pos)
		{
			unsigned char *t = text;
			int distance = y - ybegin;

			if (distance >= 23)
			{
				int old_distance = oldy - ybegin;
				if (y - oldy > 1)
				{
					ybegin = y - old_distance;
				}
				else
				{
					ybegin = y - 23;
				}
				t = find_nth_line(text, ybegin + 1, NULL);
				print_lines(1, 23, t);
			}
			else if (distance <= 0)
			{
				if (y > 23)
				{
					ybegin = y - 23;
				}
				else
				{
					ybegin = 0;
				}
				t = find_nth_line(text, ybegin + 1, NULL);
				print_lines(1, 23, t);
			}
			else if (!changed_lines)
			{
				move_cursor(oldy - ybegin, oldcx);
				fputc_cons(oldchar);
			}
			else
			{
				t = find_nth_line(text, y - 1, NULL);
				print_lines(y - 1 - ybegin, 23, t);
			}
		}
		else if (changed_one_line)
		{
			unsigned char begin = cx > 0 ? cx - 1 : 0;

			move_cursor(y - ybegin, begin);
			print_line(line, begin);
		}
		print_status();

		k = 0;
		while (!k)
		{
			k = fgetc_cons();
		}

		changed_one_line = 0;
		changed_lines = 0;
		changed_pos = 0;
		oldy = y;
		oldchar = line[cx] > ' ' ? line[cx] : ' ';
		oldcx = cx;

		switch (k)
		{
		case 8://KEY_LEFT:
			left();
			break;
		case 9://KEY_RIGHT
			right();
			break;
		case 11://KEY_UP:
			up();
			break;
		case 10://KEY_DOWN
			down();
			break;
		case 4://PAGE_DOWN
			page_down();
			break;
		case 5:
			page_up();
			break;
		case 15://KEY_DEL:
			del();
			break;
		case 12://127:
			backspace();
			break;
		case 13://ENTER
			enter();
			break;
		case 14://25:
			delete_line();
			break;
		case 7:
			polish = 1 - polish;
			break;
		case 6:
			*capital = 8 - *capital;
			break;
		case 199:
			wpoke(24576 + 6144, (unsigned int)text);
			wpoke(24576 + 6146, end);
			#asm
			xor a
			out (255), a
			#endasm
			return 0;
		default:
			if (polish) k = polish_letter(k, *capital);
			add((unsigned char)k);
			break;
		}
	}
	return 0;
}

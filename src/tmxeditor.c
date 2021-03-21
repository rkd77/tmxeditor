#include <stdlib.h>
#include <string.h>
#include "tmxeditor.h"

unsigned char text[TEXT_LENGTH] = "";
unsigned char *line;

char cx, cxend;
int end, realend = (TEXT_LENGTH - 1);
int y = 1, yend = 1;
int changed_one_line;
int changed_lines;
int changed_pos;

unsigned char *
find_0(unsigned char *pos)
{
	while (*pos) pos++;

	return pos;
}

unsigned char *
find_10(unsigned char *pos)
{
	while (*pos && *pos != 10) pos++;

	if (*pos) return pos;

	return NULL;
}

unsigned char *
find_10_or_0(unsigned char *pos)
{
	while (*pos && *pos != 10) pos++;

	return pos;
}

void
left(void)
{
	if (cx > 0)
	{
		changed_pos = 1;
		--cx;
	}
}

void
right(void)
{
	if (cx < cxend && cx < LAST_POS_IN_LINE)
	{
		changed_pos = 1;
		++cx;
	}
}

int
add(unsigned char ch)
{
	if (end >= realend)
	{
		return 0;
	}

	if (cx <= cxend && ((cxend < LAST_POS_IN_LINE) || (ch == 10)))
	{
		memmove(line + cx + 1, line + cx, end - cx);
		line[cx] = ch;
		++cx;
		++cxend;
		++end;
		changed_one_line = 1;
		return 1;
	}
	return 0;
}

void
enter(void)
{
	if (add(10))
	{
		line += cx;
		cxend -= cx;
		cx = 0;
		changed_lines = 1;
		++y;
		++yend;
	}
}

void
backspace(void)
{
	if (cx > 0)
	{
		memmove(line + cx - 1, line + cx, end - cx + 1);
		changed_one_line = 1;
		--cx;
		--cxend;
		--end;
	}
}

void
del(void)
{
	if (cx < cxend)
	{
		memmove(line + cx, line + cx + 1, end - cx);
		changed_one_line = 1;
		--cxend;
		--end;
	}
}

static void
up_common(void)
{
	if (y <= 1)
	{
		return;
	}

	--y;
	changed_pos = 1;

	if (y == 1)
	{
		line = text;
	}
	else
	{
		for (--line; *(--line) != 10;);
		++line;
	}
}

void
up(void)
{
	unsigned char *old_line = line - 1;

	if (y <= 1)
	{
		return;
	}

	up_common();

	cxend = old_line - line;
	if (cx > cxend)
	{
		cx = cxend;
	}
}

void
cursor_position(void)
{
	unsigned char *endline = find_10_or_0(line);

	//fprintf(stderr, "cursor_position: endline = %s", endline);
	cxend = endline - line;

	//fprintf(stderr, "cursor_position: cxend = %d cx=%d\n", cxend, cx);
	if (cx > cxend)
	{
		cx = cxend;
	}
}

static void
down_common(void)
{
	if (y >= yend)
	{
		return;
	}
	changed_pos = 1;
	++y;

	//fprintf(stderr, "down_common: before line = %s", line);

	line = find_10_or_0(line) + 1;
	//fprintf(stderr, "down_common: after line = %s", line);
}

void
down(void)
{
	down_common();
	cursor_position();
}

void
page_up(void)
{
	int i;

	if (y <= 23)
	{
		y = 1;
		line = text;
		cursor_position();
		changed_pos = 1;
		return;
	}
	for (i = 0; i < 23; ++i)
	{
		up_common();
	}
	cursor_position();
}

void
page_down(void)
{
	int maks = yend - y;
	int count = maks < 23 ? maks : 23;
	int i;

	for (i = 0; i < count; ++i)
	{
		down_common();
	}
	cursor_position();
}

void
home(void)
{
	cx = 0;
	changed_pos = 1;
}

void
kbend(void)
{
	cx = cxend;
	changed_pos = 1;
}

void
delete_line(void)
{
	if (!(*line))
	{
		return;
	}
	memmove(line, line + cxend + 1, (text + end) - (line + cxend) + 1);
	changed_lines = 1;
	if (y < yend) --yend;
	end -= cxend;
	cursor_position();
}

unsigned char *
find_nth_line(unsigned char *current, unsigned int n, unsigned int *res)
{
	unsigned int i;

	for (i = 1; i < n; ++i)
	{
		unsigned char *next = find_10(current);

		if (!next)
		{
			break;
		}
		current = next + 1;
	}
	if (res)
	{
		*res = i;
	}
	return current;
}

#include <config.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <check.h>
#include "../src/tmxeditor.h"

void setup(void)
{
	cx = 0;
	cxend = 0;
	end = 0;
	realend = (TEXT_LENGTH - 1);
	line = text;
	y = 1;
	yend = 1;
	memset(text, 0, sizeof(text));
}

void teardown(void)
{
	setup();
}

START_TEST(test_left)
{
	memcpy(line, "12345", 5);
	cx = 2;
	left();
	ck_assert_int_eq(cx, 1);
	left();
	ck_assert_int_eq(cx, 0);
	left();
	ck_assert_int_eq(cx, 0);
}
END_TEST

START_TEST(test_right)
{
	memcpy(line, "12345", 5);
	cx = 2;
	end = cxend = 5;
	right();
	ck_assert_int_eq(cx, 3);
	right();
	ck_assert_int_eq(cx, 4);

	end = cxend = 90;
	cx = LAST_POS_IN_LINE - 1;
	right();
	ck_assert_int_eq(cx, LAST_POS_IN_LINE);
	right();
	ck_assert_int_eq(cx, LAST_POS_IN_LINE);

}
END_TEST

START_TEST(test_add_first)
{
	cx = 0;
	cxend = 0;
	end = 0;
	add('a');
	ck_assert_str_eq("a", text);
}
END_TEST

START_TEST(test_add)
{
	memcpy(line, "12345", 5);
	cx = 2;
	cxend = 5;
	end = (TEXT_LENGTH - 2);
	add('v');
	ck_assert_int_eq(cx, 3);
	ck_assert_int_eq(cxend, 6);
	ck_assert_str_eq(line, "12v345");
	ck_assert_int_eq(end, (TEXT_LENGTH - 1));

	add('b');
	ck_assert_int_eq(cx, 3);
	ck_assert_int_eq(cxend, 6);
	ck_assert_str_eq(line, "12v345");
	ck_assert_int_eq(end, (TEXT_LENGTH - 1));
}
END_TEST

START_TEST(test_add_at_end)
{
#if LINE_LENGTH == 80
	unsigned char tmp[90] = "0123456789v012345678901234567890123456789012345678901234567890123456789012345678";

	memcpy(line, "0123456789012345678901234567890123456789012345678901234567890123456789012345678", 80);

	tmp[LAST_POS_IN_LINE] = '\0';
	line[LAST_POS_IN_LINE - 1] = '\0';

	cx = 10;
	end = cxend = LAST_POS_IN_LINE - 1;
	add('v');

	ck_assert_int_eq(cx, 11);
	ck_assert_int_eq(cxend, LAST_POS_IN_LINE);
	ck_assert_str_eq(line, tmp);

	add('v');
	ck_assert_int_eq(cx, 11);
	ck_assert_int_eq(cxend, LAST_POS_IN_LINE);
	ck_assert_str_eq(line, tmp);
#endif
}
END_TEST

START_TEST(test_backspace)
{
	unsigned char tmp[] = "012356789";

	memcpy(line, "0123456789", 10);

	cx = 5;
	end = cxend = 10;

	backspace();
	ck_assert_int_eq(cx, 4);
	ck_assert_int_eq(cxend, 9);
	ck_assert_str_eq(line, tmp);

	cx = 0;
	backspace();
	ck_assert_int_eq(cx, 0);
	ck_assert_int_eq(cxend, 9);
	ck_assert_str_eq(line, tmp);
}
END_TEST

START_TEST(test_delete)
{
	unsigned char tmp[] = "012356789";

	memcpy(line, "0123456789", 10);

	cx = 4;
	end = cxend = 10;

	del();
	ck_assert_int_eq(cx, 4);
	ck_assert_int_eq(cxend, 9);
	ck_assert_str_eq(line, tmp);

	cx = 8;
	del();
	del();
	del();
	ck_assert_int_eq(cx, 8);
	ck_assert_int_eq(cxend, 8);
}
END_TEST

START_TEST(test_enter)
{
	unsigned char tmp[] = "012345\n6789";
	unsigned char tmp2[] = "012345\n 6789";

	memcpy(line, "0123456789", 10);
	cx = 6;
	cxend = end = 10;

	y = 1;
	enter();
	ck_assert_int_eq(cx, 0);
	ck_assert_int_eq(cxend, 4);
	ck_assert_str_eq(line, "6789");
	ck_assert_str_eq(tmp, text);
	ck_assert_int_eq(y, 2);

	add(' ');
	ck_assert_str_eq(tmp2, text);
}
END_TEST

START_TEST(test_up)
{
	memcpy(line, "0123456\n7890", 11);
	line += 8;
	cx = 2;
	cxend = 4;
	end = 12;
	y = 2;
	yend = 2;

	up();
	ck_assert_int_eq(cx, 2);
	ck_assert_int_eq(cxend, 7);
	ck_assert_str_eq(line, text);
	ck_assert_int_eq(y, 1);
}
END_TEST

START_TEST(test_up_2)
{
	unsigned char *line2;

	memcpy(line, "0123456\n7890\n01234567890", 24);
	line = line2 = strchr((char *)line, '\n') + 1;
	line = strchr((char *)line, '\n') + 1;

	//fprintf(stderr, "line before = '%s'\n", line);

	cx = 8;
	cxend = 10;
	end = strchr((char *)text, '\0') - (char *)text;
	y = 3;
	yend = 3;

	up();

	//fprintf(stderr, "line='%s', text='%s', line+cx='%s'\n", line, text, line + cx);
	ck_assert_int_eq(cx, 4);
	ck_assert_int_eq(cxend, 4);
	ck_assert_str_eq(line, line2);
	ck_assert_int_eq(y, 2);
}
END_TEST

START_TEST(test_down)
{
	unsigned char *line2;

	memcpy(line, "01\n012\n0123", 12);
	line = strchr((char *)line, '\n') + 1;
	line2 = strchr(line, '\n') + 1;

	//fprintf(stderr, "text before = '%s'\n", text);
	//fprintf(stderr, "line before = '%s'\n", line);
	//fprintf(stderr, "line2 before = '%s'\n", line2);

	cx = 2;
	cxend = 3;
	end = strchr((char *)text, '\0') - (char *)text;
	y = 2;
	yend = 3;

	down();

	//fprintf(stderr, "line='%s', text='%s', line+cx='%s'\n", line, text, line + cx);
	ck_assert_int_eq(cx, 2);
	ck_assert_int_eq(cxend, 4);
	ck_assert_str_eq(line, line2);
	ck_assert_int_eq(y, 3);
}
END_TEST

START_TEST(test_page_up)
{
	unsigned char *line2;

	memcpy(line, "01\n012\n0123", 12);
	line = strchr((char *)line, '\n') + 1;

	//fprintf(stderr, "text before = '%s'\n", text);
	//fprintf(stderr, "line before = '%s'\n", line);
	//fprintf(stderr, "line2 before = '%s'\n", line2);

	cx = 2;
	cxend = 3;
	end = strchr((char *)text, '\0') - (char *)text;
	y = 2;
	yend = 3;

	page_up();

	//fprintf(stderr, "line='%s', text='%s', line+cx='%s'\n", line, text, line + cx);
	ck_assert_int_eq(cx, 2);
	ck_assert_int_eq(cxend, 2);
	ck_assert_str_eq(line, text);
	ck_assert_int_eq(y, 1);
}
END_TEST

START_TEST(test_page_up_2)
{
	unsigned char *line2;

	char t[] = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n0";

	memcpy(line, t, sizeof(t));
	line = text + 26 * 2;
	line2 = text + 3 * 2;
	y = 26;
	yend = 40;

	//fprintf(stderr, "text before = '%s'\n", text);
	//fprintf(stderr, "line before = '%s'\n", line);
	//fprintf(stderr, "line2 before = '%s'\n", line2);

	cx = 0;
	cxend = 1;
	end = strchr((char *)text, '\0') - (char *)text;

	page_up();
	//fprintf(stderr, "line='%s', text='%s', line+cx='%s'\n", line, text, line + cx);
	ck_assert_int_eq(cx, 0);
	ck_assert_int_eq(cxend, 1);
	ck_assert_str_eq(line, line2);
	ck_assert_int_eq(y, 3);
}
END_TEST

START_TEST(test_page_down)
{
	unsigned char *line2;

	char t[] = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n0";

	memcpy(line, t, sizeof(t));
	line = text + 26 * 2;
	line2 = text + 40 * 2;
	y = 26;
	yend = 40;

	//fprintf(stderr, "text before = '%s'\n", text);
	//fprintf(stderr, "line before = '%s'\n", line);
	//fprintf(stderr, "line2 before = '%s'\n", line2);

	cx = 0;
	cxend = 1;
	end = strchr((char *)text, '\0') - (char *)text;

	page_down();
	//fprintf(stderr, "line='%s', text='%s', line+cx='%s'\n", line, text, line + cx);
	ck_assert_int_eq(cx, 0);
	ck_assert_int_eq(cxend, 1);
	ck_assert_str_eq(line, line2);
	ck_assert_int_eq(y, 40);
}
END_TEST

START_TEST(test_page_down_2)
{
	char t[] = "\n12345\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

	memcpy(line, t, sizeof(t));
	line = text + 1;
	y = 1;
	yend = 40;

	//fprintf(stderr, "text before = '%s'\n", text);
	//fprintf(stderr, "line before = '%s'\n", line);
	//fprintf(stderr, "line2 before = '%s'\n", line2);

	cx = 3;
	cxend = 5;
	end = strchr((char *)text, '\0') - (char *)text;

	page_down();
	//fprintf(stderr, "line='%s', text='%s', line+cx='%s'\n", line, text, line + cx);
	ck_assert_int_eq(cx, 0);
	ck_assert_int_eq(cxend, 0);
	ck_assert_int_eq(y, 24);
}
END_TEST

START_TEST(test_delete_line)
{
	char t[] = "12345\n23456\n34567";
	char *tmp = "12345\n34567";

	memcpy(line, t, sizeof(t));
	line = strchr(line, '\n') + 1;
	y = 2;
	yend = 3;
	cx = 3;
	cxend = 5;
	end = strchr((char *)text, '\0') - (char *)text;

	delete_line();
	ck_assert_int_eq(y, 2);
	ck_assert_int_eq(yend, 2);
	ck_assert_str_eq(text, tmp);
}
END_TEST

START_TEST(test_delete_line_2)
{
	char t[] = "12345";
	char *tmp = "";

	memcpy(line, t, sizeof(t));
	y = 1;
	yend = 1;
	cx = 2;
	cxend = 5;
	end = strchr((char *)text, '\0') - (char *)text;

	delete_line();
	ck_assert_int_eq(y, 1);
	ck_assert_int_eq(yend, 1);
	ck_assert_str_eq(text, tmp);

	delete_line();
	ck_assert_int_eq(y, 1);
	ck_assert_int_eq(yend, 1);
	ck_assert_str_eq(text, tmp);
}
END_TEST

START_TEST(test_123)
{
	add('1');
	add('2');
	add('3');
	//fprintf(stderr, "t=%s\n", text);
	enter();
	enter();
	ck_assert_int_eq(cx, 0);
	//fprintf(stderr, "t=%s\n", text);
	up();
	ck_assert_int_eq(cx, 0);
	//fprintf(stderr, "line=%s\n", line);
	up();
	ck_assert_int_eq(cx, 0);
	//fprintf(stderr, "line=%s\n", line);
}
END_TEST

START_TEST(test_down_22)
{
	add('1');
	add('2');
	add('3');
	enter();
	up();
	kbend();
	down();
	ck_assert_str_eq(text, "123\n");
	ck_assert_int_eq(cxend, 0);
	ck_assert_int_eq(cx, 0);
}
END_TEST

START_TEST(test_enter_at_full_line)
{
#if LINE_LENGTH == 80
	memcpy(line,
	"0123456789"
	"0123456789"
	"0123456789"
	"0123456789"
	"0123456789"
	"0123456789"
	"0123456789"
	"012345678", 80);
	cx = LAST_POS_IN_LINE;
	cxend = LAST_POS_IN_LINE;
	end = strchr((char *)text, '\0') - (char *)text;

	enter();
	ck_assert_int_eq(cx, 0);
	ck_assert_int_eq(cxend, 0);
	ck_assert_str_eq(text,
	"0123456789"
	"0123456789"
	"0123456789"
	"0123456789"
	"0123456789"
	"0123456789"
	"0123456789"
	"012345678\n");
#endif
}
END_TEST

START_TEST(test_find_nth_line)
{
	unsigned char *nth_line;
	unsigned int res = 0;
	unsigned char t[] =
	"1 2\n"
	"2 3\n"
	"3 4\n"
	"4 5\n"
	"5 6\n"
	"6 7\n"
	"7 8\n"
	"8 9\n"
	"9 10\n"
	"10 11\n"
	"11 12\n"
	"12 13\n"
	"13 14\n"
	"14 15\n"
	"15 16\n"
	"16 17\n"
	"17 18\n"
	"18 19\n"
	"19 20\n"
	"20 21\n"
	"21 22\n"
	"22 23\n"
	"23 24\n"
	"24 25\n"
	"25 26\n"
	"26 27\n"
	"27 28\n"
	"28 29\n"
	"29 30\n"
	"30 31\n";

	memcpy(line, t, sizeof(t));
	end = strchr((char *)text, '\0') - (char *)text;
	cx = 0;
	cxend = 2;

	nth_line = find_nth_line(text, 25, &res);
	ck_assert_str_eq(nth_line,
	"25 26\n"
	"26 27\n"
	"27 28\n"
	"28 29\n"
	"29 30\n"
	"30 31\n");
	ck_assert_int_eq(res, 25);
}
END_TEST

Suite * tmxeditor_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("Tmxeditor");

	/* Core test case */
	tc_core = tcase_create("Core");

	tcase_add_checked_fixture(tc_core, setup, teardown);
	tcase_add_test(tc_core, test_left);
	tcase_add_test(tc_core, test_right);
	tcase_add_test(tc_core, test_add);
	tcase_add_test(tc_core, test_add_at_end);
	tcase_add_test(tc_core, test_backspace);
	tcase_add_test(tc_core, test_delete);
	tcase_add_test(tc_core, test_enter);
	tcase_add_test(tc_core, test_up);
	tcase_add_test(tc_core, test_up_2);
	tcase_add_test(tc_core, test_down);
	tcase_add_test(tc_core, test_page_up);
	tcase_add_test(tc_core, test_page_up_2);
	tcase_add_test(tc_core, test_page_down);
	tcase_add_test(tc_core, test_page_down_2);
	tcase_add_test(tc_core, test_delete_line);
	tcase_add_test(tc_core, test_delete_line_2);
	tcase_add_test(tc_core, test_add_first);
	tcase_add_test(tc_core, test_123);
	tcase_add_test(tc_core, test_down_22);
	tcase_add_test(tc_core, test_enter_at_full_line);
	tcase_add_test(tc_core, test_find_nth_line);
	suite_add_tcase(s, tc_core);

	return s;
}

int main(void)
{
	int number_failed;
	Suite *s;
	SRunner *sr;

	s = tmxeditor_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

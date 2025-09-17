#include "vbc.h"
#include <stdio.h>
#include <stdlib.h>

int g_tests_failed = 0;
int g_tests_passed = 0;

typedef int (*test_func)(void);

void run_test(const char *name, test_func f)
{
	printf("🔹 %s ... ", name);
	fflush(stdout);

	if (f() == 0) {
		printf("✅\n");
		g_tests_passed++;
	} else {
		printf("❌\n");
		g_tests_failed++;
	}
}

/* ---------------- BASE TESTS ---------------- */

int test_simple_addition(void) {
	node *tree = parse_expr("1+2");
	if (!tree) return 1;
	int result = eval_tree(tree);
	destroy_tree(tree);
	return (result == 3 ? 0 : 1);
}

int test_simple_multiplication(void) {
	node *tree = parse_expr("3*4");
	if (!tree) return 1;
	int result = eval_tree(tree);
	destroy_tree(tree);
	return (result == 12 ? 0 : 1);
}

int test_operator_precedence(void) {
	node *tree = parse_expr("2+3*4");
	if (!tree) return 1;
	int result = eval_tree(tree);
	destroy_tree(tree);
	return (result == 14 ? 0 : 1); // 2 + (3*4)
}

int test_parentheses(void) {
	node *tree = parse_expr("(2+3)*4");
	if (!tree) return 1;
	int result = eval_tree(tree);
	destroy_tree(tree);
	return (result == 20 ? 0 : 1);
}

int test_nested_parentheses(void) {
	node *tree = parse_expr("((1+3)*2+(2*5))");
	if (!tree) return 1;
	int result = eval_tree(tree);
	destroy_tree(tree);
	return (result == 18 ? 0 : 1); // (4*2 + 10)
}

int test_unexpected_token(void) {
	node *tree = parse_expr("(1+2*3");
	if (tree) {
		destroy_tree(tree);
		return 1; // should fail!
	}
	return 0;
}

/* ---------------- COMPLEX TESTS ---------------- */

int test_mixed_ops(void) {
	node *tree = parse_expr("2*3+4*5+6");
	if (!tree) return 1;
	int result = eval_tree(tree);
	destroy_tree(tree);
	// (2*3) + (4*5) + 6 = 6 + 20 + 6 = 32
	return (result == 32 ? 0 : 1);
}

/* ---------------- ERROR CASES ---------------- */

int test_extra_closing_parenthesis(void) {
	node *tree = parse_expr("1+2)");
	if (tree) {
		destroy_tree(tree);
		return 1; // should fail!
	}
	return 0;
}

int test_invalid_characters(void) {
	node *tree = parse_expr("2+3a");
	if (tree) {
		destroy_tree(tree);
		return 1; // should fail!
	}
	return 0;
}

/* ---------------- MAIN ---------------- */

int main(void)
{
	printf("🧪 VBC TESTER (Arithmetic Expression Parser)\n");
	printf("===========================================\n\n");

	/* Base tests */
	run_test("Simple addition (1+2)", test_simple_addition);
	run_test("Simple multiplication (3*4)", test_simple_multiplication);
	run_test("Operator precedence (2+3*4)", test_operator_precedence);
	run_test("Parentheses ((2+3)*4)", test_parentheses);
	run_test("Nested parentheses", test_nested_parentheses);
	run_test("Unexpected token error", test_unexpected_token);

	/* Complex expressions */
	run_test("Mixed ops (2*3+4*5+6)", test_mixed_ops);

	/* Error handling */
	run_test("Extra closing parenthesis (1+2))", test_extra_closing_parenthesis);
	run_test("Invalid characters (2+3a)", test_invalid_characters);

	printf("\n📊 RESULTS:\n");
	printf("   Passed: %d\n", g_tests_passed);
	printf("   Failed: %d\n", g_tests_failed);

	if (g_tests_failed == 0) {
		printf("✅ ALL TESTS PASSED! 🎉\n");
		return 0;
	} else {
		printf("❌ Some tests failed. Review your parser.\n");
		return 1;
	}
}

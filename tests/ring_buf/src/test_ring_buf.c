#include <zephyr/ztest.h>
#include <errno.h>
#include <ring_buf.h>

static void *ring_buf_setup(void)
{
	return NULL;
}

static void ring_buf_before(void *fixture)
{
	ARG_UNUSED(fixture);
	rb_init(4);
}

/* ========================================================================= */
/* Suite 1: ring_buf_init                                                    */
/* ========================================================================= */

/**
 * @brief Fresh state test (provided example).
 */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer should be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count should be 0");
}

/**
 * @brief Test that reinitializing an existing ring_buf resets its state.
 */
ZTEST(ring_buf_init, test_reinit_clears_state)
{
	zassert_equal(rb_push(99), 0, "Push failed");
	zassert_equal(rb_init(4), 0, "Reinit failed");

	zassert_true(rb_is_empty(), "Buffer should be empty after reinit");
	zassert_equal(rb_count(), 0, "Count should be 0 after reinit");
}

/* ========================================================================= */
/* Suite 2: ring_buf_push_pop                                                */
/* ========================================================================= */

/**
 * @brief Test single push followed by a pop.
 */
ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	int v = 0;

	zassert_equal(rb_push(42), 0, "Push failed");
	zassert_equal(rb_pop(&v), 0, "Pop failed");
	zassert_equal(v, 42, "Popped value should be 42");
	zassert_true(rb_is_empty(), "Buffer should be empty");
}

/**
 * @brief Test FIFO order preservation across multiple elements.
 */
ZTEST(ring_buf_push_pop, test_fifo_order)
{
	int v = 0;

	zassert_equal(rb_push(1), 0, "Push 1 failed");
	zassert_equal(rb_push(2), 0, "Push 2 failed");
	zassert_equal(rb_push(3), 0, "Push 3 failed");

	zassert_equal(rb_pop(&v), 0, "Pop 1 failed");
	zassert_equal(v, 1, "First value should be 1");

	zassert_equal(rb_pop(&v), 0, "Pop 2 failed");
	zassert_equal(v, 2, "Second value should be 2");

	zassert_equal(rb_pop(&v), 0, "Pop 3 failed");
	zassert_equal(v, 3, "Third value should be 3");

	zassert_true(rb_is_empty(), "Buffer should be empty after popping all elements");
}

/**
 * @brief Test pushing into a completely full buffer fails with -ENOSPC.
 */
ZTEST(ring_buf_push_pop, test_push_full)
{
	for (int i = 1; i <= 4; i++) {
		zassert_equal(rb_push(i), 0, "Push %d failed", i);
	}

	zassert_true(rb_is_full(), "Buffer should be full");
	zassert_equal(rb_push(99), -ENOSPC, "Push past capacity should return -ENOSPC");
	zassert_equal(rb_count(), 4, "Count should remain 4");
}

/* ========================================================================= */
/* Suite 3: ring_buf_boundaries                                              */
/* ========================================================================= */

/**
 * @brief Test peek reads data without consuming it.
 */
ZTEST(ring_buf_boundaries, test_peek_non_consuming)
{
	int v = 0;

	zassert_equal(rb_push(7), 0, "Push failed");

	zassert_equal(rb_peek(&v), 0, "First peek failed");
	zassert_equal(v, 7, "First peek expected 7");

	v = 0;
	zassert_equal(rb_peek(&v), 0, "Second peek failed");
	zassert_equal(v, 7, "Second peek expected 7");

	zassert_equal(rb_count(), 1, "Count should remain 1 after peeking");
}

/**
 * @brief Test rb_pop with NULL pointer returns -EINVAL.
 */
ZTEST(ring_buf_boundaries, test_pop_null)
{
	zassert_equal(rb_pop(NULL), -EINVAL, "rb_pop(NULL) should return -EINVAL");
}

/**
 * @brief Test is_full reports true exactly when filled to capacity.
 */
ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	for (int i = 1; i <= 4; i++) {
		zassert_equal(rb_push(i), 0, "Push %d failed", i);
	}

	zassert_true(rb_is_full(), "Buffer should be full");
	zassert_equal(rb_count(), 4, "Count should be 4");
}

/* Register test suites */
ZTEST_SUITE(ring_buf_init, NULL, ring_buf_setup, ring_buf_before, NULL, NULL);
ZTEST_SUITE(ring_buf_push_pop, NULL, ring_buf_setup, ring_buf_before, NULL, NULL);
ZTEST_SUITE(ring_buf_boundaries, NULL, ring_buf_setup, ring_buf_before, NULL, NULL);
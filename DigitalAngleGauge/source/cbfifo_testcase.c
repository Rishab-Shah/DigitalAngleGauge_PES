/*
 * cbfifo_testcase.h
 *
 *      Author: Howdy
 */
/*******************************************************************************
Header files
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "cbfifo.h"
#include "uart.h"
/*******************************************************************************
GLobal variables
*******************************************************************************/
int g_tests_passed = 0;
int g_tests_total = 0;

CB_t cb_test = {.size = CBFIFO_SIZE, .read = 0, .write = 0, .tracker = 0};
/*******************************************************************************
Function macro
*******************************************************************************/
#define test_assert(value) {                                            \
  g_tests_total++;                                                      \
  if (value) {                                                          \
    g_tests_passed++;                                                   \
  } else {                                                              \
    printf("ERROR: test failure at line %d\n", __LINE__);               \
  }                                                                     \
}

#define test_equal(value1, value2) {                                    \
  g_tests_total++;                                                      \
  long res1 = (long)(value1);                                           \
  long res2 = (long)(value2);                                           \
  if (res1 == res2) {                                                   \
    g_tests_passed++;                                                   \
  } else {                                                              \
    printf("ERROR: test failure at line %d: %ld != %ld\r\n", __LINE__, res1, res2); \
  }                                                                     \
}

/*******************************************************************************
Header files
*******************************************************************************/
/*******************************************************************************
* @Function Name: test_cbfifo_one_iteration
* @Description: consists of various test cases which will be executed
* @input param : None
* @return: None
*******************************************************************************/
static void test_cbfifo_one_iteration()
{
  char *str ="To be, or not to be: that is the question:\n"
    "Whether 'tis nobler in the mind to suffer\n"
    "The slings and arrows of outrageous fortune,\n"
    "Or to take arms against a sea of troubles,\n"
    "And by opposing end them? To die, to sleep--\n"
    "No more--and by a sleep to say we end\n"
    "The heart-ache and the thousand natural shocks\n"
    "That flesh is heir to, 'tis a consummation\n"
    "Devoutly to be wish'd. To die, to sleep;\n"
    "To sleep: perchance to dream: ay, there's the rub;\n"
    "For in that sleep of death what dreams may come\n"
    "When we have shuffled off this mortal coil,\n"
    "Must give us pause.";


  char buf[1024];    //prof
  const int cap = cbfifo_capacity(&cb_test);
  size_t ret = 0;

  // the asserts in the following 2 lines are there because they're
  // not testing the student, they're validating that the test is
  // correct
  assert(strlen(str) >= cap*2);
  assert(sizeof(buf) > cap);
  test_assert(cap == 256 || cap == 255);

  test_equal(cbfifo_length(&cb_test), 0);
  test_equal(cbfifo_dequeue(&cb_test,buf, cap), 0);
  test_equal(cbfifo_dequeue(&cb_test,buf, 2), 0);

  // enqueue 10 bytes, then dequeue same amt
  test_equal(cbfifo_enqueue(&cb_test,str, 10), 10);
  test_equal(cbfifo_length(&cb_test), 10);
  test_equal(cbfifo_dequeue(&cb_test,buf, 10), 10);
  test_equal(strncmp(buf, str, 10), 0);
  test_equal(cbfifo_length(&cb_test), 0);

  // enqueue 20 bytes;  dequeue 5, then another 20
  test_equal(cbfifo_enqueue(&cb_test,str, 20), 20);
  test_equal(cbfifo_length(&cb_test), 20);
  test_equal(cbfifo_dequeue(&cb_test,buf, 5), 5);
  test_equal(cbfifo_length(&cb_test), 15);
  test_equal(cbfifo_dequeue(&cb_test,buf+5, 20), 15);
  test_equal(cbfifo_length(&cb_test), 0);
  test_equal(strncmp(buf, str, 20), 0);

  // fill buffer and then read it back out
  test_equal(cbfifo_enqueue(&cb_test,str, cap), cap);
  test_equal(cbfifo_length(&cb_test), cap);
  ret = cbfifo_enqueue(&cb_test,str, 1);
  test_assert(ret == 0 || (int)ret == -1);
  test_equal(cbfifo_dequeue(&cb_test,buf, cap), cap);
  test_equal(cbfifo_length(&cb_test), 0);
  test_equal(strncmp(buf, str, cap), 0);

  // Add 20 bytes and pull out 18
  test_equal(cbfifo_enqueue(&cb_test,str, 20), 20);
  test_equal(cbfifo_length(&cb_test), 20);
  test_equal(cbfifo_dequeue(&cb_test,buf, 18), 18);
  test_equal(cbfifo_length(&cb_test), 2);
  test_equal(strncmp(buf, str, 18), 0);

  // Now add a bunch of data in 4 chunks
  int chunk_size = (cap-2) / 4;
  for (int i=0; i<4; i++) {
    test_equal(cbfifo_enqueue(&cb_test,str+i*chunk_size, chunk_size), chunk_size);
    test_equal(cbfifo_length(&cb_test), (i+1)*chunk_size + 2);
  }
  test_equal(cbfifo_length(&cb_test), 4*chunk_size + 2);

  // Take out the 2 remaining bytes from above
  test_equal(cbfifo_dequeue(&cb_test,buf, 2), 2);
  test_equal(strncmp(buf, str+18, 2), 0);

  // now read those chunks out a byte at a time
  for (int i=0; i<chunk_size*4; i++) {
    test_equal(cbfifo_dequeue(&cb_test,buf+i, 1), 1);
    test_equal(cbfifo_length(&cb_test), chunk_size*4 - i - 1);
  }
  test_equal(strncmp(buf, str, chunk_size*4), 0);

  // write more than capacity
  test_equal(cbfifo_enqueue(&cb_test,str, 65), 65);
  test_equal(cbfifo_enqueue(&cb_test,str+65, cap), cap-65);
  test_equal(cbfifo_length(&cb_test), cap);
  test_equal(cbfifo_dequeue(&cb_test,buf, cap), cap);
  test_equal(cbfifo_length(&cb_test), 0);
  test_equal(strncmp(buf, str, cap), 0);

  // write zero bytes
  test_equal(cbfifo_enqueue(&cb_test,str, 0), 0);
  test_equal(cbfifo_length(&cb_test), 0);

  // Exercise the following conditions:
  //    enqueue when read < write:
  //        bytes < CAP-write  (1)
  //        bytes exactly CAP-write  (2)
  //        bytes > CAP-write but < space available (3)
  //        bytes exactly the space available (4)
  //        bytes > space available (5)
  test_equal(cbfifo_enqueue(&cb_test,str, 32), 32);  // advance so that read < write
  test_equal(cbfifo_length(&cb_test), 32);
  test_equal(cbfifo_dequeue(&cb_test,buf, 16), 16);
  test_equal(cbfifo_length(&cb_test), 16);
  test_equal(strncmp(buf, str, 16), 0);

  test_equal(cbfifo_enqueue(&cb_test,str+32, 32), 32);  // (1)
  test_equal(cbfifo_length(&cb_test), 48);
  test_equal(cbfifo_enqueue(&cb_test,str+64, cap-64), cap-64);  // (2)
  test_equal(cbfifo_length(&cb_test), cap-16);
  test_equal(cbfifo_dequeue(&cb_test,buf+16, cap-16), cap-16);
  test_equal(strncmp(buf, str, cap), 0);

  test_equal(cbfifo_enqueue(&cb_test,str, 32), 32);  // advance so that read < write
  test_equal(cbfifo_length(&cb_test), 32);
  test_equal(cbfifo_dequeue(&cb_test,buf, 16), 16);
  test_equal(cbfifo_length(&cb_test), 16);
  test_equal(strncmp(buf, str, 16), 0);

  test_equal(cbfifo_enqueue(&cb_test,str+32, cap-20), cap-20);  // (3)
  test_equal(cbfifo_length(&cb_test), cap-4);
  test_equal(cbfifo_dequeue(&cb_test,buf, cap-8), cap-8);
  test_equal(strncmp(buf, str+16, cap-8), 0);
  test_equal(cbfifo_length(&cb_test), 4);
  test_equal(cbfifo_dequeue(&cb_test,buf, 8), 4);
  test_equal(strncmp(buf, str+16+cap-8, 4), 0);
  test_equal(cbfifo_length(&cb_test), 0);

  test_equal(cbfifo_enqueue(&cb_test,str, 49), 49);  // advance so that read < write
  test_equal(cbfifo_length(&cb_test), 49);
  test_equal(cbfifo_dequeue(&cb_test,buf, 16), 16);
  test_equal(cbfifo_length(&cb_test), 33);
  test_equal(strncmp(buf, str, 16), 0);

  test_equal(cbfifo_enqueue(&cb_test,str+49, cap-33), cap-33);  // (4)
  test_equal(cbfifo_length(&cb_test), cap);
  test_equal(cbfifo_dequeue(&cb_test,buf, cap), cap);
  test_equal(cbfifo_length(&cb_test), 0);
  test_equal(strncmp(buf, str+16, cap), 0);

  test_equal(cbfifo_enqueue(&cb_test,str, 32), 32);  // advance so that read < write
  test_equal(cbfifo_length(&cb_test), 32);
  test_equal(cbfifo_dequeue(&cb_test,buf, 16), 16);
  test_equal(cbfifo_length(&cb_test), 16);
  test_equal(strncmp(buf, str, 16), 0);

  test_equal(cbfifo_enqueue(&cb_test,str+32, cap), cap-16);  // (5)
  test_equal(cbfifo_dequeue(&cb_test,buf, 1), 1);
  test_equal(cbfifo_length(&cb_test), cap-1);
  test_equal(cbfifo_dequeue(&cb_test,buf+1, cap-1), cap-1);
  test_equal(cbfifo_length(&cb_test), 0);
  test_equal(strncmp(buf, str+16, cap), 0);

  //    enqueue when write < read:
  //        bytes < read-write (6)
  //        bytes exactly read-write (= the space available) (7)
  //        bytes > space available (8)

  int wpos=0, rpos=0;
  test_equal(cbfifo_enqueue(&cb_test,str, cap-4), cap-4);
  wpos += cap-4;
  test_equal(cbfifo_length(&cb_test), cap-4);
  test_equal(cbfifo_dequeue(&cb_test,buf, 32), 32);
  rpos += 32;
  test_equal(cbfifo_length(&cb_test), cap-36);
  test_equal(strncmp(buf, str, 32), 0);
  test_equal(cbfifo_enqueue(&cb_test,str+wpos, 12), 12);
  wpos += 12;
  test_equal(cbfifo_length(&cb_test), cap-24);

  test_equal(cbfifo_enqueue(&cb_test,str+wpos, 16), 16);  // (6)
  test_equal(cbfifo_length(&cb_test), cap-8);
  test_equal(cbfifo_dequeue(&cb_test,buf, cap), cap-8);
  test_equal(cbfifo_length(&cb_test), 0);
  test_equal(strncmp(buf, str+rpos, cap-8), 0);

  // reset
  wpos=0;
  rpos=0;
  test_equal(cbfifo_enqueue(&cb_test,str, cap-4), cap-4);
  wpos += cap-4;
  test_equal(cbfifo_length(&cb_test), cap-4);
  test_equal(cbfifo_dequeue(&cb_test,buf, 32), 32);
  rpos += 32;
  test_equal(cbfifo_length(&cb_test), cap-36);
  test_equal(strncmp(buf, str, 32), 0);
  test_equal(cbfifo_enqueue(&cb_test,str+wpos, 12), 12);
  wpos += 12;
  test_equal(cbfifo_length(&cb_test), cap-24);

  test_equal(cbfifo_enqueue(&cb_test,str+wpos, 24), 24);  // (7)
  test_equal(cbfifo_length(&cb_test), cap);
  test_equal(cbfifo_dequeue(&cb_test,buf, cap), cap);
  test_equal(cbfifo_length(&cb_test), 0);
  test_equal(strncmp(buf, str+rpos, cap), 0);

  // reset
  wpos=0;
  rpos=0;
  test_equal(cbfifo_enqueue(&cb_test,str, cap-4), cap-4);
  wpos += cap-4;
  test_equal(cbfifo_length(&cb_test), cap-4);
  test_equal(cbfifo_dequeue(&cb_test,buf, 32), 32);
  rpos += 32;
  test_equal(cbfifo_length(&cb_test), cap-36);
  test_equal(strncmp(buf, str, 32), 0);
  test_equal(cbfifo_enqueue(&cb_test,str+wpos, 12), 12);
  wpos += 12;
  test_equal(cbfifo_length(&cb_test), cap-24);

  test_equal(cbfifo_enqueue(&cb_test,str+wpos, 64), 24);  // (8)
  test_equal(cbfifo_length(&cb_test), cap);
  test_equal(cbfifo_dequeue(&cb_test,buf, cap), cap);
  test_equal(cbfifo_length(&cb_test), 0);
  test_equal(strncmp(buf, str+rpos, cap), 0);
}

/*******************************************************************************
* @Function Name: test_cbfifo
* @Description: executes the test cycle
* @input param : None
* @return: None
*******************************************************************************/
void test_cbfifo()
{
  g_tests_passed = 0;
  g_tests_total = 0;

  test_cbfifo_one_iteration();

  printf("%s: passed %d/%d test cases\r\n", __FUNCTION__,
      g_tests_passed, g_tests_total);
}

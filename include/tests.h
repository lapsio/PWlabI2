#ifndef TESTS_H
#define TESTS_H

#define _DEBUG

void test_punkt();
void test_punkt_changeTo(long double k , long double l);
void test_punkt_konstruktor();
void test_punkt_moveBy(long double k,long double l);
void test_punkt_operatory(long double ax, long double ay, long double bx , long double by);

void test_wektor();
void test_wektor_konstruktory();
void test_wektor_set();
void test_wektor_wymiary(long double k, long double l);
void test_wektor_move(long double k, long double l);
void test_wektor_centerOn();

void CORE_DEBUG();

#endif // TESTS_H

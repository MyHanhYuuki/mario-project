#pragma once

#include <Windows.h>

#include <signal.h>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <vector>


using namespace std;

vector<string> split(string line, string delimeter = "\t");
vector<string> Split(string line, string delimeter = "\t");
wstring ToWSTR(string st);

LPCWSTR ToLPCWSTR(string st);

template <typename T>
T Clamp(T val, T min, T max) {
	return val >= max ? max : val <= min ? min : val;
}
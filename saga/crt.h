// Copyright 2013 by Pavel Chistyakov
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#pragma once

extern wchar_t				chupper(wchar_t c); // to upper reg
extern wchar_t				chlower(wchar_t c); // to lower reg
const wchar_t*				gstr(int id); // get name of string token (export)
extern bool					ischa(int c); // is alphabetical character?
int							isqrt(int num); // square root number
int							lasterror(); // get last command error
extern int					locale; // current locale (defined external)
void						rmcpy(void* p1, const void* p2, int count);
void						rmmov(void* p1, const void* p2, int count);
void						rmset(void* p1, int value, int count);
int							rmcmp(const void* p1, const void* p2, int count);
void*						rmresize(void* ptr, unsigned size);
unsigned					rmsize(void* ptr);
void						sleep(int millisecond);
const wchar_t*				szchr(const wchar_t* p1, wchar_t ch);
void						szcpy(char* p1, const char* p2);
void						szcpy(char* p1, const char* p2, int max_count);
void						szcpy(wchar_t* p1, const wchar_t* p2);
void						szcpy(wchar_t* p1, const wchar_t* p2, int max_count);
const wchar_t*				szcmd();
int							szcmp(const char* p1, const char* p2);
int							szcmp(const char* p1, const char* p2, int count);
int							szcmp(const wchar_t* p1, const wchar_t* p2);
int							szcmp(const wchar_t* p1, const wchar_t* p2, int count);
int							szcmpi(const char* p1, const char* p2);
int							szcmpi(const char* p1, const char* p2, int count);
int							szcmpi(const wchar_t* p1, const wchar_t* p2);
int							szcmpi(const wchar_t* p1, const wchar_t* p2, int count);
const wchar_t*				szdup(const wchar_t *text);
char*						szend(char* text);
wchar_t*					szend(wchar_t* text);
inline const wchar_t*		szend(const wchar_t* text) { while(*text) text++; return text; }
extern int					szerror(wchar_t* result, int count, int id);
const wchar_t*				szext(const wchar_t* path);
void*						xfind(const void *key, const void *base, int count, int size, int(*compar)(const void* p1, const void* p2, void* param), void* param = 0);
const wchar_t*				szfname(const wchar_t* text); // get file name from string (no fail)
wchar_t*					sznum(wchar_t* p1, int num, int precision = 0, const wchar_t* empthy = 0, int radix = 10);
const wchar_t*				sznumber(int i, int precision = 0);
wchar_t*					szprint(wchar_t* dst, const wchar_t* src, ...);
inline const wchar_t*		szskipsp(const wchar_t* p) { while(*p==' ' || *p=='\t') p++; return p; }
const wchar_t*				szt(const wchar_t* en, const wchar_t* ru);
int							sz2num(const wchar_t* p1, const wchar_t** pp1 = 0);
extern unsigned				timeticks(); // get random seed
void						xsrand();
void						xsrand(unsigned seed);
int							xrand();
bool						xrun(const wchar_t* url, wchar_t* command_line, const wchar_t* directory, bool handle_output=false, bool hide_window=false);
void						xsort(void* base, int count, int size, int(*compare)(const void* p1, const void* p2, void* param), void* param = 0);
const wchar_t*				upper(const wchar_t* string);
inline void					szcat(wchar_t* p1, const wchar_t* p2) { szcpy((wchar_t*)szend(p1), p2); }
inline void					szcat(char* p1, const char* p2) { szcpy((char*)szend(p1), p2); }
inline wchar_t*				szchr(wchar_t* text, char ch) { return (wchar_t*)szchr((const wchar_t*)text, ch); }
inline int					szlen(const wchar_t* text) { return szend((wchar_t*)text) - text; }
inline int					szlen(const char* text) { return szend((char*)text) - text; }
extern int					sza2w(wchar_t* w, int wc, const char* a, int ac); // convert ansi string to wide char string
extern int					szu2w(wchar_t* w, int wc, const char* u, int uc); // convert utf-8 string to wide char string
extern int					szw2u(char* u, int uc, const wchar_t* w, int wc);
template<class T> inline T	imax(T a, T b) { return a > b ? a : b; }
template<class T> inline T	imin(T a, T b) { return a < b ? a : b; }
template<class T> inline T	iabs(T a) { return a > 0 ? a : -a; }
template<class T> inline void iswap(T& a, T& b) { T i = a; a = b; b = i; }
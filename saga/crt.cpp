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

extern wchar_t	chupper(wchar_t c); // to upper reg
int				locale; // curret locale

int rmcmp(const void* p1, const void* p2, int count)
{
	const unsigned char* e1 = (const unsigned char*)p1;
	const unsigned char* e2 = (const unsigned char*)p2;
	while(count--)
	{
		if(*e1 != *e2)
			return *e1 - *e2;
		e1++;
		e2++;
	}
	return 0;
}

wchar_t* szend(wchar_t* text)
{
	while(*text)
		text++;
	return text;
}

void szcpy(wchar_t* p1, const wchar_t* p2)
{
	while(*p2)
		*p1++ = *p2++;
	*p1 = 0;
}

inline void	szcat(wchar_t* p1, const wchar_t* p2) { szcpy((wchar_t*)szend(p1), p2); }

void szcpy(wchar_t* p1, const wchar_t* p2, int max_count)
{
	while(*p2 && max_count-->0)
		*p1++ = *p2++;
	*p1 = 0;
}

int szcmp(const wchar_t* p1, const wchar_t* p2)
{
	for(; *p2 && *p1; p1++, p2++)
	{
		if(*p1 == *p2)
			continue;
		return *p1 - *p2;
	}
	return *p1 - *p2;
}

int szcmp(const wchar_t* p1, const wchar_t* p2, int max_count)
{
	for(; *p2 && *p1 && max_count>0; p1++, p2++, max_count--)
	{
		if(*p1 == *p2)
			continue;
		return *p1 - *p2;
	}
	if(!max_count)
		return 0;
	return *p1 - *p2;
}

int szcmpi(const wchar_t* p1, const wchar_t* p2)
{
	for(; *p2 && *p1; p1++, p2++)
	{
		if(*p1==*p2)
			continue;
		if(chupper(*p1) == chupper(*p2))
			continue;
		return chupper(*p1) - chupper(*p2);
	}
	return chupper(*p1) - chupper(*p2);
}

int szcmpi(const wchar_t* p1, const wchar_t* p2, int max_count)
{
	for(; *p2 && *p1 && max_count>0; p1++, p2++, max_count--)
	{
		if(*p1 == *p2)
			continue;
		if(chupper(*p1) == chupper(*p2))
			continue;
		return chupper(*p1) - chupper(*p2);
	}
	if(!max_count)
		return 0;
	return chupper(*p1) - chupper(*p2);
}

wchar_t* sznum(wchar_t* p1, int num, int precision, const wchar_t* empthy, int radix)
{
	if(num == 0)
	{
		if(empthy)
			szcpy(p1, empthy);
		else
		{
			szcpy(p1, L"0");
			while(--precision>0)
				szcat(p1, L"0");
		}
	}
	else
	{
		char temp[32];
		int p = 0;
		if(num < 0)
		{
			*p1++ = '-';
			num = -num;
		}
		switch(radix)
		{
		case 16:
			while(num)
			{
				int a = (num%radix);
				if(a>9)
					temp[p++] = 'A' - 10 + a;
				else
					temp[p++] = '0' + a;
				num /= radix;
			}
			break;
		default:
			while(num)
			{
				temp[p++] = '0' + (num%radix);
				num /= radix;
			}
			break;
		}
		while(precision-- > p)
			*p1++ = '0';
		while(p)
			*p1++ = temp[--p];
		p1[0] = 0;
	}
	return p1;
}

const wchar_t* sznumber(int i, int precision)
{
	static wchar_t temp[32];
	sznum(temp, i, precision, 0, 10);
	return temp;
}

int sz2num(const wchar_t* p1, const wchar_t** pp1)
{
	int result = 0;
	bool sign = false;
	const int radix = 10;
	while(*p1 && *p1!='-' && (*p1 < '0' || *p1 > '9'))
		p1++;
	while(*p1)
	{
		wchar_t a = *p1;
		if(a < '0' || a > '9')
			break;
		if(a == '-')
		{
			sign = true;
			p1++;
			continue;
		}
		result = result*radix;
		result += a - '0';
		p1++;
	}
	if(sign)
		result = -result;
	if(pp1)
		*pp1 = p1;
	return result;
}

const wchar_t* szchr(const wchar_t* p1, wchar_t ch)
{
	for(; *p1; p1++)
	{
		if(*p1==ch)
			return p1;
	}
	return 0;
}

const wchar_t* szext(const wchar_t* path)
{
	for(const wchar_t* r = szend((wchar_t*)path); r > path; r--)
	{
		if(*r == '.')
			return r + 1;
		else if(*r == '\\')
			return 0;
	}
	return 0;
}

const wchar_t* szfname(const wchar_t* path)
{
	for(const wchar_t* r = szend((wchar_t*)path); r > path; r--)
	{
		if(*r == '\\')
			return r + 1;
	}
	return path;
}

wchar_t* szprint(wchar_t* result, const wchar_t* src, ...)
{
	wchar_t** a = (wchar_t**)((char*)&src + sizeof(src));
	wchar_t* dst = result;
	while(true)
	{
		switch(*src)
		{
		case 0:
			*dst = 0;
			return result;
		case '%':
			src++;
			if(*src == '%')
			{
				*dst++ = *src++;
				continue;
			}
			if(*src >= '0' && *src <= '9')
			{
				int pn = 0;
				while(*src >= '0' && *src <= '9')
					pn = pn * 10 + (*src++) - '0';
				wchar_t* pt = 0;
				if(*src == 'i')
				{
					src++;
					static wchar_t temp[16];
					sznum(temp, *((int*)((char*)a + sizeof(wchar_t*)*(pn - 1))), 0, 0, 10);
					pt = temp;
				}
				else
					pt = a[pn-1];
				if(pt)
				{
					while(*pt)
						*dst++ = *pt++;
				}
			}
			break;
		default:
			*dst++ = *src++;
			break;
		}
	}
	return result;
}

const wchar_t* szt(const wchar_t* en, const wchar_t* ru)
{
	if(locale==1)
		return ru;
	return en;
}

void rmcpy(void* p1, const void* p2, int size)
{
	for(int i = 0; i < size; i++)
		((char*)p1)[i] = ((char*)p2)[i];
}

void rmmov(void *dst, const void *src, int count)
{
	if(dst <= src || (char*)dst >= ((char*)src + count))
	{
		while(count--)
		{
			*(char*)dst = *(char*)src;
			dst = (char*)dst + 1;
			src = (char*)src + 1;
		}
	}
	else
	{
		dst = (char*)dst + count - 1;
		src = (char*)src + count - 1;
		while(count--)
		{
			*(char*)dst = *(char*)src;
			dst = (char*)dst - 1;
			src = (char*)src - 1;
		}
	}
}

void rmset(void* p1, int value, int size)
{
	for(int i = 0; i < size; i++)
		((char*)p1)[i] = ((char)value);
}

const wchar_t* upper(const wchar_t* string)
{
	static wchar_t temp[256];
	szcpy(temp, string, sizeof(temp));
	temp[0] = chupper(string[0]);
	return temp;
}

char* szend(char* text)
{
	while(*text)
		text++;
	return text;
}

void szcpy(char* p1, const char* p2)
{
	while(*p2)
		*p1++ = *p2++;
	*p1 = 0;
}

void szcpy(char* p1, const char* p2, int max_count)
{
	while(*p2 && max_count-->0)
		*p1++ = *p2++;
	*p1 = 0;
}

int szcmp(const char* p1, const char* p2)
{
	for(; *p2 && *p1; p1++, p2++)
	{
		if(*p1 == *p2)
			continue;
		return *p1 - *p2;
	}
	return *p1 - *p2;
}

int szcmp(const char* p1, const char* p2, int max_count)
{
	for(; *p2 && *p1 && max_count>0; p1++, p2++, max_count--)
	{
		if(*p1 == *p2)
			continue;
		return *p1 - *p2;
	}
	if(!max_count)
		return 0;
	return *p1 - *p2;
}

int szcmpi(const char* p1, const char* p2)
{
	for(; *p2 && *p1; p1++, p2++)
	{
		if(*p1==*p2)
			continue;
		if(chupper(*p1) == chupper(*p2))
			continue;
		return chupper(*p1) - chupper(*p2);
	}
	return chupper(*p1) - chupper(*p2);
}
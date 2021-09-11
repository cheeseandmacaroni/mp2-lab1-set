// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Битовое поле

#include <limits>
#include <iostream>
#include "tbitfield.h"

TBitField::TBitField(size_t len):bitLen(len)
{
	memLen = getIndex(bitLen) + 1;
	pMem = new uint[memLen];
	for (size_t i =0; i < memLen; ++i)
	{
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	*this = bf;
}

size_t TBitField::getIndex(const size_t n) const  // индекс в pМем для бита n
{
    return n / (8 *sizeof(uint));
}

uint TBitField::getMask(const size_t n) const // битовая маска для бита n
{
    return pow(2, n);
}

// доступ к битам битового поля
uint TBitField::getLength() const // получить длину (к-во битов)
{
    return bitLen;
}

void TBitField::setBit(const size_t n) // установить бит
{
	if (n > bitLen - 1)
	{
		throw std::exception("Bad index");
	}
	char bit = n % (8 * sizeof(uint));
	pMem[getIndex(n)] |= getMask(bit);
}

void TBitField::clrBit(const size_t n) // очистить бит
{
	if (n > bitLen - 1)
	{
		throw std::exception("Bad index");
	}
	char bit = n % (8 * sizeof(uint));
	pMem[getIndex(n)] &= ~getMask(bit);
}

bool TBitField::getBit(const size_t n) const // получить значение бита
{
	if (n > bitLen - 1)
	{
		throw std::exception("Bad index");
	}
	char bit = n % (8 * sizeof(uint));
    return ((pMem[getIndex(n)] & getMask(bit)) >> bit);
}

// битовые операции
TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (*this != bf)
	{
		if(pMem != nullptr)
			delete[] pMem;
		bitLen = bf.getLength();
		memLen = getIndex(bitLen) + 1;
		pMem = new uint[memLen];
		for (size_t i = 0; i < memLen; ++i)
		{
			pMem[i] = 0;
		}
		for (size_t i = 0; i < bitLen; ++i)
		{
			if (bf.getBit(i))
				setBit(i);
		}
	}
    return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (bitLen != bf.getLength())
	{
		return false;
	}
	for (size_t i = 0; i < bitLen; ++i)
	{
		if (bf.getBit(i) != getBit(i))
			return false;
	}
    return true;
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (bitLen != bf.getLength())
	{
		return true;
	}
	for (size_t i = 0; i < bitLen; ++i)
	{
		if (bf.getBit(i) != getBit(i))
			return true;
	}
    return false;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField min = bitLen <= bf.getLength() ? *this : bf;
	TBitField max = bitLen > bf.getLength() ? *this : bf;
	for (size_t i = 0; i < min.getLength(); ++i)
	{
		if(min.getBit(i))
			max.setBit(i);
	}
    return max;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	size_t min = bitLen <= bf.getLength() ? bitLen : bf.getLength();
	TBitField res(min == bitLen ? bf.getLength() : bitLen);
	for (size_t i = 0; i < min; ++i)
	{
		if (getBit(i) & bf.getBit(i))
			res.setBit(i);
	}
    return res;
}

TBitField TBitField::operator~() // отрицание
{
	TBitField res(*this);
	for (size_t i = 0; i < bitLen; ++i)
	{
		if (res.getBit(i))
			res.clrBit(i);
		else if (!res.getBit(i))
			res.setBit(i);
	}
    return res;
}

TBitField::~TBitField()
{
	delete[] pMem;
}

// ввод/вывод
std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
	size_t i = 0;
	bool bit;
	while (!istr.eof())
	{
		istr >> bit;
		if (!istr)
			throw std::exception("Bad input");
		if (bit)
			bf.setBit(i);
		++i;
	}
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
	try
	{
		for (size_t i = 0; i < bf.getLength(); ++i)
		{
			ostr << bf.getBit(i) << ' ';
		}
	}
	catch (std::exception &error)
	{
		throw error;
	}
    return ostr;
}

#include "tbitfield.h"
#include <iostream>
#include <sstream>

TBitField::TBitField(size_t len) :bitLen(len)
{
	memLen = getIndex(bitLen);
	if ((bitLen % (sizeof(elType) * 8)) != 0)
		memLen++;
	pMem = new elType[memLen];
	for (size_t i = 0; i < memLen; ++i)
	{
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf):pMem(nullptr), bitLen(0) // конструктор копирования
{
	*this = bf;
}

size_t TBitField::getIndex(const size_t n) const  // индекс в pМем для бита n
{
	return n / (8 * sizeof(elType));
}

elType TBitField::getMask(const size_t n) const // битовая маска для бита n
{
	elType res = static_cast<elType>(1) << n;
	return res;
}

// доступ к битам битового поля
size_t TBitField::getLength() const // получить длину (к-во битов)
{
	return bitLen;
}

size_t TBitField::getNumBytes() const // получить количество байт выделенной памяти
{
	return memLen * sizeof(elType);
}

void TBitField::setBit(const size_t n) // установить бит
{
	if (n > bitLen - 1)
	{
		throw "Bad bit";
	}
	size_t bit = n % (8 * sizeof(elType));
	pMem[getIndex(n)] |= getMask(bit);
}

void TBitField::clrBit(const size_t n) // очистить бит
{
	if (n > bitLen - 1)
	{
		throw "Bad bit";
	}
	char bit = n % (8 * sizeof(elType));
	pMem[getIndex(n)] &= ~getMask(bit);
}

bool TBitField::getBit(const size_t n) const // получить значение бита
{
	if (n > bitLen - 1)
	{
		throw "Bad bit";
	}
	char bit = n % (8 * sizeof(elType));
	return ((pMem[getIndex(n)] & getMask(bit)) >> bit);
}

// битовые операции
TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (*this != bf)
	{
		if (pMem != nullptr)
			delete[] pMem;
		bitLen = bf.getLength();
		memLen = getIndex(bitLen);
		if ((bitLen % (sizeof(elType) * 8)) != 0)
			memLen++;
		pMem = new elType[memLen];
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

TBitField TBitField::operator|(const TBitField &bf) // операция "или"//
{
	TBitField max(0), min(0);
	if (bitLen >= bf.getLength())
	{
		min = bf;
		max = *this;
	}
	else if (bitLen < bf.getLength())
	{
		max = bf;
		min = *this;
	}
	for (size_t i = 0; i < min.getLength(); ++i)
	{
		if (min.getBit(i))
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
	for (size_t i = 0; i < memLen - 1; ++i)
	{
		this->pMem[i] = ~(this->pMem[i]);
	}
	TBitField res(*this);
	for (size_t i = sizeof(elType) * 8 * (memLen - 1); i < bitLen; ++i)
	{
		if (!getBit(i))
		{
			res.setBit(i);
		}
		else if (getBit(i))
		{
			res.clrBit(i);
		}
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
	size_t size;
	std::string in_string;
	istr >> in_string;
	if (istr.fail())
		throw "Bad input";
	size = in_string.length();
	bf = TBitField(size);
	for (size_t i = 0; i < size; ++i)
	{
		if (in_string[i] == '1')
			bf.setBit(size - i - 1);
		else if (in_string[i] != '1' && in_string[i] != '0')
		{
			throw "Bad input";
		}
	}
	return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
	try
	{
		for (size_t i = bf.getLength(); i > 0; --i)
		{
			ostr << bf.getBit(i - 1);
		}
	}
	catch (std::exception &error)
	{
		throw error;
	}
	return ostr;
}

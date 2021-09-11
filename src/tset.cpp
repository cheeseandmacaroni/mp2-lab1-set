// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Множество - реализация через битовые поля

#include <iostream>
#include "tbitfield.h"
#include "tset.h"

TSet::TSet(size_t mp) : maxPower(mp), bitField(mp) {}

// конструктор копирования
TSet::TSet(const TSet &s) : maxPower(s.getMaxPower()), bitField(s.bitField) {}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) :maxPower(bf.getLength()), bitField(bf) {}

TSet::operator TBitField()
{
    return bitField;
}

size_t TSet::getMaxPower() const // получить макс. к-во эл-тов
{
    return maxPower;
}

bool TSet::isMember(const uint elem) const // элемент множества?
{
	if(elem < maxPower && bitField.getBit(elem))
		return true;
	else
		return false;
}

void TSet::insElem(const uint elem) // включение элемента множества
{
	if (elem > maxPower - 1 || elem < 0)
		throw std::exception();
	else
	{
		bitField.setBit(elem);
	}
}

void TSet::delElem(const uint elem) // исключение элемента множества
{
	if (elem > maxPower - 1 || elem < 0)
		throw std::exception();
	else
	{
		bitField.clrBit(elem);
	}
}

// теоретико-множественные операции
TSet& TSet::operator=(const TSet &s) // присваивание
{
	if (*this != s)
	{
		bitField = s.bitField;
		maxPower = s.getMaxPower();
	}
    return *this;
}

bool TSet::operator==(const TSet &s) const // сравнение
{
	if (maxPower == s.getMaxPower() && bitField == s.bitField)
		return true;
	else
		return false;
}

bool TSet::operator!=(const TSet &s) const // сравнение
{
	if (maxPower != s.getMaxPower() || bitField != s.bitField)
		return true;
	else
		return false;
}

TSet TSet::operator+(const TSet &s) // объединение
{
	TBitField tmp = bitField | s.bitField;
	TSet res(tmp);
    return res;
}

TSet TSet::operator+(const uint &elem) // объединение с элементом
{
	if (elem >= maxPower)
	{
		TSet res(TBitField(elem) | bitField);
		res.bitField.setBit(elem);
		return res;
	}
	else if (elem < maxPower && elem >= 0)
	{
		TSet res(bitField);
		res.bitField.setBit(elem);
		return res;
	}
	else if (elem < 0)
	{
		throw std::exception();
	}
}

TSet TSet::operator-(const uint &elem) // разность с элементом
{
	if (elem < maxPower && elem >= 0)
	{
		TSet res(bitField);
		res.bitField.clrBit(elem);
		return res;
	}
	else if (elem >= maxPower)
	{
		return TSet(bitField);
	}
	else if (elem < 0)
	{
		throw std::exception();
	}
}

TSet TSet::operator*(const TSet &s) // пересечение
{
	TSet res(bitField & s.bitField);
    return res;
}

TSet TSet::operator~() // дополнение
{
	TSet res(~bitField);
    return res;
}

// перегрузка ввода/вывода
std::istream &operator>>(std::istream &istr, TSet &s) // ввод
{
	istr >> s.bitField;
    return istr;
}

std::ostream& operator<<(std::ostream &ostr, const TSet &s) // вывод
{
	ostr << s.bitField;
    return ostr;
}

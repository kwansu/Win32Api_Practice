#ifndef __LIST_H__
#define __LIST_H__

template<typename T>
class List
{
	T* member;
	INT len;
public:
	List()
	{
		member = NULL;
		len = 0;
	}
	List(const List<T> &list)
	{
		len = list.len;
		member = new T[len];
		for (int n = 0; n < len; n++)
			member[n] = list.member[n];
	}
	VOID operator=(T memberTem)
	{
		if (member == NULL)
		{
			len = 1;
			member = new T[len];
			member[0] = memberTem;
			return;
		}
		T* tem = new T[len];
		for (INT n = 0; n < len; n++)
			tem[n] = member[n];

		delete[] member;
		len++;
		member = new T[len];
		for (INT n = 0; n < len - 1; n++)
			member[n] = tem[n];

		delete[] tem;
		member[len - 1] = memberTem;
	}
	T& operator[](INT index)
	{
		if (index < 0 || index >= len)
		{
			return member[0];
		}
		return member[index];
	}
	VOID Vacate()
	{
		len = 0;
		if (member != NULL)
			delete[] member;
	}
	INT GetLen() const
	{
		return len;
	}
	~List()
	{
		Vacate();
	}
};

#endif
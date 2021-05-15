#include <iostream>
using namespace std;

class First
{
public:
	virtual void Function()
	{
		printf("First\n");
	}
	void Function2()
	{
		this->Function();
	}
};

class Second : public First
{
	int a;
public:
	Second(int num)
	{
		a = num;
	}
	void Function()
	{
		printf("Second\n");
	}
};

void main()
{
	Second C(3);
	First* pC = (First*)&C;
	pC->Function2();
}
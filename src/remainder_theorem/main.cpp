#include <iostream>
#include "number/Integer.h"
#include "number/algrm.h"

//#pragma comment(lib, ".\\Number.lib")

using namespace std;
using namespace Number;

int main()
{
	typedef Integer inte;
	unsigned int number;
	cout << "Please input the total number:" << endl;
	cin >> number;
	vector<inte> num(number, inte(0)), mod(number, inte(0));
	vector<inte> n2(number, inte(1));
	inte temp;
	for (unsigned int ii = 0;ii < number;ii++)
	{
		cout << "Please input " << ii << "th number." << endl;
		cin >> num[ii] >> mod[ii];
	}

	for (unsigned int ii = 0;ii < number;ii++)
	{
		for (unsigned int jj = 0;jj < number;jj++)
		{
			if (jj != ii)
				n2[ii] = n2[ii] * num[jj];
		}
		inte tem1, tem2;
		ExGCD(n2[ii], num[ii], tem1, tem2);
		n2[ii] = n2[ii] * tem1;
	}
	inte temp2(1);
	for (unsigned int ii = 0;ii < number;ii++)
	{
		temp = temp + n2[ii] * mod[ii];
		temp2 = temp2 * num[ii];
	}
	while (temp < 0)
		temp = temp + temp2;
	while (temp > temp2)
		temp = temp % temp2;

	cout << "The result is:" << temp << " +- n*" << temp2 << endl;

	system("pause");
	return 0;
}

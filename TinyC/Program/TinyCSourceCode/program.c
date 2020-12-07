struct MyStruct0
{
	int a;
	float b;
	char c;
};

struct MyStruct1
{
	int d;
	int e;
	float f;
	struct MyStruct2
	{
		MyStruct0 g;
		int h;
	};

	MyStruct2 i;

	MyStruct0 j;
};

int main()
{
	//sfasfl;
	float a = 10 / 5;
	float b = 10 + 5;
	a++;
	a = a * a;
	bool b = false;
	bool c = true;
	c = b && c;
	c = b || c;
	a = !a;
	a != a;
	a--;
	--a;
	a >= a;
	a <= a;
	a%a;
	a | a;
	a&a;
	a^a;

	int numI = 996;
	float numF = 99.6;
	printf("Hello World!");
}
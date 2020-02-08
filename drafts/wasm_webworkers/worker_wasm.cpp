#include <stdlib.h>
#include <vector>



extern "C" {

	extern void cb(float v);
	extern void logString(const char* data, unsigned int len);
	
	const int bigBufferLen = 1024 * 1024 * 24;
	char bigBuffer[bigBufferLen];
	
	char b[7];
	
	int vectorSum()
	{
		const int len = 1024 * 1024 * (128 + 64);
		std::vector<int> v;
		for (int i = 0; i < len; ++i)
		{
			v.push_back(rand());
		}
		int sum = 0;
		for (int i = 0; i < len; ++i)
		{
			sum += v[i];
		}
		return sum;
	}
	
	int run(int x, int y)
	{
		int sum = vectorSum();
		cb(sum);
		// char* mem = (char*)malloc(1024 * 1024 * 256); 
		// const char* str = "Hello!";
		// mem[0] = 42;
		// for (int i = 0; i < 7; ++i)
			// mem[i] = str[i];
		// //cb(0.4);
		// for (int i = 0; i < bigBufferLen; ++i)
			// bigBuffer[i] = 8;
		// logString(mem, 7);
		// free(mem);
		return 0 ; //x + y; 
	}
}

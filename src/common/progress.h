#ifndef PROGRESS_H
#define PROGRESS_H
#include <sstream>
#include <iostream>

inline void progress(int percent)
{
	assert(percent >= 0);

	static std::stringstream bars;
	static int x = 0;
	static int last_percent =0;
	static int last_ten_percent =0;
	if(percent > 100)
		percent = 100;
	std::string slash[4];
	slash[0] = "\\";
	slash[1] = "-";
	slash[2] = "/";
	slash[3] = "|";
	if(percent < last_percent)
	{
		last_percent = 0;
		last_ten_percent = 0;
		bars.clear();
		bars.str("");
	}

	int k = percent - last_percent;
	int j = (percent - last_ten_percent)/10;

	
	for(int i=0; i < j; i++)
		bars << ".";
	if(j >  0)
		last_ten_percent+=j*10;
		
	if(k > 0)
	{
		last_percent=percent;
		std::cout << "\r"; 
		if(percent < 100)
			std::cout << bars.str() << " " <<  slash[x] << " " << percent << " %"; 
		else
			std::cout << bars.str()  << " " << percent << " %"<< std::endl;
		x++; 
		if(x == 4)
		x = 0; 
	}
}


inline void progress(float i,float n)
{
	progress((int)(i*100/n));
}


#endif

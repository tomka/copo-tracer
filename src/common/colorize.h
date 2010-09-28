#ifndef COLORIZE_H
#define COLORIZE_H

#include <vec.h>
#include <mat.h>



///creates different type of color map matrices
///colors are stored as columns in the resulting matrix
///the number of columns is defined by the parameter steps
///colormaps can be chosen by name :
/// possible colormaps are 
/// "gray" black .. white
/// "bone" grayscale colormap with a higher blue component (looks like xray images)
/// "jet" blue .. cyan .. yellow .. orange .. red
/// "hot" black .. red .. yellow .. white
/// "thermo" black .. magenta .. blue.. cyan..green .. yellow ..red.. white
/// "hsv" red .. yellow .. green .. cyan .. blue .. magenta .. red
	

inline mat<unsigned char> create_color_map(unsigned steps=256,std::string name="jet")
{
	
	
	mat<float> map;
	map.resize(3,steps);
	
	if(name == "gray")
	{
		vec<float> ls = lin_space<float>(0,1,steps);
		
		//a linear grayscale colormap.
		for(unsigned i = 0; i < steps;i++)
		{
			map(0,i)=ls(i);
			map(1,i)=ls(i);
			map(2,i)=ls(i);		
		}
		return float_2_uchar(map);
	}
	if(name == "bone")
	{
		vec<float> x;
		x=lin_space<float>(0,1,steps);
		//bone
		//is a grayscale colormap with a higher value for the blue component. 
		//This colormap is useful for adding an "electronic" look to grayscale images.
		for(unsigned i = 0; i < steps;i++)
		{
			if(x(i) < 3.0f/4.0f)
				map(0,i)=(7.0f/8.0f * x(i));
			else
				map(0,i)=(11.0f/8.0f * x(i) - 3.0f/8.0f);
			
			if(x(i) < 3.0f/8.0f)
				map(1,i)=(7.0f/8.0f * x(i));
			else if(x(i) < 3.0f/4.0f)
				map(1,i)=(29.0f/24.0f * x(i) - 1.0f/8.0f);
			else
				map(1,i)=(7.0f/8.0f * x(i) + 1.0f/8.0f);

			if(x(i) < 3.0f/8.0f)
				map(2,i)= (29.0f/24.0f * x(i));
			else
				map(2,i)=(7.0f/8.0f * x(i) + 1.0f/8.0f);
	

		}
		return float_2_uchar(map);
	}
	if(name == "jet")
	{
		vec<float> x;
		x=lin_space<float>(0,1,steps);
		
		for(unsigned i = 0; i < steps;i++)
		{
			map(0,i)=map(1,i)=map(2,i)=0.0f;
			
			if((x(i) >= 3.0f/8.0f) && (x(i) < 5.0f/8.0f)) 
				map(0,i) = (4.0f * x(i) - 3.0f/2.0f);
		
			if((x(i) >= 5.0f/8.0f) && (x(i) < 7.0f/8.0f))
			{
				map(0,i)=1.0f;
			}
			if(x(i) >= 7.0f/8.0f)
				map(0,i)+= -4.0f * x(i) + 9.0f/2.0f;
			
			
			if(x(i) >= 1.0f/8.0f && x(i) < 3.0f/8.0f)
				map(1,i)= (4.0f * x(i) - 1.0f/2.0f);

			if(x(i) >= 3.0f/8.0f && x(i) < 5.0f/8.0f)
			{ 
				map(1,i)=1;
			}
			if(x(i) >= 5.0f/8.0f && x(i) < 7.0f/8.0f)
					map(1,i)+= (-4.0f * x(i) + 7.0f/2.0f);
			

			if(x(i) < 1.0f/8.0f)
				map(2,i)= (4.0f * x(i) + 1.0f/2.0f);

			if(x(i) >= 1.0f/8.0f && x(i) < 3.0f/8.0f)
			{
				map(2,i)=1.0;
			}
			if(x(i) >= 3.0f/8.0f && x(i) < 5.0f/8.0f)
				map(2,i) += -4.0f * x(i) + 5.0f/2.0f;
			

	

		}
		return float_2_uchar(map);
	}
	if(name == "hot")
	{
		vec<float> x;
		x=lin_space<float>(0,1,steps);
		
		for(unsigned i = 0; i < steps;i++)
		{
			map(0,i)=map(1,i)=map(2,i)=0.0f;
			
			if(x(i) <= 1.0f/3.0f) 
				map(0,i) =  3.0f*x(i);
			else
				map(0,i) =  1.0f;
			
		
			if(x(i) > 1.0f/3.0f && x(i) < 2.0f/3.0f)
				map(1,i)= 3.0f*x(i) -1.0f;
			if(x(i) >=2.0/3.0f)
				map(1,i)=1.0f;
			
			
			if(x(i) >= 2.0f/3.0f)
				map(2,i)=3.0f*x(i)-2.0f;
			

		}
		return float_2_uchar(map);
	}
	if(name == "hsv")
	{
		vec<float> x;
		x=lin_space<float>(0,1,steps);
		
		for(unsigned i = 0; i < steps;i++)
		{
			map(0,i)=map(1,i)=map(2,i)=0.0f;
			
			if(x(i) <= 1.0f/5.0f) 
				map(0,i) =  1.0f;
			if(x(i) > 1.0f/5.0f && x(i) <=2.0f/5.0f)
				map(0,i) =  -5.0f*x(i) + 2.0f;
			if(x(i) > 4.0f/5.0f)
				map(0,i) = 5.0f*x(i)-4.0f;

			if(x(i) <= 1.0f/5.0f) 
				map(1,i) =  5.0f*x(i);
			if(x(i) > 1.0f/5.0f && x(i) <=3.0f/5.0f)
				map(1,i) =  1.0f;
			if(x(i) > 3.0f/5.0f && x(i) <= 4.0f/5.0f)
				map(1,i) = -5.0f*x(i)+4.0f;


			
			if(x(i) > 2.0f/5.0f && x(i) <=3.0f/5.0f)
				map(2,i) =  5.0f*x(i)-2.0f;
			if(x(i) > 3.0f/5.0f && x(i) <= 4.0f/5.0f)
				map(2,i) = 1.0f;
			if(x(i) >4.0f/5.0f) 
				map(2,i) =  -5.0f*x(i)+5.0f;
				
			
			
		}
		return float_2_uchar(map);
	}
	if(name == "thermo")
	{
		vec<float> x;
		x=lin_space<float>(0,1,steps);
		
		for(unsigned i = 0; i < steps;i++)
		{
			map(0,i)=map(1,i)=map(2,i)=0.0f;
		

			if(x(i) <= 1.0f/7.0f) 
				map(0,i) =  7.0f*x(i);
			if(x(i) > 1.0f/7.0f && x(i) <=2.0f/7.0f)
				map(0,i) =  -7.0f*x(i) + 2.0f;
			if(x(i) > 4.0f/7.0f && x(i) <=5.0f/7.0f)
				map(0,i) = 7.0f*x(i)-4.0f;
			if(x(i) >5.0f/7.0f)
				map(0,i) = 1.0f;

			if(x(i) >=2.0f/7.0f && x(i) <= 3.0f/7.0f) 
				map(1,i) =  7.0f*x(i)-2.0f;
			else if(x(i) > 3.0f/7.0f && x(i) <=5.0f/7.0f)
				map(1,i) =  1.0f;
			else if(x(i) > 5.0f/7.0f && x(i) <=6.0f/7.0f)
				map(1,i) = -7.0f*x(i)+6.0f;
			else if(x(i) > 6.0f/7.0f)
				map(1,i) = 7.0f*x(i)-6.0f;

			if(x(i) <= 1.0f/7.0f) 
				map(2,i) = 7.0f*x(i);
			if(x(i) > 1.0f/7.0f && x(i) <=3.0f/7.0f)
				map(2,i) =  1.0f;
			if(x(i) > 3.0f/7.0f && x(i) <=4.0f/7.0f)
				map(2,i) = -7.0f*x(i)+4.0f;
			if(x(i) > 6.0f/7.0f)
				map(2,i) = 7.0f*x(i)-6.0f;

		}
		return float_2_uchar(map);

	}
	assert(false); //colormap not found
	return float_2_uchar(map);
}

inline mat<tiny_vec<unsigned char,3> > colorize(const mat<unsigned char>& grayscale_img,std::string colormap_name="jet")
{
	int w = (int) grayscale_img.w();
	int h = (int)grayscale_img.h();
	mat<unsigned char> colormap = create_color_map(256,colormap_name);
	mat<tiny_vec<unsigned char,3>> color_img(w,h);
	for(int y = 0;y < h; y++)
		for(int x = 0; x < w; x++)
			color_img(x,y)=colormap.col((int)grayscale_img(x,y));
	return color_img;	
}

inline mat<tiny_vec<unsigned char,3> > colorize(const mat<unsigned int>& grayscale_img,std::string colormap_name="jet")
{
	int w = (int) grayscale_img.w();
	int h = (int)grayscale_img.h();
	int maxval = *std::max_element(grayscale_img.begin(),grayscale_img.end());
	mat<unsigned char> colormap = create_color_map(maxval,colormap_name);
	mat<tiny_vec<unsigned char,3>> color_img(w,h);
	for(int y = 0;y < h; y++)
		for(int x = 0; x < w; x++)
			color_img(x,y)=colormap.col((int)grayscale_img(x,y));
	return color_img;	
}


#endif
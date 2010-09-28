#include "mat.h"
#include "math_utils.h"
#include <vector>
#include "sampler_2d.h"



template <typename T>
class rasterizer_2d
{
public:
	rasterizer_2d(mat<T>* img=NULL);

	void bind_image(mat<T>* img);

	int w() const;
	
	int h() const;

	bool is_inside_clip_region(int x, int y) const;

	void set_clipping_region(int x0,int y0, int x1, int y1);
	
	void clear(const T& color);

	void set_pixel(int x, int y, const T& color);

	void raster_hline(int x1, int x2, int y,const T& color);
	
	void raster_vline(int x, int y1, int y2,const T& color);
	
	void raster_box(int x1, int y1, int x2, int y2,const  T& color);
	
	void raster_solid_box(int x1, int y1, int x2, int y2,const T& color);
	
	void raster_line(int x0, int y0,int x1,int y1,const T& color);
	
	void raster_circle(int x0,int y0,int radius, const T& color);
	
	void raster_solid_circle(int x0, int y0, int radius, const T& color);
	
	void raster_ellipse( double xc, double yc,double r1, double r2, double theta,const T& col, int n=32);
	
	void raster_triangle(int x1, int y1, int x2, int y2, int x3, int y3, const T& color);
	
	void raster_solid_triangle(int x1, int y1, int x2, int y2, int x3, int y3, const T& color);
	
	void raster_polygon(const std::vector<tiny_vec<int,2>>& pts, const T& color);
	template <typename S>
	void raster_level_set(mat<S>& func,const S level,const T& color)
	{
		sampler_2d<S> f(&func);
		f.set_interpolation(sampler_2d<S>::BILINEAR);
		f.set_wrapping(sampler_2d<S>::MIRROR_REPEAT);
	
		float sx = func.w()/(float)w();
		float sy = func.h()/(float)h();
	
		for(int j = 0; j < h(); j++)
		{
			for(int i = 0; i < w();i++)
			{

				if(f(sx*i,sy*j) <= level)
				{
					if(f(sx*(i+1),sy*j) > level || f(sx*(i-1),sy*j) > level || f(sx*i,sy*(j-1)) > level || f(sx*i,sy*(j+1)) > level )
						set_pixel_unchecked(i,j,color);
				
				} else
				{
					if(f(sx*(i+1),sy*j) <= level || f(sx*(i-1),sy*j) <= level || f(sx*i,sy*(j-1)) <= level || f(sx*i,sy*(j+1)) <= level )
						set_pixel_unchecked(i,j,color);
				}
			}
		}


	}

	template <typename S>
	void raster_lower_level_set(mat<S>& func,const S level,const T& color)
	{
		sampler_2d<S> f(&func);
		f.set_interpolation(sampler_2d<S>::BILINEAR);
		f.set_wrapping(sampler_2d<S>::MIRROR_REPEAT);
	
		float sx = func.w()/(float)w();
		float sy = func.h()/(float)h();
	
		for(int j = 0; j < h(); j++)
		{
			for(int i = 0; i < w();i++)
			{

				if(f(sx*i,sy*j) <= level)
				{
					set_pixel_unchecked(i,j,color);
				}
			}
		}
	}

	template <typename S>
	void raster_upper_level_set(mat<S>& func,const S level,const T& color)
	{
		sampler_2d<S> f(&func);
		f.set_interpolation(sampler_2d<S>::BILINEAR);
		f.set_wrapping(sampler_2d<S>::MIRROR_REPEAT);
	
		float sx = func.w()/(float)w();
		float sy = func.h()/(float)h();
	
		for(int j = 0; j < h(); j++)
		{
			for(int i = 0; i < w();i++)
			{

				if(f(sx*i,sy*j) > level)
				{
					set_pixel_unchecked(i,j,color);
				}
			}
		}
	}
	
	/*

	void fill(int sx, int sy, const T& color, const T& border_color)
	{
	}

	bool is_inside_polygon()
	{
	}

	bool is_inside_convex_polygon()
	{
	}

	void raster_convex_solid_polygon(const std::vector<tiny_vec<int,2>>& pts, const T& color)
	{
	}

	void raster_solid_polygon(const std::vector<tiny_vec<int,2>>& pts, const T& color)
	{
		

	}


	
	*/
protected:
	//pointer to image
	mat<T> *img_ptr;
	//clip window
	int xmin,xmax,ymin,ymax;

	void set_pixel_unchecked(int x, int y,const T& color);

	T get_pixel_unchecked(int x, int y) const;

	int out_code(double x, double y);

	void bresenham(int x1, int y1, int x2, int y2,const T& color);

};

template <typename T>
rasterizer_2d<T>::rasterizer_2d(mat<T>* img)
{
	bind_image(img);
}

template <typename T>
void rasterizer_2d<T>::bind_image(mat<T>* img)
{
	img_ptr = img;
	if(img!= NULL)
	{
		xmin= ymin=0;
		xmax= img->w()-1;
		ymax = img->h()-1;
	}
}

template <typename T>
int rasterizer_2d<T>::w() const
{
	return img_ptr->w();
}

template <typename T>
int rasterizer_2d<T>::h() const
{
	return img_ptr->h();
}

template <typename T>
void rasterizer_2d<T>::set_clipping_region(int x0,int y0, int x1, int y1)
{
	xmin = clamp(x0,0,w()-1);	
	ymin = clamp(y0,0,h()-1);
	xmax = clamp(x1,0,w()-1);
	ymax = clamp(y1,0,h()-1);
}	
	
	
template <typename T>
void rasterizer_2d<T>::clear(const T& color)
{
	for(int y = ymin; y <=ymax; y++)
		for(int x = xmin; x <= xmax; x++)
			set_pixel_unchecked(x,y,color);
}

template <typename T>
bool rasterizer_2d<T>::is_inside_clip_region(int x, int y) const
{
	return (x >= xmin && x <= xmax && y >= ymin && y <= ymax);
}

template <typename T>
T rasterizer_2d<T>::get_pixel_unchecked(int x, int y) const
{
	return (*img_ptr)(x,y);
}

template <typename T>
void rasterizer_2d<T>::set_pixel(int x, int y, const T& color)
{
	if(is_inside_clip_region(x,y))
		(*img_ptr)(x,y)=color;
}

template <typename T>
void rasterizer_2d<T>::raster_hline(int x1, int x2, int y,const T& color)
{
		
	x1 = clamp(x1,xmin,xmax);
	x2 = clamp(x2,xmin,xmax);
	y = clamp(y,ymin,ymax);
	if(x1 > x2)
		std::swap(x1,x2);

	for(int x = x1; x <= x2; x++)
		set_pixel_unchecked(x,y,color);
}

template <typename T>
void rasterizer_2d<T>::raster_vline(int x, int y1, int y2,const T& color)
{
		
	x = clamp(x,xmin,xmax);
	y1 = clamp(y1,ymin,ymax);
	y2 = clamp(y2,ymin,ymax);
	if(y1 > y2)
		std::swap(y1,y2);
	for(int y = y1; y <= y2; y++)
		set_pixel_unchecked(x,y,color);
}

template <typename T>
void rasterizer_2d<T>::raster_box(int x1, int y1, int x2, int y2,const  T& color)
{
	raster_hline(x1,x2,y1,color);
	raster_hline(x1,x2,y2,color);
	raster_vline(x1,y1,y2,color);
	raster_vline(x2,y1,y2,color);
}

template <typename T>
void rasterizer_2d<T>::raster_solid_box(int x1, int y1, int x2, int y2,const T& color)
{
	x1 = clamp(x1,xmin,xmax);
	x2 = clamp(x2,xmin,xmax);
	y1 = clamp(y1,ymin,ymax);
	y2 = clamp(y2,ymin,ymax);
	if(x1 > x2)
		std::swap(x1,x2);
	if(y1 > y2)
		std::swap(y1,y2);

	for(int y = y1; y <=y2; y++)
		for(int x = x1; x <=x2; x++)
			set_pixel_unchecked(x,y,color);		
}

template <typename T>
void rasterizer_2d<T>::raster_line(int x0, int y0,int x1,int y1,const T& color)
{
	const int RIGHT = 8;  //1000
	const int TOP = 4;    //0100
	const int LEFT = 2;   //0010
	const int BOTTOM = 1; //0001
	//Outcodes for P0, P1, and whatever point lies outside the clip rectangle
	int outcode0, outcode1, outcodeOut;
	bool accept = false, done = false;
 
	//compute outcodes
	outcode0 = out_code(x0, y0);
	outcode1 = out_code(x1, y1);
 
	do
	{
		if (!(outcode0 | outcode1))      //logical or is 0. Trivially accept and get out of loop
		{
			accept = true;
			done = true;
		}
		else if (outcode0 & outcode1)//logical and is not 0. Trivially reject and get out of loop
        {
			done = true;
        }
		else
		{
			//failed both tests, so calculate the line segment to clip
			//from an outside point to an intersection with clip edge
			int x, y;
			//At least one endpoint is outside the clip rectangle; pick it.
			outcodeOut = outcode0 ? outcode0: outcode1;
			//Now find the intersection point;
			//use formulas y = y0 + slope * (x - x0), x = x0 + (1/slope)* (y - y0)
			if (outcodeOut & TOP)          //point is above the clip rectangle
			{
				x = x0 + (x1 - x0) * (ymax - y0)/(y1 - y0);
				y = ymax;
			}
			else if (outcodeOut & BOTTOM)  //point is below the clip rectangle
			{
				x = x0 + (x1 - x0) * (ymin - y0)/(y1 - y0);
				y = ymin;
			}
			else if (outcodeOut & RIGHT)   //point is to the right of clip rectangle
			{
				y = y0 + (y1 - y0) * (xmax - x0)/(x1 - x0);
				x = xmax;
			}
			else if (outcodeOut & LEFT)                         //point is to the left of clip rectangle
			{
				y = y0 + (y1 - y0) * (xmin - x0)/(x1 - x0);
				x = xmin;
			}
			//Now we move outside point to intersection point to clip
			//and get ready for next pass.
			if (outcodeOut == outcode0)
			{
				x0 = x;
				y0 = y;
				outcode0 =out_code(x0, y0);
			}
			else 
			{
				x1 = x;
				y1 = y;
				outcode1 = out_code(x1, y1);
			}
		}
	}while (!done);
 
	if (accept)
	{       
		bresenham(x0,y0,x1,y1,color);
	}
}

template <typename T>
void rasterizer_2d<T>::raster_circle(int x0,int y0,int radius, const T& color)
{	
	int f = 1 - radius;
	int ddF_x = 0;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;
 
	set_pixel(x0,  y0 + radius,color);
	set_pixel(x0, y0 - radius,color);
	set_pixel(x0 + radius, y0,color);
	set_pixel(x0 - radius, y0,color);
 
	while(x < y) 
	{
		if(f >= 0) 
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;    
		set_pixel(x0 + x, y0 + y,color);
		set_pixel(x0 - x, y0 + y,color);
		set_pixel(x0 + x, y0 - y,color);
		set_pixel(x0 - x, y0 - y,color);
		set_pixel(x0 + y, y0 + x,color);
		set_pixel(x0 - y, y0 + x,color);
		set_pixel(x0 + y, y0 - x,color);
		set_pixel(x0 - y, y0 - x,color);
	}
}

template <typename T>
void rasterizer_2d<T>::raster_solid_circle(int x0, int y0, int radius, const T& color)
{
	int xxmin = clamp(x0-radius,xmin,xmax);
	int xxmax = clamp(x0+radius,xmin,xmax);
	int yymin = clamp(y0-radius,ymin,ymax);
	int yymax = clamp(y0+radius,ymin,ymax);
	int rr = radius*radius;
	for (int x = xxmin; x < xxmax; x++)
	{
		for (int y= yymin; y < yymax; y++)
		{
			int d = sqr(x- x0) +sqr(y-y0) -rr;
			if(d <= 0)
				set_pixel_unchecked(x,y,color);
		}
	}

}

template <typename T>
void rasterizer_2d<T>::raster_ellipse( double xc, double yc,double r1, double r2, double theta,const T& col, int n=32)
{
	int x1,y1;
	double st = sin(theta);
	double ct = cos(theta);

	std::vector<tiny_vec<int,2> >pts(n+1);
	int j=0;
	for (int i = 0; i < (n+1)*360/n; i += 360 / n) 
	{
		
		double alpha = i * (3.14159 / 180) ;
		double sinalpha = sin(alpha);
		double cosalpha = cos(alpha);
 		x1 =(int)( xc + (r1 * cosalpha * ct - r2 * sinalpha * st) +0.5);
		y1 =(int)( yc + (r1* cosalpha * st + r2 * sinalpha * ct) +0.5);
		pts[j++].set(x1,y1);
	}
	raster_polygon(pts,col);

	
}

template <typename T>
void rasterizer_2d<T>::raster_triangle(int x1, int y1, int x2, int y2, int x3, int y3, const T& color)
{
	raster_line(x1,y1,x2,y2,color);
	raster_line(x2,y2,x3,y3,color);
	raster_line(x3,y3,x1,y1,color);
}

template <typename T>
void rasterizer_2d<T>::raster_solid_triangle(int x1, int y1, int x2, int y2, int x3, int y3, const T& color) 
{
	int xxmin = clamp(min(x1,x2,x3),xmin,xmax);
	int xxmax = clamp(max(x1,x2,x3),xmin,xmax);
	int yymin = clamp(min(y1,y2,y3),ymin,ymax);
	int yymax = clamp(max(y1,y2,y3),ymin,ymax);

	tiny_vec<float,2> A((float)x1,(float)y1);
	tiny_vec<float,2> B((float)x2,(float)y2);
	tiny_vec<float,2> C((float)x3,(float)y3);
	tiny_vec<float,2> nmlAB(A[1]-B[1], B[0]-A[0]);
	float lAB = nmlAB.length();
	if(lAB == 0)
	{
		raster_line(x1,y1,x3,y3,color);
		return;
	}
		

	tiny_vec<float,2> nmlBC(B[1]-C[1], C[0]-B[0]);
	float lBC = nmlBC.length();
	if(lBC == 0)
	{
		raster_line(x1,y1,x3,y3,color);
		return;
	}
		

	tiny_vec<float,2> nmlCA(C[1]-A[1], A[0]-C[0]);
	float lCA = nmlCA.length();
	if(lCA == 0)
	{
		raster_line(x2,y2,x3,y3,color);
		return;
	}
		
	nmlAB/=lAB;
	float dAB = dot(nmlAB,A);
	nmlBC/=lBC;
	float dBC = dot(nmlBC,B);
	nmlCA/=lCA;
	float dCA = dot(nmlCA,C);

	float inv_distA_BC = 1.0f/(dot(A,nmlBC)-dBC);
	float inv_distB_CA = 1.0f/(dot(B,nmlCA)-dCA);
	float inv_distC_AB = 1.0f/(dot(C,nmlAB)-dAB);

	tiny_vec<float,2> P;
	for (int x = xxmin; x < xxmax; x++)
	{
		for (int y= yymin; y < yymax; y++)
		{
			P[0]= (float)x;
			P[1]= (float)y;
				
			float t = dot(P,nmlBC);
			float alpha =  (dot(P,nmlBC)-dBC) *inv_distA_BC;
			if(alpha < -0.0001f) 
				continue;
			t = dot(P,nmlCA);
			float beta =  (dot(P,nmlCA)-dCA) *inv_distB_CA;
			if(beta < -0.0001f)
				continue;
			t = dot(P,nmlAB);
			float gamma = (dot(P,nmlAB)-dAB) *inv_distC_AB;
			if(gamma < -0.0001f)
				continue;
			//color = alpha * color(A) + beta * color(B) + gamma * color(C);
				
			set_pixel_unchecked(x,y,color);
			}
	}


}

template <typename T>
void rasterizer_2d<T>::raster_polygon(const std::vector<tiny_vec<int,2>>& pts, const T& color)
{
	if(pts.empty())
		return;
		
	int n = pts.size()-1;
		
	for(int i = 0; i < n; i++)
		raster_line(pts[i][0],pts[i][1],pts[i+1][0],pts[i+1][1],color);

	raster_line(pts[n][0],pts[n][1],pts[0][0],pts[0][1],color);

}

template <typename T>
void rasterizer_2d<T>::set_pixel_unchecked(int x, int y,const T& color)
{
	(*img_ptr)(x,y)=color;
}

template <typename T>
int rasterizer_2d<T>::out_code(double x, double y)
{
	const int RIGHT = 8;  //1000
	const int TOP = 4;    //0100
	const int LEFT = 2;   //0010
	const int BOTTOM = 1; //0001

	int code = 0;
	if (y > ymax)              //above the clip window
		code |= TOP;
	else if (y < ymin)         //below the clip window
		code |= BOTTOM;
	if (x > xmax)              //to the right of clip window
		code |= RIGHT;
	else if (x < xmin)         //to the left of clip window
		code |= LEFT;
	return code;
}


template <typename T>
void rasterizer_2d<T>::bresenham(int x1, int y1, int x2, int y2,const T& color)
{
	int x, y, t, dx, dy, incx, incy, pdx, pdy, ddx, ddy, es, el, err;
 
	dx = x2 - x1;
	dy = y2 - y1;
 
	incx = sign(dx);
	incy = sign(dy);
	if(dx<0) dx = -dx;
	if(dy<0) dy = -dy;
 
	if (dx>dy)
	{
		pdx=incx; pdy=0;    /* pd. ist Parallelschritt */
		ddx=incx; ddy=incy; /* dd. ist Diagonalschritt */
		es =dy;   el =dx;   /* Fehlerschritte schnell, langsam */
	} else
	{
		pdx=0;    pdy=incy; /* pd. ist Parallelschritt */
		ddx=incx; ddy=incy; /* dd. ist Diagonalschritt */
		es =dx;   el =dy;   /* Fehlerschritte schnell, langsam */
	}
 
	x = x1;
	y = y1;
	err = el/2;
	set_pixel_unchecked(x,y,color);

	for(t=0; t<el; ++t) 
	{
		err -= es; 
		if(err<0)
		{
			err += el;
			x += ddx;
			y += ddy;
		} 
		else
		{
			
			x += pdx;
			y += pdy;
		}
		set_pixel_unchecked(x,y,color);
	} 
}
	

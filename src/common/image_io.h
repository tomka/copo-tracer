// Copyright: The original source code was provided by Sören König (Soeren.Koenig
// AT tu-dresden.de). You are free to modify and/or redistribute that code in
// terms of the BSD license.

#pragma once

#include "tiny_vec.h"
#include "mat.h"
#include <cstdio>
#include <cstdlib>
#include <string>


inline void write16bit(int n, FILE* fp) 
{ 
  unsigned char bytes[] = { n % 256, n / 256 };
  fwrite(bytes, 2, sizeof(unsigned char),fp);
}

inline bool write_tga( mat<tiny_vec<unsigned char,3> >& img, const std::string& filename) 
{
	FILE *fp =fopen(filename.c_str(),"wb");
	{
        perror( "error" );
		std::cerr << "can't open file for writing: "<< filename<<"\n";

		return false;
	}
	// fixed header values for the subset of TGA we use for writing
    unsigned char TGAHeaderColor[12] = 
    { 0,// 0 ID length = no id
      0,// 1 color map type = no color map
      2,// 2 image type = uncompressed true color
      0, 0, 0, 0, 0,// color map spec = empty
      0, 0,  // x origin of image 
      0, 0   // y origin of image
    };

    fwrite(TGAHeaderColor, 12, sizeof(unsigned char),fp);

    write16bit(img.w(),fp);  
    write16bit(img.h(),fp);  

    putc(24,fp); 
    putc(0x00,fp);
    unsigned char r,g,b;
    for(int y = (int)img.h()-1; y >= 0; y--)
        for(int x = 0; x < (int)img.w(); x++)
        {
            r = img(x,y)[0];
            g = img(x,y)[1];
            b = img(x,y)[2];
         

            fwrite((void*)&b,1 , sizeof(char),fp);
            fwrite((void*)&g,1 , sizeof(char),fp);
            fwrite((void*)&r,1 , sizeof(char),fp);
          

        }


    fclose(fp);
    return true;
}


inline bool write_bmp(const mat<tiny_vec<unsigned char,3> >& img, const std::string& filename)
{
	
    unsigned short width = (unsigned short) img.w();
	unsigned short height = (unsigned short) img.h();
	const unsigned char* data = (const unsigned char*)img.begin();
	
	FILE* fp = fopen(filename.c_str(), "wb");
	if (!fp) 
	{
        perror( "error" );
		std::cerr << "can't open file for writing: "<< filename<<"\n";

		return false;
	}
	
	bool success = fp != 0;
	if (success) 
	{
		static unsigned char bmp_header[54] = 
		{
			66,  77,  90,   0,   0,   0,   0,   0,   0,   0,  54,   0,   0,   0,  40,   0,   0,   0,   4,   1,   0,   0,   3,   0,   0,   0,   1,   0,  24,   0,   0,   0,   0,   0,  36,   0,   0,   0, 196,  14,   0,   0, 196,  14,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 
		};

		((unsigned short*)bmp_header)[9]  = width;
		((unsigned short*)bmp_header)[11] = height;
		bool success = fwrite(bmp_header, 1, 54, fp) == 54;
		if (success) 
		{
			 int bytes_per_line = width*3;
			 int padded=0;
			 if(bytes_per_line % 4 != 0)
				 padded=(4-bytes_per_line %4);
			data += (height-1)*bytes_per_line;
			for (unsigned short y = 0; success && y < height; ++y) 
			{
				for (unsigned short x = 0; x < width; ++x, data += 3)
				{
					
					unsigned char col[3] = { data[2], data[1], data[0] };
					if (!(success = fwrite(col, 1, 3, fp) == 3) )
						break;
				}
				int pad = 0;
				if(padded != 0)
					fwrite(&pad, 1, padded, fp);

				data -= 2*bytes_per_line;
		

			}
		}
		if(!success)
			std::cerr<< "could not write bmp header\n";
		fclose(fp);
		return success;

	}
	return success;
}



inline unsigned int endianReadInt(FILE* file)
{
	unsigned char  b[4]; 
	unsigned int i;

   if ( fread( b, 1, 4, file) < 4 )
     return 0;
   /* LITTLE VS BIG ENDIAN LINES - comment out one or the other */
   i = (b[3]<<24) | (b[2]<<16) | (b[1]<<8) | b[0]; // big endian
   //i = (b[0]<<24) | (b[1]<<16) | (b[2]<<8) | b[3]; // little endian
   return i;
}

/* Reads a 16 bit integer; comment out one or the other shifting line below, 
whichever makes your system work right. */
inline unsigned short int endianReadShort(FILE* file) 
{
	unsigned char  b[2]; 
	unsigned short s;

   if ( fread( b, 1, 2, file) < 2 )
     return 0;
   /* LITTLE VS BIG ENDIAN LINES - comment out one or the other */
   s = (b[1]<<8) | b[0]; // big endian
   //s = (b[0]<<8) | b[1]; // little endian
   return s;
}
						  

inline bool read_bmp(mat<tiny_vec<unsigned char,3> >& img, const std::string& filename)
{
    FILE *file;
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1) 
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // temporary color storage for bgr-rgb conversion.
	int width,height;

    // make sure the file is there.
    if ((file = fopen(filename.c_str(), "rb"))==NULL)
    {
		std::cerr<<"File Not Found : "<<filename<<std::endl;
		return false;
    }
    
    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);

    // read the width
    if (!(width = endianReadInt(file))) 
	{
		std::cerr <<"Error reading width from "<< filename<<std::endl;
		return false;
    }
   // std::cout<<"Width of "<< filename<<": "<<width<<std::endl;
    
    // read the height 
    if (!(height = endianReadInt(file))) 
	{
		std::cerr<<"Error reading height from "<< filename<<std::endl;
		return false;
    }
	 // read the planes
    if (!(planes=endianReadShort(file))) 
	{
		std::cerr<<"Error reading planes from "<< filename<<std::endl;
		return false;
    }
    if (planes != 1) 
	{
		std::cerr<<"Planes from " <<filename <<" is not 1: "<<planes<<std::endl;
		return false;
    }
	  // read the bits per pixel
    if (!(bpp = endianReadShort(file))) 
	{
		std::cerr<< "Error reading bpp from "<< filename<<std::endl;
		return false;
    }
    if (bpp != 24) 
	{
		std::cerr <<"Bpp from "<< filename << " is not 24: "<<bpp<<std::endl;
		return false;
    }
	
    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);
  //  std::cout<<"Height of " <<filename<<": "<< height<<std::endl;
    
    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = width * height * 3;
	int bytes_per_line = width*3;
	int padded=0;
	if(bytes_per_line % 4 != 0)
		padded=(4-bytes_per_line %4);

  

    // read the data. 
	img.resize(width,height);

	
	
	
    

	int pad;
	unsigned char* image_data = ( unsigned char*)img.begin();
	for(int i = 0; i < height; i++)
	{
		fread(image_data,bytes_per_line,1,file);
		if(padded > 0)
			fread(&pad,padded,1,file);
		image_data+=bytes_per_line;

	}
	image_data = ( unsigned char*)img.begin();
	


    for (i=0;i<size;i+=3) 
	{ // reverse all of the colors. (bgr -> rgb)
		temp = image_data[i];
		image_data[i] = image_data[i+2];
		image_data[i+2] = temp;
	}
	img.flipud();
	

    
    // we're done.
    return true;
}
	
	


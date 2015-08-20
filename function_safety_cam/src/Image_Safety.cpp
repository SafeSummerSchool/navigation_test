//#pragma once
//#include <stdafx.h>
#include "Image_Safety.h"

namespace safety
{

#define NCOLORS 256
#define NCHANNELS 1

	safe::safe()
	{
		error_length = 0;
		error_size = 0;
	}
	safe::~safe()
	{
		stored_Histogram.~histogram();
		stored_double_Histogram.~histogram();
	}
	void safe::reset()
	{
		error_length = 0;
		error_size = 0;
		stored_Histogram.~histogram();
		stored_double_Histogram.~histogram();
	}
	
		/*

	int main(int argc, char * argv[])
	{
		unsigned int * histogram; // the histogram
		int bins_per_color, width, height;
		unsigned char * data;


		// compute the histogram
		histogram = compute_histogram(data, width, height, bins_per_color);
		if (!histogram)
		{
			printf("Error while computing histogram.\n");
			return 2;
		}
		delete_data(data);

		// write histogram to file
		//write_histogram(histogram, bins_per_color, argv[2]);
		delete_histogram(histogram);
		return 0;
	}*/
	std::vector<int> safe::get_error_count()
	{
		std::vector<int> total_errors;
		total_errors.push_back(error_length);
		total_errors.push_back(error_size);
		return total_errors;
	}
	
	histogram<unsigned int>  safe::do_histogram(unsigned char * pixels, int width, int height, int bins_per_color)
	{
		return compute_histogram(pixels, width, height, bins_per_color);
	}
	histogram<double> safe::do_uniform_histogram(unsigned char * pixels, int width, int height, int bins_per_color, bool uniform, bool accumulate)
	{
		return compute_histogram_uniform(pixels, width, height, bins_per_color, uniform, accumulate);
	}
	histogram<double> safe::compute_histogram_uniform(unsigned char * pixels, int width, int height, int bins_per_color, bool uniform, bool accumulate)
	{
		int i, j, q;
		unsigned char g, bin_g;
		histogram<double> his(true);
		his.uniform = uniform;
		stored_double_Histogram.uniform = accumulate;

		//make sure bins_per_color is power of 2
		q = bins_per_color;
		while (q > 1)
		{
			if (q % 2)
			{
				printf("Number of bins is not power of 2.\n");
				return his;
			}
			q /= 2;
		}

		// allocate memory for the histogram
		his.init_vec(NCOLORS / bins_per_color);

		// compute histogram
		for (i = 0; i<height; i++)
			for (j = 0; j<width; j++)
			{
				g = pixels[i * width * NCHANNELS + j * NCHANNELS];
				if (g > 0)
				{
					q = bins_per_color;
					while (q > 1)
					{
						g = std::floor(g / 2);
						q /= 2;
					}
					bin_g = g;
				}
				else
					bin_g = 0;
				his.increment(bin_g);
			}
		if (uniform)
			his.Uniform(width*height);
		if (accumulate)
		{
			if (stored_double_Histogram.size() == (NCOLORS / bins_per_color))
			{
				;
			}
			else
			{
				stored_double_Histogram.init_vec(NCOLORS / bins_per_color);
			}
			if (uniform)
			{
				for (i = 0; i < (NCOLORS / bins_per_color); i++)
					stored_double_Histogram.addUni(i, his.get(i));
			}
			else
			{
				for (i = 0; i < (NCOLORS / bins_per_color); i++)
					stored_double_Histogram.addUni(i, his.get(i)/(width*height));
			}
		}
		his.error = false;
		return his;
	}
	histogram<unsigned int> safe::compute_histogram(unsigned char * pixels, int width, int height, int bins_per_color)
	{
		int i, j, q;
		unsigned char g, bin_g;
		histogram<unsigned int> his(true);

		//make sure bins_per_color is power of 2
		q = bins_per_color;
		while (q > 1)
		{
			if (q % 2)
			{
				printf("Number of bins is not power of 2.\n");
				return his;
			}
			q /= 2;
		}
		// allocate memory for the histogram
		his.init_vec(NCOLORS / bins_per_color);

		// compute histogram
		for (i = 0; i<height; i++)
			for (j = 0; j<width; j++)
			{
				g = pixels[i * width * NCHANNELS + j * NCHANNELS];
				if (g > 0)
				{
					q = bins_per_color;
					while (q > 1)
					{
						g = floor(g / 2);
						q /= 2;
					}
					bin_g = g;
				}
				else
					bin_g = 0;
				his.increment(bin_g);
			}
		
		his.error = false;
		return his;
	}
	void safe::write_histogram(unsigned int * histogram, int bins_per_color,
		char * file_name)
	{
		int i, j, k;
		FILE * out = fopen(file_name, "w"); // open file
		if (!out)
		{
			printf("Unable to create file.\n");
			return;
		}
		// write file
		for (i = 0; i < bins_per_color; i++)
		{
			for (j = 0; j < bins_per_color; j++)
			{
				for (k = 0; k < bins_per_color; k++)
					fprintf(out, "%d,",
					histogram[i * bins_per_color * bins_per_color + j * bins_per_color + k]);

				fprintf(out, "\n");
			}
			fprintf(out, "\n");
		}
		fclose(out);
	}

	void safe::delete_histogram(unsigned int * histogram)
	{
		delete[] (histogram);
	}
	void safe::delete_data(unsigned char * histogram)
	{
		delete[](histogram);
	}
}

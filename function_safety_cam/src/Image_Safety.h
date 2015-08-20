#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>


namespace safety
{

	template<typename TT>
	struct histogram {
	public:	
		bool error;
		bool uniform;
		void init_vec(size_t _size)
		{
			h.clear();
			h.resize(_size);
		}
		void increment(int pos)
		{
			h[pos]++;
		}
		void Uniform(unsigned int _size)
		{
			for (int i = 0; i < h.size(); i++)
				h[i] = h[i] / _size;
		}
		int size()
		{
			return h.size();
		}
		TT max()
		{
			TT maximum = 0;
			for (int i = 0; i < h.size(); i++)
				if (maximum < h[i])
					maximum = h[i];
			return maximum;
		}
		TT min()
		{
			TT minimum = h[0];
			for (int i = 0; i < h.size(); i++)
				if (minimum > h[i])
					minimum = h[i];
			return minimum;
		}
		unsigned int nonempty()
		{
			unsigned int empty = 0;
			for (int i = 0; i < h.size(); i++)
				if (h[i] < 1)
					empty++;
			return empty;
		}
		void addUni(int pos, unsigned int Val)
		{
			if (h[pos]>0)
				h[pos] = (h[pos] + Val)/2;
			else
				h[pos] = Val;
		}
		TT get(int pos)
		{
			return h[pos];
		}
		histogram()
		{
			error = false;
		}
		histogram(bool _error)
		{
			error = _error;
		}
		~histogram()
		{
			h.clear();
		}
	private:
		std::vector<TT> h;
	};

	class safe
	{


	public:
		//Construct
		safe();
		~safe();
		//Functions
		void reset();
		std::vector<int> get_error_count();

		histogram<unsigned int> do_histogram(
			unsigned char * pixels, int width, int height, int bins_per_color);
		
		histogram<double>  do_uniform_histogram(
			unsigned char * pixels, int width, int height, int bins_per_color, bool uniform
			, bool accumulate);
	private:
		// computes a histogram from pixel data
		histogram<double> compute_histogram_uniform(
			unsigned char * pixels, int width, int height, int bins_per_color, bool uniform
			, bool accumulate);

		histogram<unsigned int>  compute_histogram(
			unsigned char * pixels, int width, int height, int bins_per_color);

		// frees the memory allocated by compute_histogram
		void delete_histogram(unsigned int * histogram);
		// frees the memory allocated by compute_histogram
		void delete_data(unsigned char * data);

		// writes a histogram to a text file
		void write_histogram(unsigned int * histogram, int bins_per_color,
			char * file_name);


		//Variables
		int error_length;
		int error_size;
		histogram<unsigned int> stored_Histogram;
		histogram<double> stored_double_Histogram;
	};
}

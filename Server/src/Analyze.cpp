#include "Analyze.h"

// FFTW3
#include <fftw3.h>

#include <vector>
#include <climits>
#include <cmath>
#include <iostream>

using namespace std;

static const vector<int> band_limits = {	44,		88,
											88,		177,
											177,	355,
											355,	710,
											710,	1420,
											1420,	2840,
											2840,	5680,
											5680,	11360,
											11360,	22720	};

template<class T>
static T mean(const vector<T>& container) {
	double sum = 0;
	
	for(const auto& element : container)
		sum += element;
		
	return sum / (double)container.size();	
}

static double calculateSD(const vector<double>& data) {
	double sum = 0;
	double mean = 0;
	double std = 0;
	
	for (auto& value : data)
		sum += value;
		
	mean = sum / data.size();
	
	for (auto& value : data)
		std += pow(value - mean, 2);
		
	return sqrt(std / data.size());	
}

static int getBandIndex(double frequency) {
	for (size_t i = 0; i < band_limits.size(); i += 2) {
		auto& lower = band_limits.at(i);
		auto& higher = band_limits.at(i + 1);
		
		if (frequency > lower && frequency < higher)
			return i / 2;
	}
	
	return -1;
}

namespace nac {
	FFTOutput fft(const vector<short>& samples) {
		vector<double> in;
		
		for (auto& sample : samples)
			in.push_back((double)sample / SHRT_MAX);

		int N = samples.size();
		double fs = 48000;
		
		fftw_complex *out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
		fftw_plan plan_forward = fftw_plan_dft_r2c_1d(N, in.data(), out, FFTW_ESTIMATE);
		fftw_execute(plan_forward);

		vector<double> v;

		for (int i = 0; i <= ((N/2)-1); i++)
			//v.push_back((20 * log(sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]))) / N);	// dB
			v.push_back(sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]) / N);				// linear
		
		vector<double> frequencies;
		vector<double> magnitudes;
		
		for (int i = 0; i <= ((N / 2) - 1); i++) {
			frequencies.push_back(fs * i / N);
			magnitudes.push_back(v[i]);
		}

		fftw_destroy_plan(plan_forward);
		fftw_free(out);
		
		return { frequencies, magnitudes };
	}
	
	BandOutput calculate(const FFTOutput& input) {
		auto& frequencies = input.first;
		auto& magnitudes = input.second;
		
		vector<double> band_energy(band_limits.size() / 2, 0);
		vector<double> band_nums(band_limits.size() / 2, 0);
		
		// Analyze bands
		for (size_t i = 0; i < frequencies.size(); i++) {
			auto& frequency = frequencies.at(i);
			auto index = getBandIndex(frequency);
			
			if (index < 0)
				continue;
			
			band_energy.at(index) += magnitudes.at(i);	
			band_nums.at(index)++;
		}
		
		for (size_t i = 0; i < band_energy.size(); i++) {
			band_energy.at(i) /= band_nums.at(i);
			
			cout << "Energy band " << i << "\t " << band_energy.at(i) << endl;
		}
		
		auto std_dev = calculateSD(band_energy);
		
		cout << "Standard deviation: " << std_dev << endl;
		
		return band_energy;
	}
}
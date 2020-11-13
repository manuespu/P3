/// @file

#include <iostream>
#include <fstream>
#include <string.h>
#include <errno.h>

#include "wavfile_mono.h"
#include "pitch_analyzer.h"

#include "docopt.h"

#define FRAME_LEN   0.030 /* 30 ms. */
#define FRAME_SHIFT 0.015 /* 15 ms. */

using namespace std;
using namespace upc;

static const char USAGE[] = R"(
get_pitch - Pitch Detector 

Usage:
    get_pitch [options] <input-wav> <output-txt>
    get_pitch (-h | --help)
    get_pitch --version

Options:
    -h, --help  Show this screen
    --version   Show the version of the project

Arguments:
    input-wav   Wave file with the audio signal
    output-txt  Output file: ASCII file with the result of the detection:
                    - One line per frame with the estimated f0
                    - If considered unvoiced, f0 must be set to f0 = 0
)";
float abs_f(float value){  
  if (value < 0.0)
    return -1.0*value;
  return value; 
}

int main(int argc, const char *argv[]) {
	/// \TODO 
  /// \DONE
	///  Modify the program syntax and the call to **docopt()** in order to
	///  add options and arguments to the program.
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
        {argv + 1, argv + argc},	// array of arguments, without the program name
        true,    // show help if requested
        "2.0");  // version string

	std::string input_wav = args["<input-wav>"].asString();
	std::string output_txt = args["<output-txt>"].asString();

  // Read input sound file
  unsigned int rate;
  vector<float> x;
  if (readwav_mono(input_wav, rate, x) != 0) {
    cerr << "Error reading input file " << input_wav << " (" << strerror(errno) << ")\n";
    return -2;
  }

  int n_len = rate * FRAME_LEN;
  int n_shift = rate * FRAME_SHIFT;

  // Define analyzer
  PitchAnalyzer analyzer(n_len, rate, PitchAnalyzer::HAMMING, 50, 500);

  /// \TODO
  /// \DONE
  /// Preprocess the input signal in order to ease pitch estimation. For instance,
  /// central-clipping or low pass filtering may be used.
  
  float max = -1.0e10, min = 1.0e10;
  float Cl1, Cl2, Cl; // Clipping first 1/3 and last 1/3 
  // Iterate for each frame and save values in f0 vector
  vector<float>::iterator iX;
  for(iX = x.begin(); iX<x.begin() + (int)(x.size()/3); iX++){    
    if(*iX > max)
      max = *iX;   
    if(*iX < min)      
      min = *iX;  
  }
  if(abs_f(min) > abs_f(max))   
    Cl1 = abs_f(min);  
  else    
    Cl1 = abs_f(max); 
 
  for(iX = x.end() - (int)(x.size()/3); iX<x.end(); iX++){   
    if(*iX > max)      
      max = *iX;
    if(*iX < min)      
      min = *iX;  
    }  
    if(abs_f(min) > abs_f(max))   
      Cl2 = abs_f(min);  
    else    
      Cl2 = abs_f(max); // Get Max in magnitude // Get minimum of both the first and last part of the signal  
    Cl = Cl2;
    if(Cl1 < Cl2)    
      Cl = Cl1;   
    Cl = 0.001*Cl;  
    cout << Cl << endl; 

  for(iX = x.begin(); iX<x.end(); iX++){
    if(abs_f(*iX)<Cl)      
      *iX = 0;    
    else{
      if(*iX > 0)        
        *iX = *iX - Cl;
      else        
        *iX = *iX + Cl;    
    } 
  }

  vector<float> f0;
  float aux; 
  for (iX = x.begin(); iX + n_len < x.end(); iX = iX + n_shift) {
    cout << (iX - x.begin())/300 << endl << endl; 
    float f = analyzer(iX, iX + n_len);
    f0.push_back(f);
  }

  // JUST ODD NUMBERS  
  int F_size = 1;  
  vector<float> filter; 

  /// \TODO
  /// \DONE
  /// Postprocess the estimation in order to supress errors. For instance, a median filter
  /// or time-warping may be used.
  for(iX = f0.begin(); iX<f0.end()-(F_size-1); iX += 1){
    // fill filter    
    for(int i = 0; i<F_size; i++)      
      filter.push_back(*(iX+i));
    // Order filter  ​
    int k, l;

    for(k = 0; k < F_size-1; k++){
      for(l = 0; l < F_size-k-1; l++){
        if (filter[l] > filter[l+1]){        
          aux = filter[l];        
          filter[l] = filter[l+1]; 
          filter[l+1] = aux;      
        }    
      }   
    }
    // Get median    
    f0[iX - f0.begin()] = filter[F_size/2];
    filter.clear();  
  } 

  //FILE

  // Write f0 contour into the output file
  ofstream os(output_txt);
  if (!os.good()) {
    cerr << "Error reading output file " << output_txt << " (" << strerror(errno) << ")\n";
    return -3;
  }

  os << 0 << '\n'; //pitch at t=0
  for (iX = f0.begin(); iX != f0.end(); ++iX) 
    os << *iX << '\n';
  os << 0 << '\n';//pitch at t=Dur


  return 0;
}

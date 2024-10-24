#include <iostream>       // For input/output operations (std::cout, std::cerr)
#include <cmath>          // For math operations (though not used directly in this code)
#include <cstdlib>        // For conversions like std::atoi (convert strings to integers)
#include <chrono>         // For measuring time (std::chrono)
#include <iomanip>        // For controlling the output format (std::setprecision)

// Declare the external functions (f1, f2, f3, f4) that will be used for numerical integration
// These are provided elsewhere i.e. libfunctions.a (Need to be linked during compilation)
#ifdef __cplusplus
extern "C" {              // Ensure compatibility with C linkage
#endif
float f1(float x, int intensity);  // Function f1 to integrate
float f2(float x, int intensity);  // Function f2 to integrate
float f3(float x, int intensity);  // Function f3 to integrate
float f4(float x, int intensity);  // Function f4 to integrate
#ifdef __cplusplus
}
#endif

// Define a type for function pointers that take two arguments (float x, int intensity)
typedef float (*FuncPtr)(float, int);

// Function to perform numerical integration using the midpoint rule
// Takes in:
//    f: the function to integrate (pointer to function f1, f2, etc.)
//    a: lower bound of integration
//    b: upper bound of integration
//    n: number of points to use for approximation
//    intensity: a parameter to pass to the function (controls computation intensity)
float numericalIntegration(FuncPtr f, float a, float b, int n, int intensity) {
    float sum = 0.0f;                 // Variable to accumulate the result of the integration
    float dx = (b - a) / n;           // Step size (difference between consecutive points)

    // Loop through n points and apply the midpoint rule
    for (int i = 0; i < n; ++i) {
        float x = a + (i + 0.5f) * dx;  // Midpoint of the current interval
        sum += f(x, intensity);         // Add the value of the function at this midpoint to the sum 
                                        // this will call functions f1, f2, f3, or f4 based on the function ID in i.e. implementation is in libfunctions.a
    }
    
    return sum * dx;                   // Multiply by dx to get the final integral approximation
}

int main(int argc, char* argv[]) {
    // Check if the user provided enough command-line arguments (must provide 6: program name + 5 args)
    if (argc < 6) {
        // If not enough arguments, print usage information and exit with error
        std::cerr << "usage: " << argv[0] << " <functionid> <a> <b> <n> <intensity>" << std::endl;
        return -1;
    }

    
    int functionId = std::atoi(argv[1]);   // function ID (which function to use)
    float a = std::atof(argv[2]);          // lower bound (a) of the integration
    float b = std::atof(argv[3]);          // upper bound (b) of the integration
    int n = std::atoi(argv[4]);            // number of points (n) for the approximation
    int intensity = std::atoi(argv[5]);    // intensity for the function (slows down computation)

    // Select the correct function (f1, f2, f3, or f4) based on the function ID provided
    FuncPtr selectedFunc;  // Pointer to the selected function
    switch (functionId) {
        case 1: selectedFunc = f1; break;  // If functionId is 1, use f1
        case 2: selectedFunc = f2; break;  // If functionId is 2, use f2
        case 3: selectedFunc = f3; break;  // If functionId is 3, use f3
        case 4: selectedFunc = f4; break;  // If functionId is 4, use f4
        default:
            // If functionId is not 1, 2, 3, or 4, print error message and exit
            std::cerr << "Error: Invalid function ID (must be 1, 2, 3, or 4)" << std::endl;
            return -1;
    }

    // Start measuring the time taken for numerical integration
    auto start = std::chrono::high_resolution_clock::now();  // Get the start time

    // Perform the numerical integration using the selected function
    float result = numericalIntegration(selectedFunc, a, b, n, intensity);

    // Stop measuring the time after the integration is complete
    auto end = std::chrono::high_resolution_clock::now();    // Get the end time
    std::chrono::duration<double> elapsed = end - start;     // Calculate the elapsed time

    // Output both the result and time to stdout, separated by a space
    std::cout << std::setprecision(15) << result << " " 
              << std::fixed << std::setprecision(6) << elapsed.count() << std::endl;

    return 0;  // Exit the program successfully
}

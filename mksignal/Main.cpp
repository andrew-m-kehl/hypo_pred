#include "Stle/U/ArgU.hpp"

#include <cmath>
#include <vector>
#include <memory>
#include <cstring>

#ifdef _WIN32

#include "Stle/U/WinU.hpp"

int u8Main(int argc, char **u8Argv);

int wmain(int argc, wchar_t **argv) {
    std::unique_ptr<char*, U::ArgvDelete> u8Argv = U::makeU8Argv(argc, argv);
    return u8Main(argc, u8Argv.get());
}

#else

int u8Main(int argc, char **u8Argv);

int main(int argc, char **argv) {
    return u8Main(argc, argv);
}

#endif

// MSVC does not define some math constants by default.
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#define M_E 2.71828182845904523536
#endif

class Function {
public:
    double call(double t) const {
        return (*this)(t);
    }
    
    virtual double operator()(double t) const = 0;
    
    virtual ~Function() = default;
    // Need to re-enable moving after adding a virtual destructor.
    Function(Function &&) = default;
    Function &operator=(Function &&) = default;
    Function(const Function &) = default;
    Function &operator=(const Function &) = default;
    Function() = default;
};

class Decay : public Function {
public:
    Decay()
        : sinAmplitude(1), sinPeriod(250), sinPhase(0), power(1/250.0) {        
    }
    
    virtual double operator()(double t) const override {        
        return sinAmplitude * sin(2 * M_PI * t / sinPeriod + sinPhase) * exp(-power*t);
    }    
    
    double sinAmplitude;
    double sinPeriod;
    double sinPhase;    
    double power;
};

class Gauss : public Function {
public:
    Gauss() 
        : amplitude(1), centre(50), width(10) {        
    }
    
    virtual double operator()(double t) const override {
        
        return amplitude * exp(- (t - centre) * (t - centre) / (2 * width * width));
    }
    
    double amplitude;
    double centre;
    double width;
};

const char *USAGE =
    "mksignal START END STEP PERIOD FUCTION1 FUNCTION2 ...\n";

int u8Main(int argc, char** argv) {
    try {
        U::ArgReader argReader(argc, argv);
        // Skipping the first argument (executable path).
        argReader.read();
        
        double startT = argReader.readDouble();
        double endT = argReader.readDouble();
        double stepT = argReader.readDouble();
        double period = argReader.readDouble();
        
        std::vector<std::unique_ptr<Function>> functions;
        
        bool eof = false;
        while (!eof) {
            U::ArgToken arg = argReader.read();
            switch (arg) {
            case U::ARG_KEY: {
                std::stringstream ss;
                ss << "Unknown option: '" << argReader.str() << "'.";
                throw U::ArgError(ss.str());
                break;
            }
                
            case U::ARG_VALUE: {
                if (!strcmp(argReader.str(), "decay")) {
                    std::unique_ptr<Decay> decay = std::make_unique<Decay>();
                    decay->sinAmplitude = argReader.readDouble();
                    decay->sinPeriod = argReader.readDouble();
                    decay->sinPhase = argReader.readDouble();
                    decay->power = argReader.readDouble();
                    functions.emplace_back(std::move(decay));
                }
                else if (!strcmp(argReader.str(), "gauss")) {
                    std::unique_ptr<Gauss> gauss = std::make_unique<Gauss>();
                    gauss->amplitude = argReader.readDouble();
                    gauss->centre = argReader.readDouble();
                    gauss->width = argReader.readDouble();
                    functions.emplace_back(std::move(gauss));
                }
                else {
                    std::stringstream ss;
                    ss << "Unknown signal type: '" << argReader.str() << "'.";
                    throw U::ArgError(ss.str());
                }
                break;
            }
                
            case U::ARG_EOF:
                eof = true;
                break;
                
            default:
                throw std::runtime_error("Unknown argument type.");
            }
        }
        
        for (double t = startT; t < endT; t += stepT) {
            printf("%g", t);
            double arg = fmod(t, period);
            for (size_t i = 0, size = functions.size(); i < size; ++i) {                
                printf(",%g", functions[i]->call(arg));
            }
            printf("\n");
        }
        
        return 0;
    }
    catch (U::ArgNoError &) {        
        return 0;
    }
    catch (U::ArgError &e) {
        fprintf(stderr, "Error: %s\n%s", e.what(), USAGE);        
    }
    catch (std::runtime_error &e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    return -1;
}

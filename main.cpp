#include <iostream>
#include <cmath>
#include <string>
#include <map>
#include <vector>
#include "Pixel.h"
#include <sstream>
#include "Image.h"
#include "RGB.h"

bool isFloat(std::string myString ) {
    std::istringstream iss(myString);
    float f;
    iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail();
}

void checkArguments(int argc, char*argv[])
{
    if(argc == 3)
    {
        Image <RGB<uchar>> im;
        im.Read(argv[1]);
        im.Update();
        im.changeLightHSL(atof(argv[2]));
        im.Update();
    }
    //Minima cantidad de argumentos
    else if(argc == 6)
    {
        std::vector<float> params(3,{});
        for(int i=1 ; i<4; i++)
            params[i-1] = atof(argv[i]);
        transformSpaces(argv[4] , argv[5] , params);
    }
    //Verificacion si es que el cuarto argumento es un flotante para CMYK
    else if(argc == 7)
    {
        std::string argumento4 = argv[4];
        std::string argumento5 = argv[5];
        if(isFloat(argv[4]) && argumento5=="CMYK")
        {
            std::vector<float> params(4,{});
            for(int i=1 ; i<5 ; i++)
                params[i-1] = atof(argv[i]);
            transformSpaces(argv[5] , argv[6] , params);
        }
        else
        {
            std::cout<<"Entradas erroneas"<<std::endl;
        }
    }
    else
    {
        std::cout<<"Numero incorrecto de entradas"<<std::endl;
    }
}


int main(int argc, char *argv[])
{
    checkArguments(argc, argv);
    return 0;
}

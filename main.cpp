#include <iostream>
#include <cmath>
#include <string>
#include <map>
#include <vector>
#include "Pixel.h"

#define sRGB 0
#define HSV 1
#define HSL 2
#define CMYK 3
#define XYZ 4
#define CIELabD65 5


void transformSpaces(std::string fromSpace, std::string toSpace, std::vector<float> params)
{
    std::map<std::string, int> colorSpaces={{"sRGB", sRGB},{"HSV", HSV},{"HSL", HSL},{"CMYK", CMYK},{"XYZ", XYZ},{"CIELab(D65)", CIELabD65}};
    int fSpace = colorSpaces.at(fromSpace);
    int tSpace = colorSpaces.at(toSpace);
    if(fSpace == -1 || tSpace == -1)
    {
        std::cout<<"Espacios de color no validos"<<std::endl;
        return;
    }
    std::vector<float>result;
    switch (fSpace)
    {
        case sRGB:
            switch (tSpace)
            {
                case HSV:
                    result = RGBtoHSV(params);
                break;
                case HSL:
                    result = RGBtoHSL(params);
                break;
                case CMYK:
                    result = RGBtoCMYK(params);
                break;
                case XYZ:
                    result = RGBtoXYZ(params);
                break;
                case CIELabD65:
                    result = RGBtoCIELab(params);
                break;
            }
        break;

        case HSV:
        result = HSVtoRGB(params);
        switch (tSpace)
            {
                case sRGB:
                    result = HSVtoRGB(params);
                break;
                case HSL:
                    result = RGBtoHSL(result);
                break;
                case CMYK:
                    result = RGBtoCMYK(result);
                break;
                case XYZ:
                    result = RGBtoXYZ(result);
                break;
                case CIELabD65:
                    result = RGBtoCIELab(result);
                break;
            }
        break;

        case HSL:
        result = HSLtoRGB(params);
        switch (tSpace)
        {
            case sRGB:
                result = HSLtoRGB(params);
            break;
            case HSV:
                result = RGBtoHSV(result);
            break;
            case CMYK:
                result = RGBtoCMYK(result);
            break;
            case XYZ:
                result = RGBtoXYZ(result);
            break;
            case CIELabD65:
                result = RGBtoCIELab(result);
            break;
        }
        break;

        case CMYK:
        result = CMYKtoRGB(params);
        switch (tSpace)
        {
            case sRGB:
                result = CMYKtoRGB(params);
            break;
            case HSV:
                result = RGBtoCMYK(result);
            break;
            case HSL:
                result = RGBtoHSL(result);
            break;
            case XYZ:
                result = RGBtoXYZ(result);
            break;
            case CIELabD65:
                result = RGBtoCIELab(result);
            break;
        }
        break;

        case XYZ:
        result = XYZtoRGB(params);
        switch (tSpace)
        {
            case sRGB:
                result = XYZtoRGB(params);
            break;
            case HSV:
                result = RGBtoHSV(result);
            break;
            case HSL:
                result = RGBtoHSL(result);
            break;
            case CMYK:
                result = RGBtoCMYK(result);
            break;
            case CIELabD65:
                result = RGBtoCIELab(result);
            break;
        }
        break;

        case CIELabD65:
        result = CIELabtoRGB(params);
        switch (tSpace)
        {
            case sRGB:
                result = CIELabtoRGB(params);
            break;
            case HSV:
                result = RGBtoHSV(result);
            break;
            case HSL:
                result = RGBtoHSL(result);
            break;
            case CMYK:
                result = RGBtoCMYK(result);
            break;
            case XYZ:
                result = RGBtoXYZ(result);
            break;
        }
        break;
    }
}

int main(int argc, char *argv[])
{
    if(argc >= 6)
    {
        std::vector<float> params(argc-3,{});
        for(int i=3 ; i<argc ; i++)
            params[i-3] = atof(argv[i]);
        transformSpaces(argv[1] , argv[2] , params);
    }
    else
    {
        std::cout<<"Cantidad invalida de argumentos"<<std::endl;
        std::cout<<"Use el siguiente formato: <EspacioDeColorOrigen> <EspacioDeColorDestino> <Parametro1> <Parametro2>..."<<std::endl;
    }
    return 0;
}

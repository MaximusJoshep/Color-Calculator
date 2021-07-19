#ifndef PIXEL_H
#define PIXEL_H

#include <vector>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <map>

#define sRGB 0
#define HSV 1
#define HSL 2
#define CMYK 3
#define XYZ 4
#define CIELabD65 5

std::vector<float> RGBtoHSV(std::vector<float> input);
std::vector<float> RGBtoHSL(std::vector<float> input);
std::vector<float> RGBtoCMYK(std::vector<float> input);
std::vector<float> RGBtoXYZ(std::vector<float> input);
std::vector<float> RGBtoCIELab(std::vector<float> input);
std::vector<float> XYZtoRGB(std::vector<float> input);
std::vector<float> HSVtoRGB(std::vector<float> input);
std::vector<float> HSLtoRGB(std::vector<float> input);
std::vector<float> CMYKtoRGB(std::vector<float> input);
std::vector<float> CIELabtoRGB(std::vector<float> input);
std::vector<float> XYZtoCIELab(std::vector<float> input);
std::vector<float> CIELabtoXYZ(std::vector<float> input);

std::vector<float> RGBtoHSV(std::vector<float> input)
{

    std::vector<float> output(3);
    float R_p = input[0] / 255;
    float G_p = input[1] / 255;
    float B_p = input[2] / 255;
    float C_max = std::max(R_p, std::max(G_p, B_p));
    float C_min = std::min(R_p, std::min(G_p, B_p));
    float Delta = C_max - C_min;
    float H, S, V;
    //Calculamos V
    V = C_max;
    //Calculamos S
    if (C_max == 0)
    {
        S = 0;
    }
    else
    {
        S = Delta / C_max;
    }
    //Calculamos H
    if (Delta == 0)
    {
        H = 0;
    }
    else
    {
        if (C_max == R_p)
        {

            H = 60 * (G_p - B_p) / Delta;
        }
        else if (C_max == G_p)
        {

            H = 60 * (((B_p - R_p) / Delta) + 2);
        }
        else
        {
            H = 60 * (((R_p - G_p) / Delta) + 4);
        }


        if (H < 0)
            H += 360;
    }
    output[0] = H;
    output[1] = S * 100;
    output[2] = V * 100;
    return output;
}
std::vector<float> RGBtoHSL(std::vector<float> input)
{

    std::vector<float> output(3);
    float R_p = input[0] / 255;
    float G_p = input[1] / 255;
    float B_p = input[2] / 255;
    float C_max = std::max(R_p, std::max(G_p, B_p));
    float C_min = std::min(R_p, std::min(G_p, B_p));
    float Delta = C_max - C_min;
    //Calculamos L
    float H, S, L;
    L = ((C_max + C_min) / 2);
    //Calculamos S
    if (Delta == 0)
    {
        S = 0;
    }
    else
    {
        S = Delta / (1 - std::abs((2 * L) - 1));
    }
    //Calculamos H
    if (Delta == 0)
    {
        H = 0;
    }
    else
    {
        if (C_max == R_p)
        {

            H = 60 * ((G_p - B_p) / Delta);
        }
        else if (C_max == G_p)
        {

            H = 60 * (((B_p - R_p) / Delta) + 2);
        }
        else
        {
            H = 60 * (((R_p - G_p) / Delta) + 4);
        }

        if (H < 0)
            H += 360;

    }
    output[0] = H;
    output[1] = S * 100;
    output[2] = L * 100;
    return output;
}
std::vector<float> RGBtoCMYK(std::vector<float> input)
{

    std::vector<float> output(4);
    float R_p = input[0] / 255;
    float G_p = input[1] / 255;
    float B_p = input[2] / 255;
    float K = 1 - std::max(R_p, std::max(G_p, B_p));
    float C = (1 - R_p - K) / (1 - K);
    float M = (1 - G_p - K) / (1 - K);
    float Y = (1 - B_p - K) / (1 - K);
    output[0] = C * 100;
    output[1] = M * 100;
    output[2] = Y * 100;
    output[3] = K * 100;

    return output;
}
std::vector<float> RGBtoXYZ(std::vector<float> input)
{

    std::vector<float> output(3);
    float R_p = input[0] / 255;
    float G_p = input[1] / 255;
    float B_p = input[2] / 255;

    if (R_p > 0.04045f)
        R_p = powf(((R_p + 0.055f) / 1.055f), 2.4f);
    else
        R_p = R_p / 12.92f;

    if (G_p > 0.04045)
        G_p = powf(((G_p + 0.055f) / 1.055f), 2.4f);
    else
        G_p = G_p / 12.92f;

    if (B_p > 0.04045f)
        B_p = powf(((B_p + 0.055f) / 1.055f), 2.4f);
    else
        B_p = B_p / 12.92f;

    R_p = R_p * 100;
    G_p = G_p * 100;
    B_p = B_p * 100;


    output[0] = R_p * 0.4124f + G_p * 0.3576f + B_p * 0.1805f;
    output[1] = R_p * 0.2126f + G_p * 0.7152f + B_p * 0.0722f;
    output[2] = R_p * 0.0193f + G_p * 0.1192f + B_p * 0.9505f;

    return output;
}
std::vector<float> RGBtoCIELab(std::vector<float> input)
{

    std::vector<float> output(3);
    output = XYZtoCIELab(RGBtoXYZ(input));
    return output;
}
std::vector<float> XYZtoRGB(std::vector<float> input)
{

    std::vector<float> output(3);
    float X_p = input[0] / 100;
    float Y_p = input[1] / 100;
    float Z_p = input[2] / 100;

    float R = X_p * 3.2406 + (Y_p * -1.5372) + Z_p * (-0.4986);
    float G = X_p * (-0.9689) + Y_p * 1.8758 + Z_p * 0.0415;
    float B = X_p * 0.0557 + Y_p * (-0.2040) + Z_p * 1.0570;

    if (R > 0.0031308)
        R = 1.055 * powf(R, (1.0f / 2.4)) - 0.055;
    else
        R = 12.92 * R;

    if (G > 0.0031308)
        G = 1.055 * powf(G, (1.0f / 2.4)) - 0.055;
    else
        G = 12.92 * G;

    if (B > 0.0031308)
        B = 1.055 * powf(B, (1.0f / 2.4)) - 0.055;
    else
        B = 12.92 * B;

    output[0] = (int)(R * 255);
    output[1] = (int)(G * 255);
    output[2] = (int)(B * 255);

    return output;
}
std::vector<float> HSVtoRGB(std::vector<float> input)
{

    std::vector<float> output(3);
    float H = input[0];
    float S = input[1];
    float V = input[2];
    float s = S / 100;
    float v = V / 100;
    float C = s * v;
    float X = C * (1 - std::abs(std::fmod(H / 60.0, 2) - 1));
    float m = v - C;
    float r, g, b;
    if (H >= 0 && H < 60) {
        r = C, g = X, b = 0;
    }
    else if (H >= 60 && H < 120) {
        r = X, g = C, b = 0;
    }
    else if (H >= 120 && H < 180) {
        r = 0, g = C, b = X;
    }
    else if (H >= 180 && H < 240) {
        r = 0, g = X, b = C;
    }
    else if (H >= 240 && H < 300) {
        r = X, g = 0, b = C;
    }
    else {
        r = C, g = 0, b = X;
    }
    int R = (r + m) * 255;
    int G = (g + m) * 255;
    int B = (b + m) * 255;
    output[0] = R;
    output[1] = G;
    output[2] = B;
    return output;
}
std::vector<float> HSLtoRGB(std::vector<float> input)
{

    std::vector<float> output(3);
    float H = input[0];
    float S = input[1] / 100;
    float L = input[2] / 100;

    float C = S * (1 - std::abs(2 * L - 1));
    float X = C * (1 - std::abs(std::fmod(H / 60.0, 2) - 1));
    float m = L - C / 2;
    float r, g, b;
    if (H >= 0 && H < 60) {
        r = C, g = X, b = 0;
    }
    else if (H >= 60 && H < 120) {
        r = X, g = C, b = 0;
    }
    else if (H >= 120 && H < 180) {
        r = 0, g = C, b = X;
    }
    else if (H >= 180 && H < 240) {
        r = 0, g = X, b = C;
    }
    else if (H >= 240 && H < 300) {
        r = X, g = 0, b = C;
    }
    else {
        r = C, g = 0, b = X;
    }
    int R = (r + m) * 255;
    int G = (g + m) * 255;
    int B = (b + m) * 255;
    output[0] = R;
    output[1] = G;
    output[2] = B;
    return output;
}
std::vector<float> CMYKtoRGB(std::vector<float> input)
{
    std::vector<float> output(3);
    float C = input[0] / 100;
    float M = input[1] / 100;
    float Y = input[2] / 100;
    float K = input[3] / 100;

    int R = round(255 * (1 - C) * (1 - K));
    int G = round(255 * (1 - M) * (1 - K));
    int B = round(255 * (1 - Y) * (1 - K));

    output[0] = R;
    output[1] = G;
    output[2] = B;
    return output;
}
std::vector<float> CIELabtoRGB(std::vector<float> input)
{

    std::vector<float> output(3);
    output = XYZtoRGB(CIELabtoXYZ(input));
    return output;
}
std::vector<float> XYZtoCIELab(std::vector<float> input)
{
    float ref_X = 95.047;
    float ref_Y = 100.0;
    float ref_Z = 108.883;
    std::vector<float> output(3);
    float X = (input[0] / ref_X);
    float Y = (input[1] / ref_Y);
    float Z = (input[2] / ref_Z);

    if (X > 0.008856)
        X = powf(X, (1.0f / 3));
    else
        X = (7.787 * X) + (16.0f / 116);

    if (Y > 0.008856)
        Y = powf(Y, (1.0f / 3));
    else
        Y = (7.787 * Y) + (16.0f / 116);

    if (Z > 0.008856)
        Z = powf(Z, (1.0f / 3));
    else
        Z = (7.787 * Z) + (16.0f / 116);

    output[0] = (116 * Y) - 16;
    output[1] = 500 * (X - Y);
    output[2] = 200 * (Y - Z);


    return output;
}
std::vector<float> CIELabtoXYZ(std::vector<float> input)
{
    float ref_X = 95.047;
    float ref_Y = 100.0;
    float ref_Z = 108.883;
    std::vector<float> output(3);
    float Y = (input[0] + 16) / 116;
    float X = (input[1] / 500) + Y;
    float Z = Y - (input[2] / 200);

    if (powf(Y, 3.f) > 0.008856)
        Y = powf(Y, 3.f);
    else
        Y = (Y - (16 / 116)) / 7.787;

    if (powf(X, 3.f) > 0.008856)
        X = powf(X, 3.f);
    else
        X = (X - (16 / 116)) / 7.787;

    if (powf(Z, 3.f) > 0.008856)
        Z = powf(Z, 3.f);
    else
        Z = (Z - (16 / 116)) / 7.787;

    output[0] = ref_X * X;
    output[1] = ref_Y * Y;
    output[2] = ref_Z * Z;

    return output;
}

void transformSpaces(std::string fromSpace, std::string toSpace, std::vector<float> params)
{
    std::map<std::string, int> colorSpaces={{"sRGB", sRGB},{"HSV", HSV},{"HSL", HSL},{"CMYK", CMYK},{"XYZ", XYZ},{"CIELab (D65)", CIELabD65}};
    int fSpace = colorSpaces.at(fromSpace);
    int tSpace = colorSpaces.at(toSpace);
    if(fSpace == CMYK && params.size() !=4)
    {
        std::cout<<"Numero incorrecto de entradas"<<std::endl;
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
                result = XYZtoCIELab(result);
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
                result = CIELabtoXYZ(result);
            break;
        }
        break;
    }
    for(std::size_t i=0 ; i<result.size() ; i++)
    {
        std::cout<<result[i]<<" ";
    }
    std::cout<<std::endl;
}

#endif // PIXEL_H

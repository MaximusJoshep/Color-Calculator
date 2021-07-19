#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <cmath>
#include "Pixel.h"

template<typename PixelType>
class Image
{
    using DataType = std::vector<std::vector<PixelType>>;

public:
    Image(){};
    void Read(const std::string& fileName);
    void Update();
    void transformSpace(std::string toSpace);
    std::vector<std::vector<PixelType>> RGB_HSV(DataType input, int rows, int cols);
    std::vector<std::vector<PixelType>> RGB_HSL(DataType input, int rows, int cols);
    std::vector<std::vector<PixelType>> RGB_CMYK(DataType input, int rows, int cols);
    std::vector<std::vector<PixelType>> RGB_XYZ(DataType input, int rows, int cols);
    std::vector<std::vector<PixelType>> RGB_CIELab(DataType input, int rows, int cols);

    std::vector<std::vector<PixelType>> HSV_RGB(DataType input, int rows, int cols);
    std::vector<std::vector<PixelType>> HSL_RGB(DataType input, int rows, int cols);
    std::vector<std::vector<PixelType>> CMYK_RGB(DataType input, int rows, int cols);
    std::vector<std::vector<PixelType>> XYZ_RGB(DataType input, int rows, int cols);
    std::vector<std::vector<PixelType>> CIELab_RGB(DataType input, int rows, int cols);

    void changeLightHSL(float lightPercent);
    void changeLightCIELab(float lightPercent);

private:
    DataType data;
    std::size_t rows{ 0 };
    std::size_t columns{ 0 };
    int space;
};

template<typename PixelType>
void Image<PixelType>::Update()
{
    cv::Mat image = cv::Mat::zeros(rows, columns, CV_8UC3);
    for(unsigned r=0 ; r<rows ; ++r)
    {
        for(unsigned c=0 ; c<columns ; ++c)
        {
            image.at<cv::Vec3b>(r,c)[0] = data[r][c][0];
            image.at<cv::Vec3b>(r,c)[1] = data[r][c][1];
            image.at<cv::Vec3b>(r,c)[2] = data[r][c][2];
        }
    }
    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Image", image);
    cv::waitKey();
}
template<typename  PixelType>
void Image<PixelType>::Read(const std::string& fileName)
{
    space = sRGB;
    cv::Mat image = cv::imread(fileName, cv::IMREAD_COLOR);
    if (!image.data)
    {
        std::cerr<<"No image data\n";
        return;
    }
    rows = image.rows;
    columns = image.cols;

    data = DataType(rows, std::vector<PixelType>(columns, PixelType{}));

    PixelType pixel;
    for (unsigned r=0; r < rows; ++r)
    {
        cv::Vec3b * row = image.ptr<cv::Vec3b>(r);
        for (unsigned c=0; c < columns; ++c)
        {
            pixel[2] = row[c][2];  //R
            pixel[1] = row[c][1];  //G
            pixel[0] = row[c][0];  //B

            if constexpr (std::is_fundamental<PixelType>::value) //uchar, short, float (gray)
            {
                data[c][r] = static_cast<PixelType>((pixel[0] + pixel[1]+ pixel[2])/3);
            }
            else //RGB LAB, channels...
            {
                //memoria
                data[r][c][0] = static_cast<typename PixelType::ValueType>(pixel[0]);
                data[r][c][1] = static_cast<typename PixelType::ValueType>(pixel[1]);
                data[r][c][2] = static_cast<typename PixelType::ValueType>(pixel[2]);
            }
        }
    }
}

template<typename PixelType>
std::vector<std::vector<PixelType>> Image<PixelType>::RGB_HSV(DataType input, int rows, int cols)
{
    DataType output = DataType(rows, std::vector<PixelType>(cols, PixelType{}));
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            std::vector<float> rgb(3);
            rgb[0] = input[r][c][2];
            rgb[1] = input[r][c][1];
            rgb[2] = input[r][c][0];
            //Respetaremos el mismo orden con el que nuestra estructura maneja rgb
            std::vector<float> hsv = RGBtoHSV(rgb);
            output[r][c][2] = static_cast<typename PixelType::ValueType>(hsv[0]);
            output[r][c][1] = static_cast<typename PixelType::ValueType>(hsv[1]);
            output[r][c][0] = static_cast<typename PixelType::ValueType>(hsv[2]);
        }
    }
    return output;
}

template<typename PixelType>
std::vector<std::vector<PixelType>> Image<PixelType>::RGB_HSL(DataType input, int rows, int cols)
{
    DataType output = DataType(rows, std::vector<PixelType>(cols, PixelType{}));
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            std::vector<float> rgb(3);
            rgb[0] = input[r][c][2];
            rgb[1] = input[r][c][1];
            rgb[2] = input[r][c][0];
            //Respetaremos el mismo orden con el que nuestra estructura maneja rgb
            std::vector<float> hsl = RGBtoHSL(rgb);
            output[r][c][2] = static_cast<typename PixelType::ValueType>(hsl[0]);
            output[r][c][1] = static_cast<typename PixelType::ValueType>(hsl[1]);
            output[r][c][0] = static_cast<typename PixelType::ValueType>(hsl[2]);
        }
    }
    return output;
}

template<typename PixelType>
 std::vector<std::vector<PixelType>> Image<PixelType>::RGB_CMYK(DataType input, int rows, int cols)
{
     DataType output = DataType(rows, std::vector<PixelType>(cols, PixelType{}));
     for (int r = 0; r < rows; r++)
     {
         for (int c = 0; c < cols; c++)
         {
             std::vector<float> rgb(3);
             rgb[0] = input[r][c][2];
             rgb[1] = input[r][c][1];
             rgb[2] = input[r][c][0];
             //Respetaremos el mismo orden con el que nuestra estructura maneja rgb
             std::vector<float> cmyk = RGBtoCMYK(rgb);
             output[r][c][3] = static_cast<typename PixelType::ValueType>(cmyk[0]);
             output[r][c][2] = static_cast<typename PixelType::ValueType>(cmyk[1]);
             output[r][c][1] = static_cast<typename PixelType::ValueType>(cmyk[2]);
             output[r][c][0] = static_cast<typename PixelType::ValueType>(cmyk[3]);
         }
     }
     return output;
}

template<typename PixelType>
std::vector<std::vector<PixelType>> Image<PixelType>::RGB_XYZ(DataType input, int rows, int cols)
{
    DataType output = DataType(rows, std::vector<PixelType>(cols, PixelType{}));
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            std::vector<float> rgb(3);
            rgb[0] = input[r][c][2];
            rgb[1] = input[r][c][1];
            rgb[2] = input[r][c][0];
            //Respetaremos el mismo orden con el que nuestra estructura maneja rgb
            std::vector<float> xyz = RGBtoXYZ(rgb);
            output[r][c][2] = static_cast<typename PixelType::ValueType>(xyz[0]);
            output[r][c][1] = static_cast<typename PixelType::ValueType>(xyz[1]);
            output[r][c][0] = static_cast<typename PixelType::ValueType>(xyz[2]);
        }
    }
    return output;
}

template<typename PixelType>
 std::vector<std::vector<PixelType>> Image<PixelType>::RGB_CIELab(DataType input, int rows, int cols)
{
    DataType output = DataType(rows, std::vector<PixelType>(cols, PixelType{}));
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            std::vector<float> rgb(3);
            rgb[0] = input[r][c][2];
            rgb[1] = input[r][c][1];
            rgb[2] = input[r][c][0];
            //Respetaremos el mismo orden con el que nuestra estructura maneja rgb
            std::vector<float> cie = RGBtoCIELab(rgb);
            output[r][c][2] = static_cast<typename PixelType::ValueType>(cie[0]);
            output[r][c][1] = static_cast<typename PixelType::ValueType>(cie[1]);
            output[r][c][0] = static_cast<typename PixelType::ValueType>(cie[2]);
        }
    }
    return output;
}

template<typename PixelType>
std::vector<std::vector<PixelType>> Image<PixelType>::HSV_RGB(DataType input, int rows, int cols)
{
    DataType output = DataType(rows, std::vector<PixelType>(cols, PixelType{}));
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            std::vector<float> hsv(3);
            hsv[0] = input[r][c][2];
            hsv[1] = input[r][c][1];
            hsv[2] = input[r][c][0];
            //Respetaremos el mismo orden con el que nuestra estructura maneja rgb
            std::vector<float> rgb = HSVtoRGB(hsv);
            output[r][c][2] = static_cast<typename PixelType::ValueType>(rgb[0]);
            output[r][c][1] = static_cast<typename PixelType::ValueType>(rgb[1]);
            output[r][c][0] = static_cast<typename PixelType::ValueType>(rgb[2]);
        }
    }
    return output;
}

template<typename PixelType>
inline std::vector<std::vector<PixelType>> Image<PixelType>::HSL_RGB(DataType input, int rows, int cols)
{
    DataType output = DataType(rows, std::vector<PixelType>(cols, PixelType{}));
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            std::vector<float> hsl(3);
            hsl[0] = input[r][c][2];
            hsl[1] = input[r][c][1];
            hsl[2] = input[r][c][0];
            //Respetaremos el mismo orden con el que nuestra estructura maneja rgb
            std::vector<float> rgb = HSLtoRGB(hsl);
            output[r][c][2] = static_cast<typename PixelType::ValueType>(rgb[0]);
            output[r][c][1] = static_cast<typename PixelType::ValueType>(rgb[1]);
            output[r][c][0] = static_cast<typename PixelType::ValueType>(rgb[2]);
        }
    }
    return output;
}

template<typename PixelType>
inline std::vector<std::vector<PixelType>> Image<PixelType>::CMYK_RGB(DataType input, int rows, int cols)
{
    DataType output = DataType(rows, std::vector<PixelType>(cols, PixelType{}));
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            std::vector<float> cmyk(4);
            cmyk[0] = input[r][c][3];
            cmyk[1] = input[r][c][2];
            cmyk[2] = input[r][c][1];
            cmyk[3] = input[r][c][0];
            //Respetaremos el mismo orden con el que nuestra estructura maneja rgb
            std::vector<float> rgb = CMYKtoRGB(cmyk);
            output[r][c][2] = static_cast<typename PixelType::ValueType>(rgb[0]);
            output[r][c][1] = static_cast<typename PixelType::ValueType>(rgb[1]);
            output[r][c][0] = static_cast<typename PixelType::ValueType>(rgb[2]);
        }
    }
    return output;
}

template<typename PixelType>
inline std::vector<std::vector<PixelType>> Image<PixelType>::XYZ_RGB(DataType input, int rows, int cols)
{
    DataType output = DataType(rows, std::vector<PixelType>(cols, PixelType{}));
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            std::vector<float> xyz(3);
            xyz[0] = input[r][c][2];
            xyz[1] = input[r][c][1];
            xyz[2] = input[r][c][0];
            //Respetaremos el mismo orden con el que nuestra estructura maneja rgb
            std::vector<float> rgb = XYZtoRGB(xyz);
            output[r][c][2] = static_cast<typename PixelType::ValueType>(rgb[0]);
            output[r][c][1] = static_cast<typename PixelType::ValueType>(rgb[1]);
            output[r][c][0] = static_cast<typename PixelType::ValueType>(rgb[2]);
        }
    }
    return output;
}

template<typename PixelType>
inline std::vector<std::vector<PixelType>> Image<PixelType>::CIELab_RGB(DataType input, int rows, int cols)
{
    DataType output = DataType(rows, std::vector<PixelType>(cols, PixelType{}));
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            std::vector<float> cie(3);
            cie[0] = input[r][c][2];
            cie[1] = input[r][c][1];
            cie[2] = input[r][c][0];
            //Respetaremos el mismo orden con el que nuestra estructura maneja rgb
            std::vector<float> rgb = CIELabtoRGB(cie);
            output[r][c][2] = static_cast<typename PixelType::ValueType>(rgb[0]);
            output[r][c][1] = static_cast<typename PixelType::ValueType>(rgb[1]);
            output[r][c][0] = static_cast<typename PixelType::ValueType>(rgb[2]);
        }
    }
    return output;
}

template <typename PixelType>
void Image<PixelType>::changeLightHSL(float lightPercent)
{
    for(int r=0 ; r<rows ; r++)
    {
        for(int c=0 ; c<columns ; c++)
        {
            std::vector<float> rgb(3);
            rgb[0] = data[r][c][2];
            rgb[1] = data[r][c][1];
            rgb[2] = data[r][c][0];
            //Respetaremos el mismo orden con el que nuestra estructura maneja rgb
            std::vector<float> hsl = RGBtoHSL(rgb);
            data[r][c][2] = static_cast<typename PixelType::ValueType>(hsl[0]);
            data[r][c][1] = static_cast<typename PixelType::ValueType>(hsl[1]);
            data[r][c][0] = static_cast<typename PixelType::ValueType>(hsl[2])*lightPercent/100;
            //Actualizamos los datos a rgb
            rgb[0] = data[r][c][2];
            rgb[1] = data[r][c][1];
            rgb[2] = data[r][c][0];
            hsl = HSLtoRGB(rgb);
            data[r][c][2] = static_cast<typename PixelType::ValueType>(hsl[0]);
            data[r][c][1] = static_cast<typename PixelType::ValueType>(hsl[1]);
            data[r][c][0] = static_cast<typename PixelType::ValueType>(hsl[2]);

        }
    }
}

template <typename PixelType>
void Image<PixelType>::changeLightCIELab(float lightPercent)
{
    for(int r=0 ; r<rows ; r++)
    {
        for(int c=0 ; c<columns ; c++)
        {
            std::vector<float> rgb(3);
            rgb[0] = data[r][c][2];
            rgb[1] = data[r][c][1];
            rgb[2] = data[r][c][0];
            //Respetaremos el mismo orden con el que nuestra estructura maneja rgb
            std::vector<float> hsl = RGBtoCIELab(rgb);
            data[r][c][2] = static_cast<typename PixelType::ValueType>(hsl[0]);
            data[r][c][1] = static_cast<typename PixelType::ValueType>(hsl[1]);
            data[r][c][0] = static_cast<typename PixelType::ValueType>(hsl[2]);
            //Actualizamos los datos a rgb
            rgb[0] = data[r][c][2];
            rgb[1] = data[r][c][1];
            rgb[2] = data[r][c][0];
            hsl = CIELabtoRGB(rgb);
            data[r][c][2] = static_cast<typename PixelType::ValueType>(hsl[0]);
            data[r][c][1] = static_cast<typename PixelType::ValueType>(hsl[1]);
            data[r][c][0] = static_cast<typename PixelType::ValueType>(hsl[2]);

        }
    }
}

#endif // IMAGE_H

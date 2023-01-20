#include<iostream>
#include<fstream>
#include "RGBValue.h"
#include "PPMImage.h"
#include "PPMLine.h"
#include <vector>
#include <string>

int main() {



    // Test constructors
    PPMImage img0;
    PPMImage img1("pikachu.ppm");
    PPMImage img2 = img1;
    std::cout << "---Constructors Tested---" << std::endl;
    // Test first class operations
    std::vector<RGBValue> lineInput;
    RGBValue purple, green;
    purple.red = 128;
    purple.blue = 128;
    green.green = 255;
    for (unsigned short i = 0; i < 676; i++) {
        lineInput.push_back(purple);
    }
    PPMLine myline(lineInput, 255);

    std::cout << "---Purple Line Created---" << std::endl;

    std::vector<RGBValue> lineInput2;
    for (unsigned short i = 0; i < 676; i++) {
        lineInput2.push_back(green);
    }
    PPMLine myline2(lineInput2, 255);

    std::cout << "---Green Line Created---" << std::endl;
    PPMImage img5("pikachu.ppm");
    for (unsigned short i = 0; i < 50; i++) {
        img5 += myline2;
    }

    std::cout << "---Line Addition Tested---" << std::endl;

    // tests + of PPMLine
    PPMImage img3 = img1 + myline + myline + myline + myline + myline + myline;
    // tests + of PPMImage
    PPMImage img4 = img1 + img1;
    // tests - of PPMImage
    PPMImage img6 = img4 - img1;
    // tests inversion of an image
    PPMImage img7 = -img5;
    // tests output to stdout
//        std::cout << img7 << std::endl;

    if (img6 == img6) {
        std::cout << "CORRECT" << std::endl;
    }
    if (img6 == img4) {
        std::cout << "ERROR" << std::endl;
    }
    if (img6 != img6) {
        std::cout << "ERROR" << std::endl;
    }
    if (img6 != img4) {
        std::cout << "CORRECT" << std::endl;
    }

    // tests LoadFile
    PPMImage img8;
    img8.LoadFile("blank.ppm");
    //tests DrawLine
    img8.DrawLine(200, 0, 400, 99, purple);
    img8.DrawLine(0, 0, 799, 799, green);
    img8.DrawLine(799, 0, 0, 400, purple);


    // tests ouput to a file

    std::vector<PPMImage> imagesToOutput;
    imagesToOutput.push_back(img0);
    imagesToOutput.push_back(img1);
    imagesToOutput.push_back(img2);
    imagesToOutput.push_back(img3);
    imagesToOutput.push_back(img4);
    imagesToOutput.push_back(img5);
    imagesToOutput.push_back(img6);
    imagesToOutput.push_back(img7);
    imagesToOutput.push_back(img8);


    for (unsigned short i = 0; i < imagesToOutput.size(); i++) {
        std::string filename = "img" + std::to_string(i) + ".ppm";   // gcc does not currently support std::format :(
        std::ofstream outFile(filename.c_str(), std::ios_base::out | std::ios_base::binary);
        outFile << imagesToOutput[i];
        outFile.close();
    }


    //test Exceptions

    return 0;
}

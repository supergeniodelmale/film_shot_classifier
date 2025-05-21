//
//  openCV_lib.cpp
//  openCV_lib
//
//  Created by Marek Tatýrek on 11.03.2025.
//

#include <iostream>
#include "openCV_lib.hpp"
#include "openCV_libPriv.hpp"

void openCV_lib::HelloWorld(const char * s)
{
    openCV_libPriv *theObj = new openCV_libPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void openCV_libPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};


/* Copyright 2017 Eric Quackenbush */

#define _GLIBCXX_USE_CXX11_ABI 0
#include <iostream>
#include <image_object.h>

int main(int argc, char *argv[])
{
	ImageObject new_image;
	new_image.load_file(argv[1]);
	new_image.get_image_type();
	new_image.fully_automatic();
	new_image.display_image(500, 500);

    return 0;
}
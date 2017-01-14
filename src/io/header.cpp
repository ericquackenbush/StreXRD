#include <stdio.h>
#include <algorithm>
#include <ctype.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <boost/algorithm/string.hpp>

using namespace std;

// header variables
int marker;
int size;
int num_high_intensity_pixels;
int collection_mode;
int total_pixels;
int pixel_length;
int pixel_height;
int wavelength;
int dist;
int phi_start;
int phi_end;
int omega_start;
int omega_end;
int chi;
int two_theta;
int format;
char *filename;

// An unsigned char can store 1 Bytes (8bits) of data (0-255)
typedef unsigned char BYTE;

// Get the size of a file
long getFileSize(FILE *file)
{
    long lCurPos, lEndPos;
    lCurPos = ftell(file);
    fseek(file, 0, 2);
    lEndPos = ftell(file);
    fseek(file, lCurPos, 0);
    return lEndPos;
}

int byteToInt(BYTE b1, BYTE b2, BYTE b3, BYTE b4)
{
    int a;
    a = b1 + b2*256 + b3*256*256 + b4*256*256*256;
    return a;
}

void getHeader(BYTE header[4096])
{
    marker = byteToInt(header[0], header[1], header[2], header[3]);
    size = byteToInt(header[4], header[5], header[6], header[7]);
    num_high_intensity_pixels = byteToInt(header[8], header[9], header[10], header[11]);
    format = byteToInt(header[12], header[13], header[14], header[15]);
    collection_mode = byteToInt(header[16], header[17], header[18], header[19]);
    total_pixels = byteToInt(header[20], header[21], header[22], header[23]);
    pixel_length = byteToInt(header[24], header[25], header[26], header[27]);
    pixel_height = byteToInt(header[28], header[29], header[30], header[31]);
    wavelength  = byteToInt(header[32], header[33], header[34], header[35]);
    dist = byteToInt(header[36], header[37], header[38], header[39]);
    phi_start = byteToInt(header[40], header[41], header[42], header[43]);
    phi_end = byteToInt(header[44], header[45], header[46], header[47]);
    omega_start = byteToInt(header[48], header[49], header[50], header[51]);
    omega_end = byteToInt(header[52], header[53], header[54], header[55]);
    chi = byteToInt(header[56], header[57], header[58], header[59]);
    two_theta = byteToInt(header[60], header[61], header[62], header[63]);

    char text[3968];
    for (int i = 128; i < 4096; i++)    {
        text[i-128] = header[i];
    }

    vector<string> list;
    string str = " ";

    char chars[64];
    for (int i = 0; i < 3968; i+=64)    {
        for (int j = 0; j < 64; j++) {
            if (text[i+j] >= 32 && text[i+j] <= 126)  {
              chars[j] = text[i+j];
            }
            else  {
              chars[j] = '\0';
            }
        }
        string str(chars);
        if (str.find("END") == 0)
            break;
        list.push_back(str);
    }

    list.erase( list.begin() );

    // print header to text file
    string fn(filename);
    fn.append(".txt");
    char *fileName = (char*)fn.c_str();
    ofstream myfile (fileName);
    if (myfile.is_open())
    {
      for(int count = 0; count < list.size(); count ++){
        myfile << list[count] << endl;
      }
      myfile.close();
      cout << "File written successfully" << "\n";
    }
    else cout << "Unable to open file" << "\n";

}

int main(int argc, char* argv[])
{
  filename = argv[1];
  const char *filePath = argv[1];
  BYTE *fileBuf;			// Pointer to our buffered data
  FILE *file = NULL;		// File pointer
  BYTE header[4096];

  // Open the file in binary mode using the "rb" format string
  // This also checks if the file exists and/or can be opened for reading correctly
  if ((file = fopen(filePath, "rb")) == NULL)
      cout << "Could not open specified file" << endl;
  else
      cout << "File opened successfully" << endl;

  // Get the size of the file in bytes
  long fileSize = getFileSize(file);

  // Allocate space in the buffer for the whole file
  fileBuf = new BYTE[fileSize];

  // Read the file in to the buffer
  fread(fileBuf, fileSize, 1, file);

  for (int i = 0; i < 4096; i++)  {
      header[i] = fileBuf[i];
  }

  getHeader(header);

  // find high intensity pixels
  if (num_high_intensity_pixels > 0)  {
    cout << "High Intensity Pixels Found!" << "\n";
    int high_intensity_pixels [2][num_high_intensity_pixels];
    int count = 0;
    for (int i = 4096; i < (4096 + 8*num_high_intensity_pixels); i+=8) {
      // store locations
      high_intensity_pixels[0][count] = byteToInt(fileBuf[i], fileBuf[i+1], fileBuf[i+2], fileBuf[i+3]);
      // store intensity values
      high_intensity_pixels[1][count] = byteToInt(fileBuf[i+4], fileBuf[i+5], fileBuf[i+6], fileBuf[i+7]);
      count++;
    }
}

  delete[]fileBuf;
  fclose(file);   // Almost forgot this
  return 0;
}
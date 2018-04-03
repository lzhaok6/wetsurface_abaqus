// wetsurface.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <iostream>
#include <fstream>
#include <string>



//description:
//this code is used to translate the MAESTRO output .wet file. Some modification needs to be made in that file before inputing
//in this code. 
//The irrelevant information has to be deleted and the last string for some element has to be changed to be identical with 
//other elements. 
//The negative element No. you may say does not matter because that only means the node numbering is organized in order to 
//make the normal side consistent with wet element definition (pointing inward structure element) 
//If you have any doubt, a template called wetsurface_template.txt can be found in project folder. 
//the staring element number needs to be changed. 
int main()
{
	std::string str("element");
	std::ifstream theFile("FSP_metric6_surface_refine7.wet"); //FSP_metric6_surface_refine6.wet
	int i, j, a, b, c, d, e, k, l, m;
	double f, g, h;
	std::string str1;
	std::string str2;
	std::string str3, str4, str5, str6;
	int count = 0;
	const int MAXELEMENT = 10000;
	const int nnpe = 4; //number of node per element 
	int** ELE;
	double**coord;
	ELE = new int*[MAXELEMENT];
	coord = new double*[MAXELEMENT];
	for (i = 0; i < MAXELEMENT; i++) {
		ELE[i] = new int[nnpe + 3];
		coord[i] = new double[3];
	}
	for (i = 0; i < MAXELEMENT; i++) {
		for (j = 0; j < nnpe + 3; j++) {
			ELE[i][j] = 0;
		}
		for (j = 0; j < 3; j++) {
			coord[i][j] = 0.0;
		}
	}
	while (/*theFile >> str1 >> a >> b >> c >> d >> e >> f >> g >> h >> k >> l >> m >> str2 >> str3 >> str4 >> str5 >> str6*/
		theFile >> str1 >> a >> b >> c >> d >> e >> f >> g >> h >> k >> l >> m >> str2 >> str3 >> str4)
	{
		if (str.compare(str1) == 0) {
			if (a < 0) {
				a = -a;
			}
			std::cout << str1 << a << b << c << d << e << f << std::endl;
			//ELE[count][0] = a; //element numbering
			ELE[count][0] = b; //the first local node
			ELE[count][1] = c; //the second local node
			ELE[count][2] = d; //the third local node
			ELE[count][3] = e; //the fourth local node
			coord[count][0] = f; //x coordinate of element central point
			coord[count][1] = g; //y coordinate of element central point 
			coord[count][2] = h; //z coordinate of element central point
			count += 1;
		}
	}
	int element_num = count;
	std::ofstream myfile;
	myfile.open("wetsurface_formated.txt");
	int element_start = 2465;
	int element = element_start; //the starting numbering of wsf element
	for (i = 0; i < element_num; i++) {
		if (element >= 100 && element < 1000) { //if 3 digit number
			myfile << "WETELMG" << "      "<<element<< "   QUAD4" << "                                                +" << std::endl;
		}
		else if (element >= 1000) { //if 4 digit number
			myfile << "WETELMG" << "     " << element << "   QUAD4" << "                                                +" << std::endl;
		}
		myfile << "+       ";
		for (j = 0; j < nnpe; j++) {
			if (ELE[i][j]<10) {
				myfile << "       " << ELE[i][j];
			}
			else if (ELE[i][j] >= 10 && ELE[i][j] < 100) {
				myfile << "      " << ELE[i][j];
			}
			else if (ELE[i][j] >= 100 && ELE[i][j] < 1000) {
				myfile << "     " << ELE[i][j];
			}
		}
		myfile << std::endl;
		element += 1;
	}


	element = element_start;
	//back surface 
	double location1 = -2.4384;
	myfile << "WETSURF        1    back                                                +" << std::endl;
	//myfile << "+       ";
	count = 0;
	for (i = 0; i < element_num; i++) {
		if (abs(coord[i][2]-location1)<1e-6) {
			if ((i + element) >= 1e1 && (i + element) < 1e2) {
				if (count % 8 == 0) {
					myfile << "+       ";
				}
				myfile << "      " << i + element;
				count += 1;
			}
			if ((i + element) >= 1e2 && (i + element) < 1e3) {
				if (count % 8 == 0) {
					myfile << "+       ";
				}
				myfile << "     " << i + element;
				count += 1;
			}
			if ((i + element) >= 1e3 && (i + element) < 1e4) {
				if (count % 8 == 0) {
					myfile << "+       ";
				}
				myfile << "    " << i + element;
				count += 1;
			}
			if (count % 8 == 0) {
				myfile << "+" << std::endl;
			}
		}
	}
	myfile << std::endl;

	//left surface 
	double location2 = -4.2672;
	myfile << "WETSURF        2    left                                                +" << std::endl;
	count = 0;
	for (i = 0; i < element_num; i++) {
		if (abs(coord[i][0] - location2)<1e-6) {
			if ((i + element) >= 1e2 && (i + element) < 1e3) {
				if (count % 8 == 0) {
					myfile << "+       ";
				}
				myfile << "     " << i + element;
				count += 1;
			}
			if ((i + element) >= 1e3 && (i + element) < 1e4) {
				if (count % 8 == 0) {
					myfile << "+       ";
				}
				myfile << "    " << i + element;
				count += 1;
			}
			if (count % 8 == 0) {
				myfile << "+" << std::endl;
			}
		}
	}
	myfile << std::endl;

	//bottom surface
	double location3 = -0.9144;
	myfile << "WETSURF        3  bottom                                                +" << std::endl;
	count = 0;
	for (i = 0; i < element_num; i++) {
		if (abs(coord[i][1] - location3)<1e-6) {
			if ((i + element) >= 1e2 && (i + element) < 1e3) {
				if (count % 8 == 0) {
					myfile << "+       ";
				}
				myfile << "     " << i + element;
				count += 1;
			}
			if ((i + element) >= 1e3 && (i + element) < 1e4) {
				if (count % 8 == 0) {
					myfile << "+       ";
				}
				myfile << "    " << i + element;
				count += 1;
			}
			if (count % 8 == 0) {
				myfile << "+" << std::endl;
			}
		}
	}
	myfile << std::endl;

	//front surface
	double location4 = 2.4384;
	myfile << "WETSURF        4   front                                                +" << std::endl;
	count = 0;
	for (i = 0; i < element_num; i++) {
		if (abs(coord[i][2] - location4) < 1e-6) {
			if ((i + element) >= 1e2 && (i + element) < 1e3) {
				if (count % 8 == 0) {
					myfile << "+       ";
				}
				myfile << "     " << i + element;
				count += 1;
			}
			if ((i + element) >= 1e3 && (i + element) < 1e4) {
				if (count % 8 == 0) {
					myfile << "+       ";
				}
				myfile << "    " << i + element;
				count += 1;
			}
			if (count % 8 == 0) {
				myfile << "+" << std::endl;
			}
		}
	}
	myfile << std::endl;

	//right surface
	double location5 = 4.2672;
	myfile << "WETSURF        5   right                                                +" << std::endl;
	count = 0;
	for (i = 0; i < element_num; i++) {
		if (abs(coord[i][0] - location5)<1e-6) {
			if ((i + element) >= 1e2 && (i + element) < 1e3) {
				if (count % 8 == 0) {
					myfile << "+       ";
				}
				myfile << "     " << i + element;
				count += 1;
			}
			if ((i + element) >= 1e3 && (i + element) < 1e4) {
				if (count % 8 == 0) {
					myfile << "+       ";
				}
				myfile << "    " << i + element;
				count += 1;
			}
			if (count % 8 == 0) {
				myfile << "+" << std::endl;
			}
		}
	}
	myfile << std::endl;
	
    return 0;
}









/*
FILE* fhandle;
const int MAXELEMENT = 10000;
const int nnpe = 4; //number of node per element
int i, j, a, b, c, d, e;
int ret;
int line = 0;
//const char* filename = "FSP_metric6.wet";
const char* filename = "wetsurface.txt";
fhandle = fopen(filename, "r");
if (fhandle == 0) {
printf("Cannot open >>%s<<.", filename);
exit(99);
}
printf("Reading file >>%s<<...\n", filename);

int** ELE;
ELE = new int*[MAXELEMENT];
for (i = 0; i < MAXELEMENT; i++) {
ELE[i] = new int[nnpe + 1];
}
for (i = 0; i < MAXELEMENT; i++) {
for (j = 0; j < nnpe + 1; j++) {
ELE[i][j] = 0;
}
}

int count = 0;
while (!feof(fhandle)) {
//Checks whether the end-of-File indicator associated with stream is set, returning a value different from zero if it is.
//This indicator is generally set by a previous operation on the stream that attempted to read at or past the end-of-file
//std::cout << "line: " << line << std::endl;
//line += 1;
//std::cout << "feof(fhandle) " << feof(fhandle) << std::endl;
ret=fscanf(fhandle, "element %i %i %i %i %i\n", &a, &b, &c, &d, &e); //scan from the first line of module definition
//std::cout << "ret " << ret << std::endl;
if (ret != 0) {
ELE[count][0] = a; //element numbering
ELE[count][1] = b; //the first local node
ELE[count][2] = c; //the second local node
ELE[count][3] = d; //the third local node
ELE[count][4] = e; //the fourth local node
count += 1;
std::cout << a << b << c << d << e << std::endl;
}
std::cout << "do nothing" << std::endl;
}
std::cout << "do nothing" << std::endl;
*/
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <iostream>
#include <fstream>
#include <string>

/*
instruction:
This version of code is used to generate abaqus wetted surface with .wet input. 
The format of wet surface is simpler than Nastran file
*/

int main()
{
	std::string str("element");//
	std::string filename;
	//filename = "FSP_Lu_canopy_loaded_18400_symmetry.wet";
	std::cout << "input the file name here: " << std::endl;
	std::getline(std::cin, filename, '\n');
	std::ifstream theFile; 
	std::ofstream interim;
	interim.open("interim.txt");
	theFile.open(filename);
	if (!theFile) {
		std::cerr << "cannot open the file" << std::endl;
		//return 1;
	}
	int i, j, a, b, c, d, e, k, l, m;
	double f, g, h;
	std::string str1;
	int count = 0;
	const int MAXELEMENT = 100000;
	//double waterline = 0.229;
	//double waterline = 0.0;
	//double waterline = 0.203; 
	double waterline = 0.3048;
	//double waterline = 1.524;
	const int nnpe = 4; //number of node per element 
	const double SX = 4.2672 * 2;
	const double SY = 0.9144;
	const double SZ = 2.4384 * 2;
	int*** ELE;
	double**coord;
	ELE = new int**[MAXELEMENT];
	coord = new double*[MAXELEMENT];
	for (i = 0; i < MAXELEMENT; i++) {
		ELE[i] = new int*[nnpe + 3];
		coord[i] = new double[3];
		for (j = 0; j < nnpe + 3; j++) {
			ELE[i][j] = new int[5];
		}
	}

	for (i = 0; i < MAXELEMENT; i++) {
		for (j = 0; j < nnpe + 3; j++) {
			for (l = 0; l < 5;l++) {
				ELE[i][j][l] = 0;
			}
		}
		for (j = 0; j < 3; j++) {
			coord[i][j] = 0.0;
		}
	}

	char buffer[256];
	char delimeter('"');
	while (!theFile.eof())
	{	
		theFile.getline(buffer, 256, '\n');
		//theFile.getline(buffer, delimeter);
		if (buffer[0] == 'e'&&buffer[1] == 'l'&&buffer[2] == 'e') {
			for (i = 0; i < 257; i++) {
				if (buffer[i] != delimeter) {
					interim << buffer[i];
				}
				else {
					break;
				}
			}
			interim << std::endl;
		}
	}
	interim.close();

	std::ifstream interim_op;
	interim_op.open("interim.txt");
	if (!interim_op) {
		std::cout << "cannot open the file" << std::endl;
		//return 1;
	}

	int count1 = 0; int count2 = 0; int count3 = 0; int count4 = 0; int count5 = 0;
	while (interim_op >> str1 >> a >> b >> c >> d >> e >> f >> g >> h >> k >> l >> m)
		//while (interim_op >> str1 >> a >> b >> c >> d >> e >> k)
	{
		if (a < 0) {
			a = -a;
		}
		if (g - waterline < -1e-5) {
			std::cout << str1 << a << b << c << d << e << f << std::endl;
			//back surface
			if (abs(h + SZ / 2) < 1e-5) {
				ELE[count1][0][0] = b; //the first local node
				ELE[count1][1][0] = c; //the second local node
				ELE[count1][2][0] = d; //the third local node
				ELE[count1][3][0] = e; //the fourth local node
				ELE[count1][4][0] = a; //element numbering 
				count1 += 1;
			}
			//left 
			if ((abs(f + SX / 2) < 1e-5)) {
				ELE[count2][0][1] = b; //the first local node
				ELE[count2][1][1] = c; //the second local node
				ELE[count2][2][1] = d; //the third local node
				ELE[count2][3][1] = e; //the fourth local node
				ELE[count2][4][1] = a; //element numbering 
				count2 += 1;
			}
			//bottom
			if (abs(g + SY) < 1e-5) {
				ELE[count3][0][2] = b; //the first local node
				ELE[count3][1][2] = c; //the second local node
				ELE[count3][2][2] = d; //the third local node
				ELE[count3][3][2] = e; //the fourth local node
				ELE[count3][4][2] = a; //element numbering 
				count3 += 1;
			}
			//front 
			if (abs(h - SZ / 2) < 1e-5) {
				ELE[count4][0][3] = b; //the first local node
				ELE[count4][1][3] = c; //the second local node
				ELE[count4][2][3] = d; //the third local node
				ELE[count4][3][3] = e; //the fourth local node
				ELE[count4][4][3] = a; //element numbering 
				count4 += 1;
			}
			//right
			if (abs(f - SX / 2) < 1e-5) {
				ELE[count5][0][4] = b; //the first local node
				ELE[count5][1][4] = c; //the second local node
				ELE[count5][2][4] = d; //the third local node
				ELE[count5][3][4] = e; //the fourth local node
				ELE[count5][4][4] = a; //element numbering 
				count5 += 1;
			}
			//coord[count][0] = f; //x coordinate of element central point
			//coord[count][1] = g; //y coordinate of element central point 
			//coord[count][2] = h; //z coordinate of element central point
		}
	}

	//int element_num = count;
	std::ofstream myfile;
	myfile.open("wetsurface_formated.txt");


	myfile << "*Elset, elset=FSI_structure_back, internal, instance=STRUCTURE-1" << std::endl;
	for (i = 0; i < count1; i++) {
		if ((i + 1) % 16 == 0) { //16 items every line 
			myfile << ELE[i][4][0];
			myfile << std::endl;
		}
		else {
			myfile << ELE[i][4][0] << ", ";
		}
	}
	myfile << std::endl;
	myfile << "*Elset, elset=FSI_structure_left, internal, instance=STRUCTURE-1" << std::endl;
	for (i = 0; i < count2; i++) {
		if ((i + 1) % 16 == 0) { //16 items every line 
			myfile << ELE[i][4][1];
			myfile << std::endl;
		}
		else {
			myfile << ELE[i][4][1] << ", ";
		}
	}
	myfile << std::endl;
	myfile << "*Elset, elset=FSI_structure_bottom, internal, instance=STRUCTURE-1" << std::endl;
	for (i = 0; i < count3; i++) {
		if ((i + 1) % 16 == 0) { //16 items every line 
			myfile << ELE[i][4][2];
			myfile << std::endl;
		}
		else {
			myfile << ELE[i][4][2] << ", ";
		}
	}
	myfile << std::endl;
	myfile << "*Elset, elset=FSI_structure_front, internal, instance=STRUCTURE-1" << std::endl;
	for (i = 0; i < count4; i++) {
		if ((i + 1) % 16 == 0) { //16 items every line 
			myfile << ELE[i][4][3];
			myfile << std::endl;
		}
		else {
			myfile << ELE[i][4][3] << ", ";
		}
	}
	myfile << std::endl;
	myfile << "*Elset, elset=FSI_structure_right, internal, instance=STRUCTURE-1" << std::endl;
	for (i = 0; i < count5; i++) {
		if ((i + 1) % 16 == 0) { //16 items every line 
			myfile << ELE[i][4][4];
			myfile << std::endl;
		}
		else {
			myfile << ELE[i][4][4] << ", ";
		}
	}


	std::cout << " " << std::endl;

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
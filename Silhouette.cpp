
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>
#include <cmath>
#include <direct.h>
#include <windows.h>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;
using namespace cv;

struct count_i {
	//unsigned char note;
	unsigned short counter;//Counting voxel
};
struct voxel_vol {
	count_i z[100];
};

int main(void)
{
	//Instrisic Parameter and extrinsic
	double ins[3][3] = { { 722.481995,0.000000,399.000000 },{ 0.000000,722.481934,311.000000 },{ 0.000000,0.000000,1.000000 } };
	double extr[3][4];
	int xx, yy, zz;
	//Monkey Teapot Last
	string folder_txt[12] = { "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11" };
	string ss, ss2;//0915/matching_street_1016_LRC											
	ifstream infile;//定義讀取檔案流，相對於程式來說是in
	FILE* Write = fopen("WriteTest.txt", "w");
	double result[3][4];
	int file_start = 0, file_end = 11;

	voxel_vol **accumulator = (voxel_vol **)calloc((100), sizeof(voxel_vol*));
	for (int r = 0; r < (100); r++)
		accumulator[r] = (voxel_vol *)calloc(100, sizeof(voxel_vol));

	for (int zzz = 0; zzz < 100; zzz++)
	{
		for (int yyy = 0; yyy < 100; yyy++)
		{
			for (int xxx = 0; xxx < 100; xxx++)
				accumulator[xxx][yyy].z[zzz].counter = 0;
		}
	}
	for (int file = file_start; file < file_end; file += 1)
	{
		cout << "File = " << file << endl;
		ss = int2str(file);
		Mat image1 = imread("E://Monkey/" + folder_txt[file] + ".bmp");
		infile.open("E://tt/" + folder_txt[file] + ".txt");//開啟檔案
		cvtColor(image1, image1, COLOR_BGR2GRAY);
		for (int i = 0; i < 3; i++)//定義行迴圈
		{
			for (int j = 0; j < 4; j++)//定義列迴圈
			{
				infile >> extr[i][j];//讀取一個值（空格、製表符、換行隔開）就寫入到矩陣中，行列不斷迴圈進行
									 //printf("%.5f ", extr[i][j]);
			}
			//printf("\n ");
		}
		printf("\n ");
		infile.close();//讀取完成之後關閉檔案

		for (int t = 0; t < 3; t++) {
			for (int r = 0; r < 4; r++) {
				result[t][r] = 0;
				for (int k = 0; k < 3; k++)
					result[t][r] += ins[t][k] * extr[k][r];
			}
		}
		int aaa, bbb, ccc;
		for (int z = 0; z < 100; z++)
		{
			for (int y = -50; y < 50; y++)
			{
				for (int x = -50; x < 50; x++)
				{
					aaa = int(result[0][0] * x + result[0][1] * y + result[0][2] * z + result[0][3]);
					bbb = int(result[1][0] * x + result[1][1] * y + result[1][2] * z + result[1][3]);
					ccc = int(result[2][0] * x + result[2][1] * y + result[2][2] * z + result[2][3]);

					aaa /= ccc;//x
					bbb /= ccc;//y
					ccc /= ccc;//z

					if ((aaa > 0 && aaa < image1.cols && bbb > 0 && bbb < image1.rows))
					{
						if (image1.at<uchar>(bbb, aaa) == 255) {
							xx = x + 50;
							yy = y + 50;
							zz = z;
							accumulator[xx][yy].z[zz].counter += 1;
							//printf("%d %d %d\n",xx,yy, accumulator[xx][yy].z[zz].counter);
						}
					}
				}
			}
		}
	}
	int x2, y2;
	for (int zzz = 0; zzz < 100; zzz++)
	{
		for (int yyy = 0; yyy < 100; yyy++)
		{
			for (int xxx = 0; xxx < 100; xxx++)
			{
				if (accumulator[xxx][yyy].z[zzz].counter>10) //threshold is 10 ,because we have ? image
				{
					x2 = xxx - 50;//store&& accumulator[xxx][yyy].z[zzz].counter>5
					y2 = yyy - 50;//store array
								  //printf("%d %d %d %d\n", x2, y2, zzz, accumulator[xxx][yyy].z[zzz].counter);
					fprintf(Write, "%d %d %d \n", x2, y2, zzz);
				}
			}
		}
	}

	for (int r = 0; r < 100; r++)
		free(accumulator[r]);//free 2d array memory
	free(accumulator);//free 1d array memory

	printf("ok\n");
	fclose(Write);
	waitKey(0);
	system("pause");
	return 0;
}
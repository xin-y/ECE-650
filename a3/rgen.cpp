#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

using namespace std;


//obtain random number from /dev/random
int random_num(int min, int max) { // refer to "Advanced linux programming" - Listing 6.1
	unsigned int random;

	int fd = -1;

	if ((fd = open("/dev/urandom", O_RDONLY)) == -1) {

		cout << "cannot open /dev/urandom." << endl;

		exit(1);

	}

	read(fd, &random, sizeof(unsigned int));

	close(fd);

	return  min + (random % (max - min + 1));
}

string random_street_name(int len) {
	//srand(time(0));
	string str = "abcdefghijklmnopqrstuvwxyz";
	string street_name;
	int pos;
	while (street_name.size() != len) {
		pos = ((rand() % (str.size() - 1)));
		street_name += str.substr(pos, 1);
	}
	return street_name;
}


bool same_point(int x1, int y1, int x2, int y2) {

	if (x1 == x2 && y1 == y2) {
		return true;
	}
	else {
		return false;
	}
}

bool TwoLinesOverlap(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
{

	int Minx01 = min(x0, x1);
	int Miny01 = min(y0, y1);
	int Minx23 = min(x2, x3);
	int Miny23 = min(y2, y3);
	int Maxx01 = max(x0, x1);
	int Maxy01 = max(y0, y1);
	int Maxx23 = max(x2, x3);
	int Maxy23 = max(y2, y3);

	if (x1 != x0 && x2 != x3)
	{
		float k1 = (y1 - y0) / (x1 - x0);
		float k2 = (y3 - y2) / (x3 - x2);
		float Den = (y1 - y0) * (x3 - x2) - (y3 - y2) * (x1 - x0);
		if (k1 == k2)
		{ //parrellel
			float d1 = abs(y0 * (x1 - x0) - x0 * (y1 - y0) - y2 * (x3 - x2) + x2 * (y3 - y2)); //??d = abs(c1-c2) / sqrt(a*a+b*b)
			if (d1 == 0)
			{//overlap
				if ((x2 > Minx01&& x2<Maxy01 && y2>Miny01&& y2 < Maxy01) || (x3 > Minx01&& x3<Maxy01 && y3>Miny01&& y3 < Maxy01)
					|| (x0 > Minx23&& x0<Maxy23 && y0>Miny23&& y0 < Maxy23) || (x1 > Minx23&& x1<Maxy23 && y1>Miny23&& y1 < Maxy23))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else {
			return false;
		}
	}
}

bool is_valid_point(vector <int> point_list, int x, int y) {
	int len = point_list.size();
	if (len == 0) {
		return true;
	}
	else if (len == 2) {
		if ((same_point(x, y, point_list[0], point_list[1]))) {
			return false;
		}
		return true;
	}
	else {
		for (int i = 0; i < point_list.size(); i = i + 2) {
			if (same_point(x, y, point_list[i], point_list[i + 1])) {
				return false;
			}
		}

	}

}

bool is_overlap(vector <int> point_list,vector< vector<int> > all_points ) {
	for (int i = 2; i < point_list.size(); i = i + 2) {
		for (int j = 0; j < all_points.size(); j + 2) {
			for (int k = 2; k < all_points[j].size(); k + 2) {
				if (TwoLinesOverlap(point_list[i], point_list[i + 1],point_list[i-2],point_list[i-1],all_points[j][k],all_points[j][k+1],all_points[j][k-2],all_points[j][k-1] )) {
					return true;
				}
			}
		}
	}
	return false;
}

int main(int argc, char* argv[]) {
	// https://blog.csdn.net/Rong_Toa/article/details/79927617 take command-line arguments
	int ch;
	int street_k = 10, line_k = 5, wait_k = 5, range_k = 20;
	while (ch = getopt(argc, argv, "s:n:l:c") != -1) {
		switch (ch)
		{
		case 's':
			if (optarg != NULL) {
				street_k = atoi(optarg);//The number of streets should be a random integer in [2, k].default k = 10
			}
			else {
				street_k = 10;
			}
			break;
		case 'n':
			if (optarg != NULL) {
				line_k = atoi(optarg); //The number of line-segments in each street should be arandom integer in[1, k].Default: k = 5.
			}
			else {
				line_k = 5;
			}
			break;
		case 'l':
			if (optarg != NULL) {
				wait_k = atoi(optarg);
			}// wait a random number w seconds,where w is in[5, k] before generating the next(random) input.Default: k = 5.
			else {
				wait_k = 5;
			}
			break;
		case 'c':
			if (optarg != NULL) {
				range_k = atoi(optarg);
			}// generate (x, y) coordinates such that every xand y value is in the range [-k, k]. Default:k = 20.
			else {
				range_k = 20;
			}
			break;
		default:
			break;
		}
	}
	int street_num, line_num, wait_num, point_num;
	int cnt = 0;
	string street_name;
	vector <string> street_list;

	std::vector< std::vector<int> > all_points;

	while (1) {
		street_num = random_num(2, street_k);
		wait_num = random_num(5, wait_k);


		while (cnt < street_num) {
			vector <int> point_list, xs, ys;
			line_num = random_num(1, line_k);

			for (int l = 0; l <= 2 * line_num; l++) {
				int try_cnt = 0;
				int x_value, y_value;

				x_value = random_num(-range_k, range_k);
				y_value = random_num(-range_k, range_k);

				if (is_valid_point(point_list, x_value, y_value)&&!is_overlap(point_list,all_points))
				{
					point_list.push_back(x_value);
					point_list.push_back(y_value);

					try_cnt = 0;
					
				}

				else
				{

					if (try_cnt == 25)
					{
						cerr << "Error: failed to generate valid input for 25 simultaneous attempts" << endl;
						exit(1);
					}

					try_cnt++;
					continue;
				}
			}

			street_name = random_street_name(4);
			street_list.push_back(street_name);

			all_points.push_back(point_list);

			cout << "a " << "\"" << street_name << "\" ";
			for (int i = 0; i <= line_num; i++) {
				cout << "(" << point_list[i] << "," << point_list[i + 1] << ") ";
			}
			cout << endl;
			cnt++;
		}
		cout << "g" << endl;

		sleep(wait_num);

		for (int i = 0; i < street_list.size(); i++) {
			std::cout << "r " << "\"" << street_list[i] << "\"" << std::endl;
		}
		street_list.clear();
		cnt = 0;

	}

	return 0;
}

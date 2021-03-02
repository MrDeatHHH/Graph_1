#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono> 

using namespace cv;
using namespace std;
using namespace std::chrono;

void init(const int width, const int height, int** colors, int& length, int**& tau, int*& nt, int**& c, int**& f)
{
	length = width * height + 2;
	nt = new int[length];
	tau = new int* [length];
	c = new int* [length];
	f = new int* [length];
	for (int i = 0; i < width; ++i)
	{
		if ((i == 0) || (i == width - 1))
		{
			for (int j = 0; j < height; ++j)
			{
				if ((j == 0) || (j == height - 1))
				{
					nt[i * height + j] = 3;
					tau[i * height + j] = new int[nt[i * height + j]];
					f[i * height + j] = new int[nt[i * height + j]];
					c[i * height + j] = new int[nt[i * height + j]];
					// Horizontal
					tau[i * height + j][0] = int(i == 0) * ((i + 1) * height + j) +
						                     int(i == width - 1) * ((i - 1) * height + j);
					f[i * height + j][0] = 0;
					c[i * height + j][0] = 50;
					// Vertical
					tau[i * height + j][1] = i * height + int(j == height - 1) * (j - 1) + int(j == 0) * (j + 1);
					f[i * height + j][1] = 0;
					c[i * height + j][1] = 50;
					// Finish
					tau[i * height + j][2] = length - 1;
					f[i * height + j][2] = 0;
					c[i * height + j][2] = 255 - colors[i][j];
				}
				else
				{
					nt[i * height + j] = 4;
					tau[i * height + j] = new int[nt[i * height + j]];
					f[i * height + j] = new int[nt[i * height + j]];
					c[i * height + j] = new int[nt[i * height + j]];
					// Horizontal
					tau[i * height + j][0] = int(i == 0) * ((i + 1) * height + j) +
						                     int(i == width - 1) * ((i - 1) * height + j);
					f[i * height + j][0] = 0;
					c[i * height + j][0] = 50;
					// Vertical Up
					tau[i * height + j][1] = i * height + j - 1;
					f[i * height + j][1] = 0;
					c[i * height + j][1] = 50;
					// Vertical Down
					tau[i * height + j][2] = i * height + j + 1;
					f[i * height + j][2] = 0;
					c[i * height + j][2] = 50;
					// Finish
					tau[i * height + j][3] = length - 1;
					f[i * height + j][3] = 0;
					c[i * height + j][3] = 255 - colors[i][j];
				}
			}
		}
		else
		{
			for (int j = 0; j < height; ++j)
			{

				if ((j == 0) || (j == height - 1))
				{
					nt[i * height + j] = 4;
					tau[i * height + j] = new int[nt[i * height + j]];
					f[i * height + j] = new int[nt[i * height + j]];
					c[i * height + j] = new int[nt[i * height + j]];
					// Horizontal Left
					tau[i * height + j][0] = (i - 1) * height + j;
					f[i * height + j][0] = 0;
					c[i * height + j][0] = 50;
					// Horizontal Rigth
					tau[i * height + j][1] = (i + 1) * height + j;
					f[i * height + j][1] = 0;
					c[i * height + j][1] = 50;
					// Vertical
					tau[i * height + j][2] = i * height + int(j == height - 1) * (j - 1) + int(j == 0) * (j + 1);
					f[i * height + j][2] = 0;
					c[i * height + j][2] = 50;
					// Finish
					tau[i * height + j][3] = length - 1;
					f[i * height + j][3] = 0;
					c[i * height + j][3] = 255 - colors[i][j];
				}
				else
				{
					nt[i * height + j] = 5;
					tau[i * height + j] = new int[nt[i * height + j]];
					f[i * height + j] = new int[nt[i * height + j]];
					c[i * height + j] = new int[nt[i * height + j]];
					// Horizontal Left
					tau[i * height + j][0] = (i - 1) * height + j;
					f[i * height + j][0] = 0;
					c[i * height + j][0] = 50;
					// Horizontal Rigth
					tau[i * height + j][1] = (i + 1) * height + j;
					f[i * height + j][1] = 0;
					c[i * height + j][1] = 50;
					// Vertical Up
					tau[i * height + j][2] = i * height + j - 1;
					f[i * height + j][2] = 0;
					c[i * height + j][2] = 50;
					// Vertical Down
					tau[i * height + j][3] = i * height + j + 1;
					f[i * height + j][3] = 0;
					c[i * height + j][3] = 50;
					// Finish
					tau[i * height + j][4] = length - 1;
					f[i * height + j][4] = 0;
					c[i * height + j][4] = 255 - colors[i][j];
				}
			}
		}
	}
	nt[length - 2] = length - 2;
	tau[length - 2] = new int[nt[length - 2]];
	f[length - 2] = new int[nt[length - 2]];
	c[length - 2] = new int[nt[length - 2]];
	nt[length - 1] = 0;
	tau[length - 1] = new int[1];
	f[length - 1] = new int[1];
	c[length - 1] = new int[1];
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			tau[length - 2][i * height + j] = i * height + j;
			f[length - 2][i * height + j] = 0;
			c[length - 2][i * height + j] = colors[i][j];
		}
	}
}

int* bfs(const int start, const int finish, const int length, int** tau, int* nt, int** c, int** f, int& path_length)
{
	int* queue = new int[length]();
	queue[0] = start;
	int cur = 0;
	int end = 0;
	bool* visited = new bool[length]();
	for (int i = 0; i < length; ++i)
	{
		visited[i] = false;
	}
	visited[start] = true;
	int* came_from = new int[length]();
	// TODO: try dfs with tau[i][nt[i] - 1] = finish
	while ((!visited[finish]) && (cur - 1 != end))
	{
		for (int i = 0; i < nt[queue[cur]]; ++i)
		{
			if ((f[queue[cur]][i] != c[queue[cur]][i]) &&
				(!visited[tau[queue[cur]][i]]))
			{
				visited[tau[queue[cur]][i]] = true;
				++end;
				queue[end] = tau[queue[cur]][i];
				came_from[tau[queue[cur]][i]] = queue[cur];
			}
		}
		++cur;
	}
	if (visited[finish])
	{
		cur = finish;
		path_length = 1;
		while (cur != start)
		{
			cur = came_from[cur];
			++path_length;
		}
		int* path = new int[path_length];
		path[path_length - 1] = finish;
		cur = path_length - 1;
		while (came_from[path[cur]] != start)
		{
			--cur;
			path[cur] = came_from[path[cur + 1]];
		}
		path[0] = start;
		delete[] visited;
		delete[] queue;
		delete[] came_from;
		return path;
	}
	else
	{
		path_length = 0;
		int* path = new int[1];
		delete[] visited;
		delete[] queue;
		delete[] came_from;
		return path;
	}
}

int find(int* array, const int length, const int value)
{
	int index = -1;
	for (int i = 0; i < length; ++i)
	{
		if (array[i] == value)
		{
			return i;
		}
	}
}

bool update_flow(const int start, const int finish, const int length, int** tau, int* nt, int** c, int** f)
{
	int path_length = 0;
	int* path;
	path = bfs(start, finish, length, tau, nt, c, f, path_length);
	if (path_length != 0)
	{
		int min_delta_f = 1000000;
		int cur_delta_f = 1000000;
		int best_i = -1;
		for (int i = 0; i < path_length - 1; ++i)
		{
			cur_delta_f = c[path[i]][find(tau[path[i]], nt[path[i]], path[i + 1])] -
				          f[path[i]][find(tau[path[i]], nt[path[i]], path[i + 1])];
			if (cur_delta_f < min_delta_f)
			{
				min_delta_f = cur_delta_f;
				best_i = i;
			}
		}
		for (int i = 0; i < path_length - 1; ++i)
		{
			f[path[i]][find(tau[path[i]], nt[path[i]], path[i + 1])] += min_delta_f;
			if ((path[i + 1] != finish) && (path[i] != start))
			{
				f[path[i + 1]][find(tau[path[i + 1]], nt[path[i + 1]], path[i])] -= min_delta_f;
			}
		}

		delete[] path;
		return true;
	}
	else
	{
		delete[] path;
		return false;
	}
}

int* mincut(const int start, const int finish, const int length, int** tau, int* nt, int** c, int** f)
{
	int* queue = new int[length]();
	queue[0] = start;
	int cur = 0;
	int end = 0;
	bool* visited = new bool[length]();
	for (int i = 0; i < length; ++i)
	{
		visited[i] = false;
	}
	visited[start] = true;
	while (cur - 1 != end)
	{
		for (int i = 0; i < nt[queue[cur]]; ++i)
		{
			if ((f[queue[cur]][i] < c[queue[cur]][i]) &&
				(!visited[tau[queue[cur]][i]]))
			{
				visited[tau[queue[cur]][i]] = true;
				++end;
				queue[end] = tau[queue[cur]][i];
			}
		}
		++cur;
	}

	int* tetta = new int[length]();
	for (int i = 0; i < length; ++i)
	{
		tetta[0] = 0;
	}
	for (int i = 0; i < end; ++i)
	{
		tetta[queue[i]] = 1;
	}
	return tetta;
}

int main()
{
	Mat image_, image;
	image_ = imread("1.png", IMREAD_UNCHANGED);
	cvtColor(image_, image, COLOR_BGR2GRAY);

	auto start = high_resolution_clock::now();

	const int height = image.size().height;
	const int width = image.size().width;

	// Get array from Mat
	int** colors = new int* [width];
	for (int x = 0; x < width; ++x)
	{
		colors[x] = new int[height];
		for (int y = 0; y < height; ++y)
			colors[x][y] = int(image.at<uchar>(y, x));
	}
	int length;
	int** tau;
	int* nt;
	int** c;
	int** f;
	init(width, height, colors, length, tau, nt, c, f);
	int counter = 0;
	while (update_flow(length - 2, length - 1, length, tau, nt, c, f) && (counter < 10000000)) { ++counter; }
	int* res = mincut(length - 2, length - 1, length, tau, nt, c, f);

	cout << "Res" << endl;
	Mat* result = new Mat[3];
	for (int c = 0; c < 3; ++c)
	{
		result[c] = Mat::zeros(Size(width, height), CV_8UC1);
		for (int x = 0; x < width; ++x)
			for (int y = 0; y < height; ++y)
			{
				result[c].at<uchar>(y, x) = uchar(255 * int(res[x * height + y] == 1));
			}
	}

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Time used: " << double(duration.count()) / 1000000. << endl;

	Mat rez;
	vector<Mat> channels;

	channels.push_back(result[0]);
	channels.push_back(result[1]);
	channels.push_back(result[2]);

	merge(channels, rez);

	namedWindow("Result image", WINDOW_AUTOSIZE);
	imshow("Result image", rez);
	imwrite("res.png", rez);

	waitKey(0);
	return 0;
}
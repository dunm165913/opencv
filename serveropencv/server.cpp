
#include <iostream>
#pragma comment(lib,"Ws2_32.lib")
#include <WinSock2.h>
#include <sys/types.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <Windows.h>
#include "opencv2\video\background_segm.hpp"
#include <opencv2\video\tracking.hpp>
using namespace std;
using namespace cv;

#define FRAME_WIDTH        640
#define FRAME_HEIGHT       480

Mat cameraFeed;
SOCKET server, client;
int height, width, IM_HEIGHT, IM_WIDTH, imgSize;

int bytes = 0;;
bool running = true;
char key;

int main() {
	int d = 0;
	VideoCapture cap(0);
	WSADATA WSAData;

	SOCKADDR_IN serverAddr, clientAddr;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	server = socket(AF_INET, SOCK_STREAM, 0);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(7200);

	bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	listen(server, 5);
	cout << "LISTENING FOR INCOMING CONNECTIONS..." << endl;

	while (1) {
		char buf[1024];
		int clientAddrSize = sizeof(clientAddr);
		if ((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)))
		{
			cout << "Client connected:" << endl;
		}


		while (running)
		{
			if (d != 0) break;
			int IM_HEIGHT = FRAME_HEIGHT;
			int IM_WIDTH = FRAME_WIDTH;
			Mat img = Mat::zeros(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3);
			imgSize = img.total()*img.elemSize();


			const int ah = 921600;
			char sockData[ah];

			for (int i = 0; i < imgSize; i += bytes)
				if ((bytes = recv(client, sockData + i, imgSize - i, 0)) == -1) {
					if (bytes <= 0) {
						cout << "Client off";
						d = 1;
						break;

					}
					cout << "ENROL:" << endl;
				}
			int ptr = 0;
			for (int i = 0; i < img.rows; i++)
				for (int j = 0; j < img.cols; j++) {
					img.at<Vec3b>(i, j) = Vec3b(sockData[ptr + 0], sockData[ptr + 1], sockData[ptr + 2]);
					ptr = ptr + 3;
				}
			namedWindow("Client send to server", CV_WINDOW_AUTOSIZE);
			imshow("Client send to server", img);
			key = waitKey(30);
			running = key;
			if (key == 27)
			{
				closesocket(server);
				cout << "SERVER DISCONNECT:" << endl;
				running = false;
			}


		}
	}



}
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

#define FRAME_WIDTH             640
#define FRAME_HEIGHT            480

int bytes = 0;;
bool running = true;
char key;


Mat cameraFeed;
SOCKET server, client;
int height, width, IM_HEIGHT, IM_WIDTH, imgSize;

int main() {
	VideoCapture cap(0);



	WSADATA WSAData;
	SOCKET server;
	SOCKADDR_IN addr;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	server = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(7200);
	connect(server, (SOCKADDR *)&addr, sizeof(addr));
	cout << "CONECT TO SERVER:" << endl;


	while (true) {



		Mat frame, image;

		if (!cap.read(cameraFeed)) {
			cout << "video ended" << endl;
			break;
		}
		height = cameraFeed.rows;
		width = cameraFeed.cols;
		IM_HEIGHT = FRAME_HEIGHT;
		IM_WIDTH = FRAME_WIDTH;
		resize(cameraFeed, cameraFeed, Size(IM_WIDTH, IM_HEIGHT));
		imgSize = cameraFeed.total()*cameraFeed.elemSize();
		send(server, reinterpret_cast<const char*>(cameraFeed.data), imgSize, 0);

		imshow("On CLient", cameraFeed);
		if (waitKey(30) == 27)
		{
			closesocket(client);
			cout << "CLIENT DISCONNECTED:" << endl;
			return 0;
		}






	}



	return 0;
}

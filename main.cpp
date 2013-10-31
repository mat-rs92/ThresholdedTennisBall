#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cxcore.h"
#include <stdio.h>

using namespace cv;

//--------------IMPOSTAZIONI------------
//imposto la risoluzione di acquisizione
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//finestre
const string mainGui="Immagine acquisita";

int main(int argc,char* argv[]){
	//avvio della videocamera
	VideoCapture capture;
	capture.open(0);
	//matrice su cui verranno salvati i frame catturati
	Mat cameraFeed;
	//imposto la dimensione dei frame da catturare
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	
	while(1){
		//catturo un frame della webcam
		capture.read(cameraFeed);
		//visualizzo su mainGui il frame originale
		imshow(mainGui,cameraFeed);
		
		//tempo di attesa prima di passare al frame successivo
		waitKey(33);
	}
	
	
	return(0);
}

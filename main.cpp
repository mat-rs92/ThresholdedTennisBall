#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cxcore.h"
#include <stdio.h>

using namespace cv;

//--------------IMPOSTAZIONI------------
//imposto la risoluzione di acquisizione
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//valori preimpostati per tracciare la pallina, cambiabili con le trackbar
int H_MIN = 20;
int S_MIN = 22;
int V_MIN = 161;
int H_MAX = 256;
int S_MAX = 256;
int V_MAX = 256;
//finestre
const string mainGui="Immagine acquisita";
const string thresholdWindow="Immagine rilevata";
const string settingWindow="Imposta soglia";
const string blurWindow="Immagine con filtro Blur";

//action listener per gli slider -> INUTILISSIMO MA NECESSARIO
void onTrackbarSlide(int, void*){
	//non c'è bisogno di fare nulla	
}

//metodo che crea gli slider
void createSlider(){
	namedWindow(settingWindow,0);
	//metodo che crea le trackbar(label, finestra, valore da cambiare, valore massimo,action listener)
	createTrackbar("H-min",settingWindow, &H_MIN, H_MAX, onTrackbarSlide);
    	createTrackbar("S-min",settingWindow, &S_MIN, S_MAX,onTrackbarSlide);
    	createTrackbar("V-min",settingWindow, &V_MIN, V_MAX,onTrackbarSlide);
    	createTrackbar("H-max",settingWindow, &H_MAX, H_MAX, onTrackbarSlide);
    	createTrackbar("S-max",settingWindow, &S_MAX, S_MAX,onTrackbarSlide);
    	createTrackbar("V-max",settingWindow, &V_MAX, V_MAX,onTrackbarSlide);
}


int main(int argc,char* argv[]){
	//avvio della videocamera
	VideoCapture capture;
	//0--> webcam default (interna)
	//1--> webcam esterna
	capture.open(0);
	//matrice su cui verranno salvati i frame catturati
	Mat cameraFeed;
	//matrice su cui salvo l'immagine HSV
	Mat hsvFrame;
	//matrice su cui salvo l'immagine filtrata
	Mat thresholded;
	//matrice per mostrare l'immagine con il filtro blur
	Mat frameBlur;
	
	
	//imposto la dimensione dei frame da catturare
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	

	//creo la finestra con gli slider
	createSlider();
	
	while(1){
		//catturo un frame della webcam
		capture.read(cameraFeed);
		
		//applico il gaussian blur
		medianBlur(cameraFeed,frameBlur,5);

		//cambio lo spazio dei colori RGB-->HSV
		//cvtColor(sorgente, destinazione, operazione)
		cvtColor(frameBlur,hsvFrame,COLOR_BGR2HSV);
		
		//filtro hsvFrame cercando solo un determinato range di colori
		//void inRange(InputArray src, InputArray lowerbound, InputArray upperbound, OutputArray dst)
		inRange(hsvFrame, Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),thresholded);
		
		//--------applico l'erosione e la dilatazione---------
		//questa matrice  indica che i 3x3 pixel intorno ad ogni punto da analizzare sono un rettangolo
		//in pratica per ogni pixel erodo il rettangolo 3x3 intorno 
		//IN PRATICA: con l'erosione rimuovo tutti i puntini piccoli che compaiono riducendo il rumore
		Mat rectErosione = getStructuringElement(MORPH_RECT,Size(3,3));
		erode(thresholded, thresholded,rectErosione);
		erode(thresholded, thresholded,rectErosione);
		//dilato ogni pixel rilevato in un rect 8x8
		Mat rectDilataz = getStructuringElement( MORPH_RECT,Size(8,8));
		dilate(thresholded, thresholded, rectDilataz);
		dilate(thresholded, thresholded, rectDilataz);
		
		//visualizzo su mainGui il frame originale
		imshow(mainGui,cameraFeed);
		//visualizzo su thresholdWindow l'immagine filtrata
		imshow(thresholdWindow,thresholded);
		//visualizzo su blurWindow l'immagine con il filtro blur
		//imshow(blurWindow,frameBlur);
		
		//premi un pulsante per uscire oppure attendi il tempo di attesa prima di passare al frame successivo
		if(waitKey(33) >= 0) break;
	}
	
	
	return(0);
}

#include <iostream>
#include <TGraphErrors.h>     
#include <TAxis.h>          
#include <TCanvas.h>        
#include <TF1.h>              
#include "TVirtualFitter.h"  
#include <iomanip>  
#include <TMultiGraph.h> 
#include <TGraph.h> 
#include <TLegend.h> 

void Ib_vs_Vbe()
{
	std::cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££" << endl;

	std::cout << fixed;

	const int nmisure = 4;

	//il tipo ci ha dato solo questi valori di Vbe. non ci ha dato tutti i suoi valori di Vbe

	//Vce = 3(V)
	//V
	//Amprobe 37XR-A
	float Vbe_3[] = {0.589,0.608,0.620,0.622};
	//errori datasheet Amprobe 37XR-A
	float sVbe_3[nmisure];
	for(int i = 0; i < nmisure; i++)
	{
		sVbe_3[i] = Vbe_3[i] * 0.001 + 5 * 0.001;
	}

	//µA
	//Amprobe 37XR-A
	float Ib_3[] = {100,200,300,400};
	//errori datasheet Amprobe 37XR-A
	float sIb_3[nmisure];
	for(int i = 0; i < nmisure; i++)
	{
		sIb_3[i] = Ib_3[i] * 0.005 + 10 * 0.1;
	}



	//Vce = 5(V)
	//V
	//Amprobe 37XR-A
	float Vbe_5[] = {0.589,0.598,0.605,0.609};
	//errori datasheet Amprobe 37XR-A
	float sVbe_5[nmisure];
	for(int i = 0; i < nmisure; i++)
	{
		sVbe_5[i] = Vbe_5[i] * 0.001 + 5 * 0.001;
	}

	//µA
	//Amprobe 37XR-A
	float Ib_5[] = {100,200,300,400};
	//errori datasheet Amprobe 37XR-A
	float sIb_5[nmisure];
	for(int i = 0; i < nmisure; i++)
	{
		sIb_5[i] = Ib_5[i] * 0.005 + 10 * 0.1;
	}



	//Vce = 6e5(V)
	//V
	//Amprobe 37XR-A
	float Vbe_6e5[] = {0.580,0.590,0.591,0.597};
	//errori datasheet Amprobe 37XR-A
	float sVbe_6e5[nmisure];
	for(int i = 0; i < nmisure; i++)
	{
		sVbe_6e5[i] = Vbe_6e5[i] * 0.001 + 5 * 0.001;
	}

	//µA
	//Amprobe 37XR-A
	float Ib_6e5[] = {100,200,300,400};
	//errori datasheet Amprobe 37XR-A
	float sIb_6e5[nmisure];
	for(int i = 0; i < nmisure; i++)
	{
		sIb_6e5[i] = Ib_6e5[i] * 0.005 + 10 * 0.1;
	}

  TCanvas * cIbVbe = new TCanvas("cIbVbe", "grafico1", 0, 0, 1280, 720);
	cIbVbe->SetFillColor(0);
	cIbVbe->cd();

	TMultiGraph * mg = new TMultiGraph();
	mg->SetTitle("I_{b} vs V_{be}, V_{ce} costante          #color[2]{#bullet} V_{ce} = 3 (V)   #color[3]{#bullet} V_{ce} = 5 (V)   #color[4]{#bullet} V_{ce}= 6.5 (V)");
	mg->GetXaxis()->SetTitle("V_{be} (V)");
	mg->GetYaxis()->SetTitle("I_{b} (#muA)");

	TGraphErrors * gIbVbe_3 = new TGraphErrors(nmisure, Vbe_3, Ib_3, sVbe_3, sIb_3);
	gIbVbe_3->SetMarkerSize(0.4);
	gIbVbe_3->SetMarkerStyle(20);
	gIbVbe_3->SetLineColor(2);
	mg->Add(gIbVbe_3);

	TGraphErrors * gIbVbe_5 = new TGraphErrors(nmisure, Vbe_5, Ib_5, sVbe_5, sIb_5);
	gIbVbe_5->SetMarkerSize(0.4);
	gIbVbe_5->SetMarkerStyle(20);
	gIbVbe_5->SetLineColor(3);
	mg->Add(gIbVbe_5);


	TGraphErrors * gIbVbe_6e5 = new TGraphErrors(nmisure, Vbe_6e5, Ib_6e5, sVbe_6e5, sIb_6e5);
	gIbVbe_6e5->SetMarkerSize(0.4);
	gIbVbe_6e5->SetMarkerStyle(20);
	gIbVbe_6e5->SetLineColor(4);
	mg->Add(gIbVbe_6e5);


	mg->Draw("APL");
}

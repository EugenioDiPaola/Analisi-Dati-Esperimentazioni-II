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

void Ic_vs_Ib_Vce_costante()
{

	std::cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££" << endl;

	std::cout << fixed;

	const int nmisure = 4;
	//Vce = 3(V)
	//Ic(mA) e Ib(µA) misurate con Amprobe 37XR-A
	float Ic_3[] = {44,88,130,160};
	float Ib_3[] = {100,200,300,400};

	float sIc_3[nmisure];// = {2,2,2,2};
	for(int i = 0; i < nmisure; ++i)
	{
		sIc_3[i] = Ic_3[i] * (0.005) + 5 * 0.1;
	}
	float sIb_3[nmisure];// = {10,10,10,10};
	for(int i = 0; i < nmisure; ++i)
	{
		sIb_3[i] = Ic_3[i] * (0.005) + 10 * 0.1;
	}

	//Vce = 5(V)
	//Ic(mA) e Ib(µA) misurate con Amprobe 37XR-A
	float Ic_5[] = {47,94,140,170};
	float Ib_5[] = {100,200,300,400};

	float sIc_5[nmisure];// = {2,2,2,2};
	for(int i = 0; i < nmisure; ++i)
	{
		sIc_5[i] = Ic_5[i] * (0.005) + 5 * 0.1;
	}
	float sIb_5[nmisure]; // = {10,10,10,10};
	for(int i = 0; i < nmisure; ++i)
	{
		sIb_5[i] = Ib_5[i] * (0.005) + 10 * 0.1;
	}

	//Vce = 6(V)
	//Ic(mA) e Ib(µA) misurate con Amprobe 37XR-A
	float Ic_6[] = {49,98,140,180};
	float Ib_6[] = {100,200,300,400};

	float sIc_6[nmisure];// = {2,2,2,2};
	for(int i = 0; i < nmisure; ++i)
	{
		sIc_6[i] = Ic_6[i] * (0.005) + 5 * 0.1;
	}
	float sIb_6[nmisure];// = {10,10,10,10};
	for(int i = 0; i < nmisure; ++i)
	{
		sIb_6[i] = Ib_6[i] * (0.005) + 10 * 0.1;
	}


	//apro un solo canvas per tutto (potrei anche non aprirlo, lo fa in default il multigrafico)
	TCanvas * cIbIc = new TCanvas("cIbIc", "grafico1", 200, 10, 600, 400);
	cIbIc->SetFillColor(0);
	cIbIc->cd();

	//multigrafico
	TMultiGraph * mg = new TMultiGraph();
	mg->SetTitle("I_{c} vs I_{b}, V_{ce} costante");
	mg->GetXaxis()->SetTitle("I_{b} (#muA)");
	mg->GetYaxis()->SetTitle("I_{c} (mA)");
	mg->GetXaxis()->SetLimits(0, 13);
	mg->GetYaxis()->SetLimits(0, 230);

	//grafico a Vce = 3(V)
	TGraphErrors * gIbIc_3 = new TGraphErrors(nmisure, Ib_3, Ic_3, sIb_3, sIc_3);
	gIbIc_3->SetMarkerSize(0.1);
	gIbIc_3->SetMarkerStyle(21);
	gIbIc_3->SetLineColor(2);
	mg->Add(gIbIc_3);

	//grafico a Vce = 5(V)
	TGraphErrors * gIbIc_5 = new TGraphErrors(nmisure, Ib_5, Ic_5, sIb_5, sIc_5);
	gIbIc_5->SetMarkerSize(0.1);
	gIbIc_5->SetMarkerStyle(21);
	gIbIc_5->SetLineColor(3);
	mg->Add(gIbIc_5);

	//grafico a Vce = 6(V)
	TGraphErrors * gIbIc_6 = new TGraphErrors(nmisure, Ib_6, Ic_6, sIb_6, sIc_6);
	gIbIc_6->SetMarkerSize(0.1);
	gIbIc_6->SetMarkerStyle(21);
	gIbIc_6->SetLineColor(4);
	mg->Add(gIbIc_6);

	mg->Draw("AP");
}

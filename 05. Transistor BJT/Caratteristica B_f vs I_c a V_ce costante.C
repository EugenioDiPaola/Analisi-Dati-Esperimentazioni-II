#include <iostream>
#include <TGraphErrors.h>
#include <TAxis.h>      
#include <TCanvas.h>    
#include <TF1.h>        
#include <iomanip>      

void Bf_vs_Ic_Vce_costante()
{
	std::cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££" << endl;

	const int nmisure = 4;

	//Vce = 3(V)
	//Ic(mA) e Ib(µA) misurate con Amprobe 37XR-A
	float Ic_3[] = {44,88,130,160};
	float Ib_3[] = {100,200,300,400};

	float sIc_3[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		sIc_3[i] = Ic_3[i] * (5 / 1000) + 5 * 0.1;
	}
	float sIb_3[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		sIb_3[i] = Ib_3[i] * (5 / 1000) + 10 * 0.1;
	}

	//Ic_3/Ib_3 
	float beta_3[nmisure];
	for(int i = 0; i < nmisure; i++)
	{
		beta_3[i] = Ic_3[i] / Ib_3[i] * 1000;
	}//per 1000 perchè Ib è in µA e Ic in mA

//errore propagato
	float sbeta_3[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		sbeta_3[i] = sqrt((pow(Ic_3[i], 2) / pow(Ib_3[i], 4)) * pow(sIb_3[i], 2) + (1 / pow(Ib_3[i], 2)) * pow(sIc_3[i], 2)) * 1000;
	}

	//Vce = 5(V)
		//Ic(mA) e Ib(µA) misurate con Amprobe 37XR-A
	float Ic_5[] = {47,94,140,170};
	float Ib_5[] = {100,200,300,400};

	//
	//errori datasheet Amprobe 37XR-A
	float sIc_5[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		sIc_5[i] = Ic_5[i] * (5 / 1000) + 5 * 0.1;
	}

	//
	//errori datasheet Amprobe 37XR-A
	float sIb_5[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		sIb_5[i] = Ib_5[i] * (5 / 1000) + 10 * 0.1;
	}

	//Ic_5/Ib_5    
	float beta_5[nmisure];
	for(int i = 0; i < nmisure; i++)
	{
		beta_5[i] = Ic_5[i] / Ib_5[i] * 1000;
	}

	//errore propagato    
	float sbeta_5[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		sbeta_5[i] = sqrt((pow(Ic_5[i], 2) / pow(Ib_5[i], 4)) * pow(sIb_5[i], 2) + (1 / pow(Ib_5[i], 2)) * pow(sIc_5[i], 2)) * 1000;
	}


	//Vce = 6(V)
		//Ic(mA) e Ib(µA) misurate con Amprobe 37XR-A
	float Ic_6[] = {49,98,140,180};
	float Ib_6[] = {100,200,300,400};

	//errori datasheet Amprobe 37XR-A
	float sIc_6[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		sIc_6[i] = Ic_6[i] * (5 / 1000) + 5 * 0.1;
	}
	float sIb_6[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		sIb_6[i] = Ib_6[i] * (5 / 1000) + 10 * 0.1;
	}

	//Ic_6/Ib_6
	float beta_6[nmisure];
	for(int i = 0; i < nmisure; i++)
	{
		beta_6[i] = Ic_6[i] / Ib_6[i] * 1000;
	}

	//errore propagato
	float sbeta_6[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		sbeta_6[i] = sqrt((pow(Ic_6[i], 2) / pow(Ib_6[i], 4)) * pow(sIb_6[i], 2) + (1 / pow(Ib_6[i], 2)) * pow(sIc_6[i], 2)) * 1000;
	}



	TCanvas * cIcbeta = new TCanvas("cIcbeta", "grafico1", 0, 0, 1280, 720);
	cIcbeta->SetFillColor(0);
	cIcbeta->cd();

	TMultiGraph * mg = new TMultiGraph();
	mg->SetTitle("#scale[1]{#beta_{f} vs I_{c}, V_{ce} costante}       #color[2]{#bullet} V_{ce} = 3 (V)   #color[3]{#bullet} V_{ce} = 5 (V)   #color[4]{#bullet} V_{ce}= 6 (V)");
	mg->GetYaxis()->SetTitle("#beta_{f} (adim)");
	mg->GetXaxis()->SetTitle("I_{c} (mA)");
	mg->GetXaxis()->SetLimits(0, 13);
	mg->GetYaxis()->SetLimits(0, 230);

	TGraphErrors * gIcbeta_3 = new TGraphErrors(nmisure, Ic_3, beta_3, sIc_3, sbeta_3);
	gIcbeta_3->SetMarkerSize(0.4);
	gIcbeta_3->SetMarkerStyle(20);
	gIcbeta_3->SetLineColor(2);

	TGraphErrors * gIcbeta_5 = new TGraphErrors(nmisure, Ic_5, beta_5, sIc_5, sbeta_5);
	gIcbeta_5->SetMarkerSize(0.4);
	gIcbeta_5->SetMarkerStyle(20);
	gIcbeta_5->SetLineColor(3);

	TGraphErrors * gIcbeta_6 = new TGraphErrors(nmisure, Ic_6, beta_6, sIc_6, sbeta_6);
	gIcbeta_6->SetMarkerSize(0.4);
	gIcbeta_6->SetMarkerStyle(20);
	gIcbeta_6->SetLineColor(4);

	mg->Add(gIcbeta_3);
	mg->Add(gIcbeta_5);
	mg->Add(gIcbeta_6);


	mg->Draw("ALP");
}

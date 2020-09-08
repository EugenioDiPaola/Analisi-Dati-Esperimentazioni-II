/ #include <iostream>
#include <TGraphErrors.h>     
#include <TAxis.h>          
#include <TCanvas.h>        
#include <TF1.h>        
#include <iomanip> 
#include <cmath>
#include <math.h>

#include "TVirtualFitter.h"  
#include <TMultiGraph.h> 
#include <TGraph.h> 
#include <TLegend.h>

void Ic_vs_Vce_Ib_costante()
{
	std::cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££" << endl;

	std::cout << fixed;

	gStyle->SetOptTitle(1);
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1111);
	gStyle->SetStatBorderSize(1);

	const int nmisure = 24;//per fortuna il numero di misure è lo stesso per tutti le 4 curve a Ib costante


//dati a Ib = 100 (μA) ------------------------------------------------------------------------------------------------------------------------------------

	//V  
	//Amprobe 37XR-A
	float Vce_100[] = {0.502,1.004,1.497,2.008,2.496,2.997,3.493,4.001,4.504,5.000,5.497,5.999,6.500,7.000,7.499,8.001,8.501,8.994,9.504,10.00,10.49,11.00,11.49,12.00};
	float sVce_100[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		if(Vce_100[i] < 10) sVce_100[i] = Vce_100[i] * 0.001 + 5 * 0.001;
		if(Vce_100[i] >= 10) sVce_100[i] = Vce_100[i] * 0.001 + 5 * 0.01;
	}

	//mA
	//Ic misurata con Amprobe 37XR-A
	float Ic_100[] = {40,41,42,43,44,44,45,46,47,47,48,49,50,50,50,52,52,54,54,56,56,56,58,58};
	//errori datasheet Amprobe 37XR-A
	float sIc_100[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		sIc_100[i] = Ic_100[i] * 0.005 + 5 * 0.1;
	}



	//dati a Ib = 200 (μA) ----------------------------------------------------------------------------------------------------------------------------------

		//V  
		//Amprobe 37XR-A
	float Vce_200[] = {0.500,1.000,1.500,2.001,2.500,3.001,3.500,3.997,4.500,5.000,5.499,6.003,6.500,7.003,7.498,8.000,8.504,9.001,9.499,10.00,10.50,11.00,11.50,12.00};
	//errori datasheet Amprobe 37XR-A
	float sVce_200[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		if(Vce_200[i] < 10) sVce_200[i] = Vce_200[i] * 0.001 + 5 * 0.001;
		if(Vce_200[i] >= 10) sVce_200[i] = Vce_200[i] * 0.001 + 5 * 0.01;
	}

	//mA
	//Amprobe 37XR-A
	float Ic_200[] = {82,82,84,86,86,88,90,92,94,94,96,98,98,100,100,100,100,110,110,110,110,110,110,120};
	//errori datasheet Amprobe 37XR-A
	float sIc_200[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		sIc_200[i] = Ic_200[i] * 0.005 + 5 * 0.1;
	}


	//dati a Ib = 300 (μA) --------------------------------------------------------------------------------------------------------------------

		//V  
		//Vce misurata con Amprobe 37XR-A
	float Vce_300[] = {0.502,0.999,1.501,2.001,2.499,2.998,3.500,4.003,4.498,5.001,5.497,6.003,6.498,7.005,7.498,7.999,8.501,9.001,9.500,10.00,10.50,11.00,11.50,12.00};
	float sVce_300[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		if(Vce_300[i] < 10) sVce_300[i] = Vce_300[i] * 0.001 + 5 * 0.001;
		if(Vce_300[i] >= 10) sVce_300[i] = Vce_300[i] * 0.001 + 5 * 0.01;
	}

	//mA
	//Amprobe 37XR-A
	float Ic_300[] = {110,120,120,120,120,130,130,130,130,140,140,140,140,150,150,150,150,160,160,160,160,170,170,170};
	//errori datasheet Amprobe 37XR-A
	float sIc_300[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		sIc_300[i] = Ic_300[i] * 0.005 + 5 * 0.1;
	}


	//dati a Ib = 400 (μA) --------------------------------------------------------------------------------------------------------------------

		//V  
		//Amprobe 37XR-A
	float Vce_400[] = {0.500,0.998,1.501,1.998,2.500,3.004,3.501,4.000,4.501,5.003,5.501,6.005,6.502,6.998,7.505,8.003,8.498,9.007,9.52,10.00,10.50,11.00,11.50,12.00};
	//errori datasheet Amprobe 37XR-A
	float sVce_400[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		if(Vce_400[i] < 10) sVce_400[i] = Vce_400[i] * 0.001 + 5 * 0.001;
		if(Vce_400[i] >= 10) sVce_400[i] = Vce_400[i] * 0.001 + 5 * 0.01;
	}

	//mA
	//Amprobe 37XR-A
	float Ic_400[] = {150,160,160,160,160,160,170,170,170,170,180,180,180,180,190,190,190,200,200,200,200,210,210,210};
	//errori datasheet Amprobe 37XR-A
	float sIc_400[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		sIc_400[i] = Ic_400[i] * 0.005 + 5 * 0.1;
	}


	//metto un po di punti di della curva Ic * Vce = 1 (W) che poi fitterò per ottenere poi graficamente la curva di massima potenza 
	float Vmax[] = {4.500,5.000,5.500,6.000,6.500,7.000,7.500,8.000,8.500,9.000,9.500,10.000,10.500,11.000,11.500,12.000};
	float Imax[] = {222,200,181,166,153,142,133,125,117,111,105,100,95,90,86,83};
	int nmisure2 = 16;


	//creo un solo canvas per tutti i fit e grafici
	TCanvas * cIcVce = new TCanvas("cIcVce", "grafico1", 0, 0, 1280, 720);
	cIcVce->SetFillColor(0);
	cIcVce->cd();

	//multigrafico
	TMultiGraph * mg = new TMultiGraph();
	mg->SetTitle("#scale[0.8]{I_{c} vs V_{ce},  I_{b} costante}        #scale[0.5]{#splitline{#color[2]{#bullet} I_{b}^{ }=^{ }100^{ }(#muA)}{#color[3]{#bullet} I_{b}^{ }=^{ }200^{ }(#muA)}}      #scale[0.5]{#splitline{#color[4]{#bullet} I_{b}^{ }=^{ }300^{ }(#muA)}{#color[5]{#bullet} I_{b}^{ }=^{ }400^{ }(#muA)}}      #scale[0.5]{#splitline{#color[7]{#bullet} V_{ce} #upoint I_{c} = 1 (W) }{ }} ");
	mg->GetXaxis()->SetTitle("V_{ce} (V)");
	mg->GetYaxis()->SetTitle("I_{c} (mA)");

	//zero, lo metto per far si che gli assi comincino entrambi da zero
	float xzero[] = {0,0};
	float sxzero[] = {0,0};
	float yzero[] = {0,0};
	float syzero[] = {0,0};
	TGraphErrors * gzero = new TGraphErrors(2, xzero, yzero, sxzero, syzero);
	mg->Add(gzero);

	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________________________________________________________FIT A Ib = 100(microA)_______________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

	//fit dati a Ib = 100 (μA)
	TGraphErrors * gVceIc_100_fit = new TGraphErrors(nmisure, Vce_100, Ic_100, sVce_100, sIc_100);
	gVceIc_100_fit->SetMarkerSize(0.4);
	gVceIc_100_fit->SetMarkerStyle(20);
	gVceIc_100_fit->SetLineColor(1);

	TF1 * funz_100 = new TF1("funz_100", "[0] * 0.1 * (1 + x / [1])");
	funz_100->SetParNames("#scale[0.8]{#color[2]{#beta_{f} (adim)}}", "#scale[0.8]{#color[2]{V_{A} (V)}}");
	funz_100->SetLineColor(2);
	funz_100->SetParameters(0, 400);
	funz_100->SetParameters(0, 26.6);
	gVceIc_100_fit->Fit(funz_100, "M");
	std::cout << "Chi^2:" << funz_100->GetChisquare() << ", number of DoF: " << funz_100->GetNDF() << " (Probability: " << funz_100->GetProb() << ")." << endl;
	std::cout << "-----------------------------------------------------------------------------------------------------------------------------------" << endl;
	mg->Add(gVceIc_100_fit);

	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________________________________________________________FIT A Ib = 200(microA)_______________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;


	//fit dati a Ib = 200 (μA)
	TGraphErrors * gVceIc_200_fit = new TGraphErrors(nmisure, Vce_200, Ic_200, sVce_200, sIc_200);
	gVceIc_200_fit->SetMarkerSize(0.4);
	gVceIc_200_fit->SetMarkerStyle(20);
	gVceIc_200_fit->SetLineColor(1);

	TF1 * funz_200 = new TF1("funz_200", "[0] * 0.2 * (1 + x / [1])");
	funz_200->SetParNames("#scale[0.8]{#color[3]{#beta_{f} (adim)}}", "#scale[0.8]{#color[3]{V_{A} (V)}}");
	funz_200->SetLineColor(3);
	funz_200->SetParameters(0, 400);
	funz_200->SetParameters(0, 26.6);
	gVceIc_200_fit->Fit(funz_200, "M");
	std::cout << "Chi^2:" << funz_200->GetChisquare() << ", number of DoF: " << funz_200->GetNDF() << " (Probability: " << funz_200->GetProb() << ")." << endl;
	std::cout << "-----------------------------------------------------------------------------------------------------------------------------------" << endl;
	mg->Add(gVceIc_200_fit);


	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________________________________________________________FIT A Ib = 300(microA)_______________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;


	//fit dati a Ib = 300 (μA)
	TGraphErrors * gVceIc_300_fit = new TGraphErrors(nmisure, Vce_300, Ic_300, sVce_300, sIc_300);
	gVceIc_300_fit->SetMarkerSize(0.4);
	gVceIc_300_fit->SetMarkerStyle(20);
	gVceIc_300_fit->SetLineColor(1);


	TF1 * funz_300 = new TF1("funz_300", "[0] * 0.3 * (1 + x / [1])");
	funz_300->SetParNames("#scale[0.8]{#color[4]{#beta_{f} (adim)}}", "#scale[0.8]{#color[4]{V_{A} (V)}}");
	funz_300->SetLineColor(4);
	funz_300->SetParameters(0, 400);
	funz_300->SetParameters(0, 26.6);
	gVceIc_300_fit->Fit(funz_300, "M");
	std::cout << "Chi^2:" << funz_300->GetChisquare() << ", number of DoF: " << funz_300->GetNDF() << " (Probability: " << funz_300->GetProb() << ")." << endl;
	std::cout << "-----------------------------------------------------------------------------------------------------------------------------------" << endl;
	mg->Add(gVceIc_300_fit);

	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________________________________________________________FIT A Ib = 400(microA)_______________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;


	//fit dati a Ib = 400 (μA)
	TGraphErrors * gVceIc_400_fit = new TGraphErrors(nmisure, Vce_400, Ic_400, sVce_400, sIc_400);
	gVceIc_400_fit->SetMarkerSize(0.4);
	gVceIc_400_fit->SetMarkerStyle(20);
	gVceIc_400_fit->SetLineColor(1);

	TF1 * funz_400 = new TF1("funz_400", "[0] * 0.4 * (1 + x / [1])");
	funz_400->SetParNames("#scale[0.8]{#color[5]{#beta_{f} (adim)}}", "#scale[0.8]{#color[5]{V_{A} (V)}}");
	funz_400->SetLineColor(5);
	funz_400->SetParameters(0, 400);
	funz_400->SetParameters(0, 26.6);
	gVceIc_400_fit->Fit(funz_400, "M");
	std::cout << "Chi^2:" << funz_400->GetChisquare() << ", number of DoF: " << funz_400->GetNDF() << " (Probability: " << funz_400->GetProb() << ")." << endl;
	std::cout << "-----------------------------------------------------------------------------------------------------------------------------------" << endl;
	mg->Add(gVceIc_400_fit);


	//grafico della potenza massima a 1W
	TGraphErrors * gPmax = new TGraphErrors(nmisure2, Vmax, Imax, 0, 0);
	gPmax->SetLineColor(6);
	gPmax->SetMarkerSize(0.1);//1
	gPmax->SetMarkerStyle(20);//3 star
	gPmax->SetLineColor(2);
	gPmax->Draw("L");// non funziona, le options del multigrafico overridano quelle del Tgraph in esso contenuto; ci dovrei smadonnare un po di più, non ho tempo

	TF1 * funzPmax = new TF1("funzPmax", "1000 *[0] / x");//[0] è un parametro fittizio, è pari a 1, mi serve solo perchè altrimenti non fitta.
	funzPmax->SetLineColor(7);
	gPmax->Fit(funzPmax);


	mg->Add(gPmax);
	//cIcVce->SaveAs("cIcVce.png");// non funziona per il multigrafico


	for(int j = 0; j < nmisure; j++)
	{
		std::cout << "Measurement number " << j << ":"
			<< " \t Vce_100 = (" << setprecision(5) << Vce_100[j] << "+-" << setprecision(5) << sVce_100[j] << ") V,"
			<< " \t Vce_200 = (" << setprecision(5) << Vce_200[j] << "+-" << setprecision(5) << sVce_200[j] << ") V,"
			<< " \t Vce_300 = (" << setprecision(5) << Vce_300[j] << "+-" << setprecision(5) << sVce_300[j] << ") V,"
			<< " \t Vce_400 = (" << setprecision(5) << Vce_400[j] << "+-" << setprecision(5) << sVce_400[j] << ") V,"
			<< " \t Ic_100 = (" << setprecision(5) << Ic_100[j] << "+-" << setprecision(5) << sIc_100[j] << ") V,"
			<< " \t Ic_200 = (" << setprecision(5) << Ic_200[j] << "+-" << setprecision(5) << sIc_200[j] << ") V,"
			<< " \t Ic_300 = (" << setprecision(5) << Ic_300[j] << "+-" << setprecision(5) << sIc_300[j] << ") V,"
			<< " \t Ic_400 = (" << setprecision(5) << Ic_400[j] << "+-" << setprecision(5) << sIc_400[j] << ") V,"
			<< endl;
	}


	mg->Draw("AP");


	//__________________________________________________________________________________________________________________________________________________________________________________________________________________________
	std::ofstream out("IC_VS_VCE_IB_COSTANTE.csv");

	out << "Vce_100 (V)" << endl;
	for(auto & value : Vce_100)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << "Vce_200 (V)" << endl;
	for(auto & value : Vce_200)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << "Vce_300 (V)" << endl;
	for(auto & value : Vce_300)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << "Vce_400 (V)" << endl;
	for(auto & value : Vce_400)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}


	out << "sVce_100 (V)" << endl;
	for(auto & value : sVce_100)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << "sVce_200 (V)" << endl;
	for(auto & value : sVce_200)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << "sVce_300 (V)" << endl;
	for(auto & value : sVce_300)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << "sVce_400 (V)" << endl;
	for(auto & value : sVce_400)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}


	out << "Ic_100 (mA)" << endl;
	for(auto & value : Ic_100)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << "Ic_200 (mA)" << endl;
	for(auto & value : Ic_200)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << "Ic_300 (mA)" << endl;
	for(auto & value : Ic_300)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << "Ic_400 (mA)" << endl;
	for(auto & value : Ic_400)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}


	out << "sIc_100 (mA)" << endl;
	for(auto & value : sIc_100)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << "sIc_200 (mA)" << endl;
	for(auto & value : sIc_200)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << "sIc_300 (mA)" << endl;
	for(auto & value : sIc_300)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << "sIc_400 (mA)" << endl;
	for(auto & value : sIc_400)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
}

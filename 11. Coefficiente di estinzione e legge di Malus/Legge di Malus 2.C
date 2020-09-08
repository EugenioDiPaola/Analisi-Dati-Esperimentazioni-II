#include <iostream>           
#include <TGraphErrors.h>     
#include <TAxis.h>            
#include <TCanvas.h>          
#include <TF1.h>              
#include <iomanip>           
#include <cmath>
#include <math.h>

void malus()
{

	std::cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££" << endl;

	std::cout << fixed;

	gStyle->SetOptFit(1111);
	gStyle->SetOptTitle(1);
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1111);
	gStyle->SetStatBorderSize(0);
	gStyle->SetStatX(0.895);
	gStyle->SetStatY(0.87);
	gStyle->SetStatW(0.12);
	gStyle->SetStatH(0.12);


	std::cout << "_____________________________________________________________________________________________________________________________________________________________________________________________________________ _____________________________" << endl;
	std::cout << "_________________________________________________________________________________________________________________LEGGE DI MALUS____________________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;



	const int nmisure_malus = 36;

	const float PI = 3.1415926535;

	//°
	//scala graduata del filtro analizzatore
	float theta[nmisure_malus];
	for(int i = 0; i < nmisure_malus; ++i)
	{
		theta[i] = 10 * i + 10;
	}

	//errore di sensibilità
	float s_theta[nmisure_malus];
	for(int i = 0; i < nmisure_malus; i++)
	{
		s_theta[i] = 1;
	}

	float theta_rad[nmisure_malus];
	for(int i = 0; i < nmisure_malus; i++)
	{
		theta_rad[i] = theta[i] * PI / 180;
	}
	float s_theta_rad[nmisure_malus];
	for(int i = 0; i < nmisure_malus; i++)
	{
		s_theta_rad[i] = s_theta[i] * PI / 180;
	}


	//nA
	//HP 34401A
	float i_theta[] = {18.8, 20.4, 20.9, 20.3, 18.1, 15, 11.6, 8.3, 5, 2.3, 0.6, 0.1, 1, 3.1, 6.1, 9.5, 13.3, 16.9, 19.3, 20.9, 21, 19.7, 18.2, 16.2, 12.8, 9.2, 5.2, 2.4, 0.5, 0.1, 1, 3, 5.9, 9.6, 13.8, 17.1};    //errore sull'ultima cifra che non oscilla immediatamente
	float s_i_theta[nmisure_malus];
	for(int i = 0; i < nmisure_malus; ++i)
	{
		s_i_theta[i] = 0.1;
	}

	TCanvas * c_theta_i_theta = new TCanvas("c_theta_i_theta", "grafico legge malus", 0, 0, 1280, 720);
	c_theta_i_theta->SetFillColor(0);
	c_theta_i_theta->cd();

	TGraphErrors * g_theta_i_theta = new TGraphErrors(nmisure_malus, theta_rad, i_theta, s_theta_rad, s_i_theta);
	g_theta_i_theta->SetMarkerSize(0.4);
	g_theta_i_theta->SetMarkerStyle(20);
	g_theta_i_theta->SetTitle("#scale[0.85]{Funzione di fit:    i_{#theta} = i_{#theta}_{0} #upoint cos^{2}(#theta + #phi)}");
	g_theta_i_theta->GetXaxis()->SetTitle("#theta (°)");
	g_theta_i_theta->GetYaxis()->SetTitle("i_{#theta} (nA)");
	g_theta_i_theta->GetXaxis()->SetLimits(0, *std::max_element(std::begin(theta_rad), std::end(theta_rad)) + *std::max_element(std::begin(theta_rad), std::end(theta_rad)) / 10);

	g_theta_i_theta->Draw("AP");

	std::cout << "\n\n --- Ipotesi  [0] * cos(x - [1]) * cos(x - [1]) --- \n" << endl;
	TF1 * fz_theta_i_theta = new TF1("fz_theta_i_theta", "[0] * cos(x - [1]) * cos(x - [1])", *std::min_element(std::begin(theta_rad), std::end(theta_rad)), *std::max_element(std::begin(theta_rad), std::end(theta_rad)));
	fz_theta_i_theta->SetParNames("i_{#theta}_{0} (nA)", "#phi (rad)");

	float guess_par0_malus = 49.51;
	float guess_par1_malus = 0.679;

	fz_theta_i_theta->SetParameter(0, guess_par0_malus);
	fz_theta_i_theta->SetParameter(1, guess_par1_malus);

	//fz_theta_i_theta->SetParLimits(0, guess_par0_malus - guess_par0_malus / 10, guess_par0_malus + guess_par0_malus / 10);            
	//fz_theta_i_theta->SetParLimits(1, guess_par1_malus - guess_par1_malus / 10, guess_par1_malus + guess_par1_malus / 10); 

	fz_theta_i_theta->SetLineColor(4);
	g_theta_i_theta->Fit(fz_theta_i_theta, "ERM+");

	std::cout << "Chi^2:" << fz_theta_i_theta->GetChisquare() << ", number of DoF: " << fz_theta_i_theta->GetNDF() << " (Probability: " << fz_theta_i_theta->GetProb() << ")." << endl;
	std::cout << "-----------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
}

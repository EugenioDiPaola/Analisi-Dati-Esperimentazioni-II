#include <iostream>           
#include <TGraphErrors.h>     
#include <TAxis.h>            
#include <TCanvas.h>          
#include <TF1.h>              
#include <iomanip>           
#include <cmath>
#include <math.h>

void rlc()
{
	std::cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££" << endl;

	std::cout << fixed;

	gStyle->SetOptFit(1111);
	gStyle->SetOptTitle(1);
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1111);
	gStyle->SetStatBorderSize(0);
	gStyle->SetStatX(0.481);
	gStyle->SetStatY(0.880);

	const int nmisure = 43;

	double z_critico = 1.96;
	std::cout << fixed;
	double PI = 3.141592;

	//resistenza interna del generatore
	//Ohm
	//dichiarata sull'output del generatore di funzioni, però non l'abbiamo misurata 

	//Ohm
	//misurato con il ponte RLC; 
	//del ponte RLC non abbiamo il datasheet con gli errori sui vari range quindi prendiamo sempre l'errore sull'ultima cifra
	float R = 155.13;
	float sR = 0.01;

	//H
	//ponte RLC
	float L = 2.489e-03;
	//errore sensibilità ponte RLC
	float sL = 0.001e-03;

	//F
	//ponte RLC
	float C = 11.75e-9;
	//errore sensibilità ponte RLC
	float sC = 0.01e-9;

	//Khz 
	//generatore di funzioni
	float f[] = {2.10,3.04,4.06,5.29,6.40,7.23,8.27,9.18,10.24,11.13,12.10,13.30,14.20,15.80,17.10,18.20,21.60,22.20,23.00,23.50,24.00,24.50,25.00,25.40,25.90,26.40,26.90,27.40,28.00,28.50,29.00,29.60,30.30,31.20,32.30,33.70,39.30,43.00,49.90,60.30,70.00,90.00,99.70};

	//V
	//menu misure oscilloscopio
	float Vi[] = {23.0,23.0,23.0,23,23,23,22.5,22.5,22,22,22,22,22,21.5,21.5,21.5,20,20,19,19,18,18,18,18,18,18,18,18,18,18,19,19,19,19,20,20,21,21,21,22,22,22,22};//V

	//V
	//menu misure oscilloscopio
	float Vo[] = {0.600,0.840,1.05,1.42,1.7,2.0,2.3,2.6,3.0,3.4,3.8,4.2,4.7,5.6,6.4,7.3,10.5,11.1,11.9,12.3,12.6,12.9,13.0,13.1,13,13,13,12.8,12.6,12.3,12.0,11.7,11.4,10.9,10.3,9.76,7.76,6.7,5.4,4.2,3.5,2.6,2.3};//V

	//kHz
	//errore di sensibilità generatore di funzioni
	float sf[] = {0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1};

	//V
	//errore sensibilità schermo oscilloscopio
	float sVi[nmisure];
	for(int i = 0; i < nmisure; i++)
	{
		sVi[i] = 1;
	};

	//V
	//errore sensibililtàs chermo oscilloscopio
	float sVo[] = {0.04,0.04,0.04,0.04,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.2,0.2,0.2,0.2,0.2,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.2,0.2,0.1,0.1,0.1};

	//adim
	float A[nmisure];

	//adim
	float sA[nmisure];
	for(int i = 0; i < nmisure; i++)
	{
		A[i] = Vo[i] / Vi[i];
		sA[i] = sqrt(1 / (pow(Vi[i], 2)) * pow(sVo[i], 2) + (pow(Vo[i], 2) / pow(Vi[i], 4)) * pow(sVi[i], 2));//adim
	};


	//converto f da kHz in Hz
	for(int i = 0; i < nmisure; i++)
	{
		f[i] = f[i] * 1000;
		sf[i] = sf[i] * 1000;
	}


	//R,L,C in Ohm,H,F => f_R_teorica in Hz
	float f_R_teorica = f_R_teorica = 1 / (2 * PI * sqrt(L * C));
	float sf_R_teorica = sqrt((pow(C, 2) / (pow(4 * PI, 2) * pow(C * L, 3)) * pow(sL, 2)) + (pow(L, 2) / (pow(4 * PI, 2) * pow(C * L, 3)) * pow(sC, 2)));

	for(int j = 0; j < nmisure; j++)
	{
		std::cout << "Measurement number " << j << ":"
			<< " \t f = (" << setprecision(4) << f[j] << "+-" << setprecision(4) << sf[j] << ") Hz,"
			<< " \t Vo = (" << setprecision(4) << Vo[j] << "+-" << setprecision(4) << sVo[j] << ") V,"
			<< " \t Vi = (" << setprecision(4) << Vi[j] << "+-" << setprecision(4) << sVi[j] << ") V,"
			<< " \t A = (" << setprecision(4) << A[j] << "+-" << setprecision(4) << sA[j] << ") adim."
			<< " \t f/sf = (" << setprecision(4) << f[j] << "+-" << setprecision(4) << sf[j] << ") Hz,"
			<< " \t Vo/sVo = (" << setprecision(4) << sVo[j] / Vo[j]
			<< " \t Vi/sVi = (" << setprecision(4) << sVi[j] / Vi[j]
			<< " \t sf/f = " << setprecision(4) << sf[j] / f[j]
			<< " \t sA/A = " << setprecision(4) << sA[j] / A[j]
			<< endl;
	}

	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "____________________________________________________________________________________________________________FIT CON R E C DA TROVARE_______________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;


	//primo fit con R e C ignoti, ma che devono stare dentro (valore misurato - errore, valore misurato + errore)
	TCanvas * cAf = new TCanvas("cAf", "grafico1", 0, 0, 1280, 720);
	cAf->SetFillColor(0);
	cAf->SetLogx();
	cAf->cd();

	TGraphErrors * gAf = new TGraphErrors(nmisure, f, A, sf, sA);
	gAf->SetMarkerSize(0.4);
	gAf->SetMarkerStyle(20);
	gAf->SetTitle("#scale[0.68]{Funzione di fit:    A = R / #sqrt{#left(R + R_{s}#right)^{2} + #left[ (f^{2} / f_{R}^{2}) - 1 #right]^{2} / (2#pifC)^{2} }}");
	gAf->GetXaxis()->SetTitle("f (Hz)");
	gAf->GetYaxis()->SetTitle("A (adim)");
	gAf->Draw("AP");

	std::cout << "\n\n --- Ipotesi  [2] * 1 / (sqrt(pow([2] + [0],2) + pow(((pow(x,2) / pow([1],2)-1)) / ( 2 * pi * x * [3]),2))) --- \n" << endl;

	TF1 * funz1 = new TF1("funz1", "[2] * 1 / (sqrt(pow([2] + [0],2) + pow(((pow(x,2) / pow([1],2)-1)) / ( 2 * pi * x * [3]),2)))", 0, 120 * 10e+3);
	funz1->SetParNames("R_{s} (#Omega)", "f_{R} (Hz)", "R (#Omega)", "C (F)");
	//funz1->SetParameter(0,?);//resistenza reale dell'induttanza
	funz1->SetParameter(1, f_R_teorica);//frequenza di risonanza teorica
	funz1->SetParameter(2, R);//R
	funz1->SetParameter(3, C);//C

	funz1->SetParLimits(1, f_R_teorica - sf_R_teorica, f_R_teorica + sf_R_teorica);
	funz1->SetParLimits(2, R - sR, R + sR);
	funz1->SetParLimits(3, C - sC, C + sC);

	funz1->SetLineColor(3);
	gAf->Fit(funz1, "RM+");//V converge con MIGRAD ricontrollare
	std::cout << "Chi^2:" << funz1->GetChisquare() << ", number of DoF: " << funz1->GetNDF() << " (Probability: " << funz1->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	cAf->SaveAs("cAf.png");

	//test z per confrontare la frequenza di risonanza teorica ottenuta dai valori misurati di L e C con quella sperimentale ricavata dal fit
	float f_R_sperimentale1 = funz1->GetParameter(1);
	float sf_R_sperimentale1 = funz1->GetParError(1);

	std::cout << setprecision(11)
		<< "R: (" << R << "+-" << sR << ")" << endl
		<< "C: (" << C << "+-" << sC << ")" << endl
		<< "L: (" << L << "+-" << sL << ")" << endl
		<< "frequenza di risonanza teorica: (" << f_R_teorica << "+-" << sf_R_teorica << ")" << endl
		<< "frequenza di risonanza sperimentale: (" << f_R_sperimentale1 << "+-" << sf_R_sperimentale1 << ")" << endl;

	float z1 = (f_R_teorica - f_R_sperimentale1) / sqrt(pow(sf_R_sperimentale1, 2) + pow(sf_R_teorica, 2));

	if(z1 <= z_critico) std::cout << "z: " << z1 << " < " << z_critico << endl;
	if(z1 > z_critico) std::cout << "z: " << z1 << " > " << z_critico << endl;


	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "________________________________________________________________________________________________________________FIT R E C FISSI____________________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

	//secondo fit con R e C considerati noti
	TCanvas * cAf2 = new TCanvas("cAf2", "grafico2", 0, 0, 1280, 720);
	cAf2->SetFillColor(0);
	cAf2->SetLogx();
	cAf2->cd();

	TGraphErrors * gAf2 = new TGraphErrors(nmisure, f, A, sf, sA);
	gAf2->SetMarkerSize(0.4);
	gAf2->SetMarkerStyle(20);
	gAf2->SetTitle("#scale[0.68]{Funzione di fit:    A = 155.13 (#Omega) / #sqrt{#left(155.13 (#Omega) + R_{s}#right)^{2} + #left[ (f^{2} / f_{R}^{2}) - 1 #right]^{2} / #left(2#pif #upoint 11.75 #upoint 10^{-9} (F)#right)^{2} }}");
	gAf2->GetXaxis()->SetTitle("f (Hz)");
	gAf2->GetYaxis()->SetTitle("A (adim)");
	gAf2->Draw("AP");

	//fit con R,C imposti e pari a quelli misurati (L non compare perchè f di risonanza dipende dal prodotto LC, dunque L e C qui sono correlati)
	std::cout << "\n\n --- Ipotesi  155.13 * 1 / (sqrt(pow(155.13 + [0],2) + pow(((pow(x,2) / pow([1],2) - 1)) / (2 * pi * x * 11.75e-9),2))) --- \n" << endl;

	TF1 * funz2 = new TF1("funz2", "155.13 * 1 / (sqrt(pow(155.13 + [0],2) + pow(((pow(x,2) / pow([1],2) - 1)) / (2 * pi * x * 11.75e-9),2)))", 0, 120 * 10e+3);
	funz2->SetParNames("R_{s} (#Omega)", "f_{R} (Hz)");
	//funz2->SetParameter(0,?);//resistenza reale dell'induttanza
	funz2->SetParameter(1, f_R_teorica);//f_R

	//funz2->SetParLimits(0,50,100);
	//funz2->SetParLimits(1,20e+3,30e+3);

	funz2->SetLineColor(4);
	gAf2->Fit(funz2, "ERM+");

	std::cout << "Chi^2:" << funz2->GetChisquare() << ", number of DoF: " << funz2->GetNDF() << " (Probability: " << funz2->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	cAf2->SaveAs("cAf2.png");

	//test z per confrontare la frequenza di risonanza teorica ottenuta dai valori misurati di L e C con quella sperimentale ricavata dal fit
	float f_R_sperimentale2 = funz2->GetParameter(1);
	float sf_R_sperimentale2 = funz2->GetParError(1);

	std::cout << setprecision(11)
		<< "R: (" << R << "+-" << sR << ")" << endl
		<< "C: (" << C << "+-" << sC << ")" << endl
		<< "L: (" << L << "+-" << sL << ")" << endl
		<< "frequenza di risonanza teorica: (" << f_R_teorica << "+-" << sf_R_teorica << ")" << endl
		<< "frequenza di risonanza sperimentale: (" << f_R_sperimentale2 << "+-" << sf_R_sperimentale2 << ")" << endl;

	float z2 = (f_R_teorica - f_R_sperimentale2) / sqrt(pow(sf_R_sperimentale2, 2) + pow(sf_R_teorica, 2));

	if(z2 <= z_critico) std::cout << "z: " << z2 << " < " << z_critico << endl;
	if(z2 > z_critico) std::cout << "z: " << z2 << " > " << z_critico << endl;



	//________________________________________________________________________________________________________________________________________________________________________________________________________
	std::ofstream out("circuitoRLC_valori_errori.csv");
	out << "f (kHz)" << endl;
	for(auto & value : f)
	{
		out << fixed << showpoint << setprecision(7) << value / 1000 << endl;
	}
	out << endl;

	out << "sf (kHz)" << endl;
	for(auto & value : sf)
	{
		out << fixed << showpoint << setprecision(7) << value / 1000 << endl;
	}
	out << endl;

	out << "Vo" << endl;
	for(auto & value : Vo)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "sVo" << endl;
	for(auto & value : sVo)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "Vi" << endl;
	for(auto & value : Vi)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "sVi" << endl;
	for(auto & value : sVi)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "A" << endl;
	for(auto & value : A)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "sA" << endl;
	for(auto & value : sA)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;
}

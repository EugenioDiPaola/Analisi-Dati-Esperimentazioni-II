#include <iostream>
#include <TGraphErrors.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <iomanip>

void amplificatore()
{
	std::cout << fixed;

	//resistenza ad alta potenza (una di quelle dorate)
	//Ohm
	float Rc = 46.23;
	float sRc = 0.005 * Rc + 8 * 1;

	//Ohm
	float Rb = 4712;
	float sRb = 0.005 * Rb + 8 * 1;

	std::cout << setprecision(7) << "Rc: (" << Rc << "+-" << sRc << ")" << endl;
	std::cout << setprecision(7) << "Rb: (" << Rb << "+-" << sRb << ")" << endl;
	std::cout << endl;


	//V = Vs * onda quadra[2pi * 1000 * t] ------------------------------------

	//Vs =  0.2 (V)
	//V
	float Delta_Vbe_02_q = 0.42;
	float Delta_Vce_02_q = 1.19;
	float sDelta_Vbe_02_q = 0.02;
	float sDelta_Vce_02_q = 0.04;

	//Vs = 0.3 (V)
	//V
	float Delta_Vbe_03_q = 0.60;
	float Delta_Vce_03_q = 1.74;
	float sDelta_Vbe_03_q = 0.02;
	float sDelta_Vce_03_q = 0.1;

	//Vs = 0.4 (V)
	//V
	float Delta_Vbe_04_q = 1;
	float Delta_Vce_04_q = 2.88;
	float sDelta_Vbe_04_q = 0.04;
	float sDelta_Vce_04_q = 0.1;


	//Beta0 per l'onda quadra
	float Beta0_02_q = (Rb / Rc) * (Delta_Vce_02_q / Delta_Vbe_02_q);
	float Beta0_03_q = (Rb / Rc) * (Delta_Vce_03_q / Delta_Vbe_03_q);
	float Beta0_04_q = (Rb / Rc) * (Delta_Vce_04_q / Delta_Vbe_04_q);

	//in realtà sti errori non servono
	float sBeta0_02_q = sqrt(pow(sDelta_Vce_02_q / (Rc * Delta_Vbe_02_q), 2) * pow(sRb, 2) + pow((Rb * Delta_Vce_02_q) / (Delta_Vbe_02_q * pow(Rc, 2)), 2) * pow(sRc, 2) + pow(Rb / (Rc * Delta_Vbe_02_q), 2) * pow(sDelta_Vce_02_q, 2) + pow((Rb * Delta_Vce_02_q) / (Rc * pow(Delta_Vbe_02_q, 2)), 2) * pow(sDelta_Vbe_02_q, 2));
	float sBeta0_03_q = sqrt(pow(sDelta_Vce_03_q / (Rc * Delta_Vbe_03_q), 2) * pow(sRb, 2) + pow((Rb * Delta_Vce_03_q) / (Delta_Vbe_03_q * pow(Rc, 2)), 2) * pow(sRc, 2) + pow(Rb / (Rc * Delta_Vbe_03_q), 2) * pow(sDelta_Vce_03_q, 2) + pow((Rb * Delta_Vce_03_q) / (Rc * pow(Delta_Vbe_03_q, 2)), 2) * pow(sDelta_Vbe_03_q, 2));
	float sBeta0_04_q = sqrt(pow(sDelta_Vce_04_q / (Rc * Delta_Vbe_04_q), 2) * pow(sRb, 2) + pow((Rb * Delta_Vce_04_q) / (Delta_Vbe_04_q * pow(Rc, 2)), 2) * pow(sRc, 2) + pow(Rb / (Rc * Delta_Vbe_04_q), 2) * pow(sDelta_Vce_04_q, 2) + pow((Rb * Delta_Vce_04_q) / (Rc * pow(Delta_Vbe_04_q, 2)), 2) * pow(sDelta_Vbe_04_q, 2));

	std::cout << "Beta0_02_q: (" << Beta0_02_q << "+-" << sBeta0_02_q << ")" << endl;
	std::cout << "Beta0_03_q: (" << Beta0_03_q << "+-" << sBeta0_03_q << ")" << endl;
	std::cout << "Beta0_04_q: (" << Beta0_04_q << "+-" << sBeta0_04_q << ")" << endl;

	//media e errore relativo (unbiased sample variance)
	float Beta0_q = (Beta0_02_q + Beta0_03_q + Beta0_04_q) / 3;
	float sBeta0_q = sqrt((pow(Beta0_02_q - Beta0_q, 2) + pow(Beta0_02_q - Beta0_q, 2) + pow(Beta0_02_q - Beta0_q, 2)) / (3 - 1));

	std::cout << "amplificazione per piccoli segnali Beta0 per l'onda quadra a 1 kHz: (" << Beta0_q << "+-" << sBeta0_q << ")" << "\n\n";



	//V = Vs * sin[2pi * 1000 * t] ------------------------------

	//Vs =  0.2 (V)
	//V
	float Delta_Vbe_02_s = 0.38;
	float Delta_Vce_02_s = 1.2;
	float sDelta_Vbe_02_s = 0.02;
	float sDelta_Vce_02_s = 0.01;

	//Vs = 0.3 (V)
	//V
	float Delta_Vbe_03_s = 0.56;
	float Delta_Vce_03_s = 1.7;
	float sDelta_Vbe_03_s = 0.02;
	float sDelta_Vce_03_s = 0.1;

	//Vs = 0.4 (V)
	//V
	float Delta_Vbe_04_s = 0.83;
	float Delta_Vce_04_s = 2.88;
	float sDelta_Vbe_04_s = 0.04;
	float sDelta_Vce_04_s = 0.04;

	float A_02_s = Delta_Vce_02_s / Delta_Vbe_02_s;
	float A_03_s = Delta_Vce_03_s / Delta_Vbe_03_s;
	float A_04_s = Delta_Vce_04_s / Delta_Vbe_04_s;

	float sA_02_s = sqrt((pow(Delta_Vce_02_s, 2) / pow(Delta_Vbe_02_s, 4)) * pow(sDelta_Vbe_02_s, 2) + (1 / pow(Delta_Vbe_02_s, 2)) * pow(sDelta_Vce_02_s, 2));
	float sA_03_s = sqrt((pow(Delta_Vce_03_s, 2) / pow(Delta_Vbe_03_s, 4)) * pow(sDelta_Vbe_03_s, 2) + (1 / pow(Delta_Vbe_03_s, 2)) * pow(sDelta_Vce_03_s, 2));
	float sA_04_s = sqrt((pow(Delta_Vce_04_s, 2) / pow(Delta_Vbe_04_s, 4)) * pow(sDelta_Vbe_04_s, 2) + (1 / pow(Delta_Vbe_04_s, 2)) * pow(sDelta_Vce_04_s, 2));

	std::cout << setprecision(7) << "A_02_s: (" << A_02_s << "+-" << sA_02_s << ")" << endl;
	std::cout << setprecision(7) << "A_03_s: (" << A_03_s << "+-" << sA_03_s << ")" << endl;
	std::cout << setprecision(7) << "A_04_s: (" << A_04_s << "+-" << sA_04_s << ")" << endl;

	float A_s = (A_02_s + A_03_s + A_04_s) / 3;
	float sA_s = sqrt((pow(A_02_s - A_s, 2) + pow(A_03_s - A_s, 2) + pow(A_04_s - A_s, 2)) / (3 - 1));

	float A_q = Beta0_q * (Rc / Rb);
	float sA_q = sqrt(pow(Rc / Rb, 2) * pow(sBeta0_q, 2) + pow(Beta0_q / Rb, 2) * pow(sRc, 2) + pow(Rc * Beta0_q / pow(Rb, 2), 2) * pow(sRb, 2));

	std::cout << "A_s a 1 kHz: (" << A_s << "+-" << sA_s << ")" << "\n\n";
	std::cout << "A_q a 1 Khz: (" << A_q << "+-" << sA_q << ")" << "\n\n";



	//test z 
	float z = (A_q - A_s) / sqrt(pow(sA_s, 2) + pow(sA_q, 2));
	float z_critico = 1.96;
	if(z <= z_critico) std::cout /*<< setprecision(3)*/ << "z: " << z << " < " << z_critico << endl;
	if(z > z_critico) std::cout /*<< setprecision(3)*/ << "z: " << z << " > " << z_critico << endl;

	/*
	//Beta0 per l'onda sinusoidale
	float Beta0_02_s = (Rb / Rc) * (Delta_Vce_02_s / Delta_Vbe_02_s);
	float Beta0_03_s = (Rb / Rc) * (Delta_Vce_03_s / Delta_Vbe_03_s);
	float Beta0_04_s = (Rb / Rc) * (Delta_Vce_04_s / Delta_Vbe_04_s);

	float sBeta0_02_s = sqrt(
							pow(sDelta_Vce_02_s / (Rc * Delta_Vbe_02_s),2) * pow(sRb,2) +
							pow((Rb * Delta_Vce_02_s) / (Delta_Vbe_02_s * pow(Rc,2)),2) * pow(sRc,2) +
							pow(Rb / (Rc * Delta_Vbe_02_s),2) * pow(sDelta_Vce_02_s,2) +
							pow((Rb * Delta_Vce_02_s) / (Rc * pow(Delta_Vbe_02_s,2)),2) * pow(sDelta_Vbe_02_q,2)
							);
	float sBeta0_03_s = sqrt(
							pow(sDelta_Vce_03_s / (Rc * Delta_Vbe_03_s),2) * pow(sRb,2) +
							pow((Rb * Delta_Vce_03_s) / (Delta_Vbe_03_q * pow(Rc,2)),2) * pow(sRc,2) +
							pow(Rb / (Rc * Delta_Vbe_03_s),2) * pow(sDelta_Vce_03_q,2) +
							pow((Rb * Delta_Vce_03_s) / (Rc * pow(Delta_Vbe_03_s,2)),2) * pow(sDelta_Vbe_03_s,2)
							);
	float sBeta0_04_s = sqrt(
							pow(sDelta_Vce_04_s / (Rc * Delta_Vbe_04_s),2) * pow(sRb,2) +
							pow((Rb * Delta_Vce_04_s) / (Delta_Vbe_04_s * pow(Rc,2)),2) * pow(sRc,2) +
							pow(Rb / (Rc * Delta_Vbe_04_s),2) * pow(sDelta_Vce_04_s,2) +
							pow((Rb * Delta_Vce_04_s) / (Rc * pow(Delta_Vbe_04_s,2)),2) * pow(sDelta_Vbe_04_s,2)
							);

	std::cout << "Beta0_02_s: (" << Beta0_02_s << "+-" << sBeta0_02_s << ")" << endl;
	std::cout << "Beta0_03_s: (" << Beta0_03_s << "+-" << sBeta0_03_s << ")" << endl;
	std::cout << "Beta0_04_s: (" << Beta0_04_s << "+-" << sBeta0_04_s << ")" << endl;

	//media e errore relativo (unbiased sample variance)
	float Beta0_s = (Beta0_02_s + Beta0_03_s + Beta0_04_q) / 3;
	float sBeta0_s = sqrt((pow(Beta0_02_s - Beta0_s,2) + pow(Beta0_02_s - Beta0_s,2) + pow(Beta0_02_s - Beta0_s,2)) / (3 - 1));

	std::cout << "amplificazione per piccoli segnali Beta0 per l'onda sinusoidale a 1 kHz: (" << Beta0_q << "+-" << sBeta0_q << ")" << "\n\n";
	*/




	//save

	const int nmisure = 34;

	//kHz
	//letti sul generatore di funzioni
	float f[] = {0.1,0.2,0.3,0.4,0.5,1,2.5,5,7.5,10,12.5,15,17.5,20,22.5,25,30,35,40,45,50,60,70,80,90,100,150,200,250,300,350,400,450,500};//34


	//V
	//misurati con il menu misure oscilloscopio; errore = scala/5
	float Vi[] = {0.984,0.992,0.992,0.992,0.992,1,1,1.02,1.02,1.02,1.02,1.02,1.02,1.01,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02};//34(5) 1.02
	float sVi[] = {0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04};//34

	//V
	//misurati con il menu misure oscilloscopio; errore = scala/5
	float Vo[] = {2.88,2.92,2.92,2.92,2.9,2.9,2.84,2.74,2.6,2.46,2.3,2.18,2.04,1.92,1.8,1.6,1.44,1.32,1.2,1.1,0.912,0.800,0.712,0.640,0.584,0.396,0.304,0.258,0.206,0.184,0.163,0.148,0.134,0.088};//34
	float sVo[] = {0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.04,0.04,0.04,0.04,0.04,0.02,0.01,0.01,0.01,0.01,0.01,0.01,0.004}; //34(5) 0.01

	//abbiamo sbagliato a segnare sti errori, i seguenti non si sa cosa sono, se errori sui tempi o l'inverso di essi. tuttavia ci danno un idea di quando abbiamo cambiato scala:
	//float sf[] = {0.002,0.002,0.002,0.002,0.002,0.2,0.2,0.2,0.2,0.2,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2};

	//simuliamo degli errori sensati sulle frequenze, come se avessimo cambiato scala ogni tot
	float sf[nmisure];
	for(int i = 0; i < 6; i++)
	{
		sf[i] = f[i] / 20;
	}
	for(int j = 6; j < 11; j++)
	{
		sf[j] = f[j] / 33.3;
	}
	for(int j = 10; j < 22; j++)
	{
		sf[j] = f[j] / 50;
	}
	for(int j = 22; j < 34; j++)
	{
		sf[j] = f[j] / 25;
	}


	float A[nmisure];
	float sA[nmisure];
	for(int j = 0; j < nmisure; j++)
	{

		f[j] = f[j] * 1000;// converto f da kHz in Hz
		sf[j] = sf[j] * 1000;// converto f da kHz in Hz

		A[j] = Vo[j] / Vi[j];
		sA[j] = sqrt((pow(Vo[j], 2) / pow(Vi[j], 4)) * pow(sVi[j], 2) + (1 / pow(Vi[j], 2)) * pow(sVo[j], 2));
	}

	//impostazioni grafiche
	gStyle->SetOptFit(1111);
	gStyle->SetOptTitle(1);
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1111);
	gStyle->SetStatBorderSize(0);
	gStyle->SetStatX(0.9);
	gStyle->SetStatY(0.880);

	for(int j = 0; j < nmisure; j++)
	{
		std::cout << "Measurement number " << j << ":"
			<< " \t f = (" << f[j] << " +- " << sf[j] << "\t) Hz,"
			<< " \t Vo = (" << Vo[j] << " +- " << sVo[j] << ") V,"
			<< " \t Vi = (" << Vi[j] << " +- " << sVi[j] << ") V,"
			<< " \t A = (" << A[j] << " +- " << sA[j] << ") adim."
			<< " \t sf/f = " << sf[j] / f[j] << " adim"
			<< " \t sA/A = " << sA[j] / A[j] << " adim"
			<< endl;
	}


	TCanvas * cAf = new TCanvas("cAf", "grafico1", 0, 0, 1280, 720);
	cAf->SetFillColor(0);
	cAf->SetLogx();
	cAf->cd();
	TGraphErrors * gAf = new TGraphErrors(nmisure, f, A, sf, sA);
	gAf->SetMarkerSize(0.6);
	gAf->SetMarkerStyle(21);
	gAf->SetTitle("#scale[0.7]{Funzione di fit:    A_{v} #left[1 / #sqrt{#left(1 + f^{2} / f_{T}^{2}#right)}#right]}");
	gAf->GetXaxis()->SetTitle("f (Hz)");
	gAf->GetYaxis()->SetTitle("A (adim)");
	gAf->Draw("AP");

	std::cout << "\n\n --- Ipotesi A = (Rb / Rc) * Bf = [1] * (1 / sqrt(1 + pow(x,2) / [0]))  ---  \n" << endl;

	TF1 * funz1 = new TF1("funz1", " [1] * (1 / sqrt(1 + pow(x,2) / [0]))", 0, 1e6);
	funz1->SetParNames("f_{T}^{2} (Hz^{2})", "A_{v} (adim)");
	funz1->SetParameter(0, 1.6e8);//faccio 2.943/sqrt(2) e poi vado a vedere più o meno a che frequenza ho quell'amplificazione sul grafico (anche se in realtà la legge non è esattamente quella del passa basso)
	funz1->SetParameter(1, 2.943e-2);//basta osservare i valori di A per vedere che l'amplificazione massima è 2.943

	funz1->SetLineColor(4);
	gAf->Fit(funz1, "ERM+");

	std::cout << "Chi^2:" << funz1->GetChisquare() << ", number of DoF: " << funz1->GetNDF() << " (Probability: " << funz1->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	cAf->SaveAs("cAf.png");


	//fit rogliendo l'outlier
	const int nmisure1 = 33;

	//kHz
	//letti sul generatore di funzioni
	float f1[] = {0.1,0.2,0.3,0.4,0.5,1,2.5,5,7.5,10,12.5,15,17.5,20,22.5,25,30,35,40,45,50,60,70,80,90,150,200,250,300,350,400,450,500};


	//V
	//misurati con il menu misure oscilloscopio; errore = scala/5
	float Vi1[] = {0.984,0.992,0.992,0.992,0.992,1,1,1.02,1.02,1.02,1.02,1.02,1.02,1.01,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02};
	float sVi1[] = {0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04,0.04};

	//V
	//misurati con il menu misure oscilloscopio; errore = scala/5
	float Vo1[] = {2.88,2.92,2.92,2.92,2.9,2.9,2.84,2.74,2.6,2.46,2.3,2.18,2.04,1.92,1.8,1.6,1.44,1.32,1.2,1.1,0.912,0.800,0.712,0.640,0.584,0.304,0.258,0.206,0.184,0.163,0.148,0.134,0.088};
	float sVo1[] = {0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.04,0.04,0.04,0.04,0.02,0.01,0.01,0.01,0.01,0.01,0.01,0.004};

	//abbiamo sbagliato a segnare sti errori, i seguenti non si sa cosa sono, se errori sui tempi o l'inverso di essi. tuttavia ci danno un idea di quando abbiamo cambiato scala:
	//float sf[] = {0.002,0.002,0.002,0.002,0.002,0.2,0.2,0.2,0.2,0.2,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2};

	//simuliamo degli errori sensati sulle frequenze, come se avessimo cambiato scala ogni tot
	float sf1[nmisure1];
	for(int i = 0; i < 6; i++)
	{
		sf1[i] = f1[i] / 20;
	}
	for(int j = 6; j < 11; j++)
	{
		sf1[j] = f1[j] / 33.3;
	}
	for(int j = 10; j < 22; j++)
	{
		sf1[j] = f1[j] / 50;
	}
	for(int j = 22; j < 33; j++)
	{
		sf1[j] = f1[j] / 25;
	}


	float A1[nmisure1];
	float sA1[nmisure1];
	for(int j = 0; j < nmisure1; j++)
	{

		f1[j] = f1[j] * 1000;// converto f da kHz in Hz
		sf1[j] = sf1[j] * 1000;// converto f da kHz in Hz

		A1[j] = Vo1[j] / Vi1[j];
		sA1[j] = sqrt((pow(Vo1[j], 2) / pow(Vi1[j], 4)) * pow(sVi1[j], 2) + (1 / pow(Vi1[j], 2)) * pow(sVo1[j], 2));
	}


	for(int j = 0; j < nmisure1; j++)
	{
		std::cout << "Measurement number " << j << ":"
			<< " \t f = (" << f1[j] << " +- " << sf1[j] << "\t) V,"
			<< " \t Vo = (" << Vo1[j] << " +- " << sVo1[j] << ") V,"
			<< " \t Vi = (" << Vi1[j] << " +- " << sVi1[j] << ") V,"
			<< " \t A = (" << A1[j] << " +- " << sA1[j] << ") adim."
			<< " \t sf/f = " << sf1[j] / f1[j] << " adim"
			<< " \t sA/A = " << sA1[j] / A1[j] << " adim"
			<< endl;
	}


	TCanvas * cAf1 = new TCanvas("cAf1", "grafico2", 0, 0, 1280, 720);
	cAf1->SetFillColor(0);
	cAf1->SetLogx();
	cAf1->cd();
	TGraphErrors * gAf1 = new TGraphErrors(nmisure1, f1, A1, sf1, sA1);
	gAf1->SetMarkerSize(0.6);
	gAf1->SetMarkerStyle(21);
	gAf1->SetTitle("#scale[0.7]{Funzione di fit:    A_{v} #left[1 / #sqrt{#left(1 + f^{2} / f_{T}^{2}#right)}#right]}");
	gAf1->GetXaxis()->SetTitle("f (Hz)");
	gAf1->GetYaxis()->SetTitle("A (adim)");
	gAf1->Draw("AP");

	std::cout << "\n\n --- Ipotesi A = (Rb / Rc) * Bf = [1] * (1 / sqrt(1 + pow(x,2) / [0]))  ---  \n" << endl;

	TF1 * funz2 = new TF1("funz1", " [1] * (1 / sqrt(1 + pow(x,2) / [0]))", 0, 1e6);
	funz2->SetParNames("f_{T}^{2} (Hz^{2})", "A_{v} (adim)");
	funz2->SetParameter(0, 1.6e8);//faccio 2.943/sqrt(2) e poi vado a vedere più o meno a che frequenza ho quell'amplificazione sul grafico (anche se in realtà la legge non è esattamente quella del passa basso)
	funz2->SetParameter(1, 2.943e-2);//basta osservare i valori di A per vedere che l'amplificazione massima è 2.943

	funz2->SetLineColor(4);
	gAf1->Fit(funz2, "ERM+");

	std::cout << "Chi^2:" << funz2->GetChisquare() << ", number of DoF: " << funz2->GetNDF() << " (Probability: " << funz2->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	cAf1->SaveAs("cAf1.png");


	std::ofstream out("aamplificatore_valori_errori.csv");
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

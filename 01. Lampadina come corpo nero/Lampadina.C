#include <iostream>           
#include <TGraphErrors.h>     
#include <TAxis.h>            
#include <TCanvas.h>          
#include <TF1.h>              
#include <iomanip>           
#include <cmath>

void lampadina()
{
	std::cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££" << endl;

	//alcuni dettagli e test normali sono fatti direttamente sulla relazione su Excel, non sto a riscriverli. qui ci sono principalmente i fit.

	//"By default ROOT creates a default style that can be accessed via the gStyle pointer". "gstyle inherits from TObject, mother of all objects in root"
	//crea in automatico gli oggetti più utili delle librerie ROOT con dei puntatori che hanno sempre gli stessi nomi (ad esempio gstyle per tutte le classi relative a stili dei grafici)

	//impostazioni grafiche
	gStyle->SetOptFit(1111);
	gStyle->SetOptTitle(1);
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1111);
	gStyle->SetStatBorderSize(0);
	gStyle->SetStatX(0.5);
	gStyle->SetStatY(0.85);

	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "_______________________________________________________________________________________________RESISTENZA INTERNA DI UN VOLTMETRO ANALOGICO_______________________________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

	//kOhm
	//misurato con ponte RLC
	float R1 = 487.9;

	//kOhm
	//errore di sensibilità del ponte RLC
	float sR1 = 0.1;

	//convertiamo in Ohm
	R1 = R1 * 1e3;
	sR1 = sR1 * 1e3;

	//V
	//voltmetro analogico
	float Vbg_analog = 1.875;

	//V
	//errore di sensibilità del voltmetro analogico per il fondo scale 10 V DC: 10 (V) / 80 (tacche) * 1 (tacca) = 0.125 (V)
	float sVbg_analog = 0.125;

	//V
	//voltmetro analogico
	float Vag_analog = 5;

	//V
	//errore di sensibilità del voltmetro analogico per il fondo scale 10 V DC: 10 (V) / 80 (tacche) * 1 (tacca) = 0.125 (V)
	float sVag_analog = 0.125;

	//Ohm
	float Rvolt_analog = (R1 * Vbg_analog) / (Vag_analog - Vbg_analog);

	//Ohm
	//errore propagato
	float sRvolt_analog = sqrt(pow(Vbg_analog / (Vag_analog - Vbg_analog), 2) * pow(sR1, 2) + pow(R1 * (Vag_analog - Vbg_analog) + Vbg_analog * R1, 2) / pow(Vag_analog - Vbg_analog, 4) * pow(sVbg_analog, 2) + pow(R1 * Vbg_analog, 2) / pow(Vbg_analog - Vag_analog, 4) * pow(sVag_analog, 2));


	std::cout << "Vbg_analog: (" << setprecision(7) << Vbg_analog << "+-" << sVbg_analog << ")" << "\n";
	std::cout << "Vag_analog: (" << setprecision(7) << Vag_analog << "+-" << sVag_analog << ")" << "\n";
	std::cout << "Rvolt_analog: (" << setprecision(7) << Rvolt_analog << "+-" << sRvolt_analog << ")" << "\n";
	std::cout << "sRvolt_analog / Rvolt_analog: (" << setprecision(7) << sRvolt_analog / Rvolt_analog << ")" << "\n";
	std::cout << endl;


	std::cout << "_________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "_______________________________________________________________________________________________RESISTENZA INTERNA DI UN VOLTMETRO DIGITALE_______________________________________________________________________________________________" << endl;
	std::cout << "_________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

	//V
	//Amprobe 37XR-A
	float Vbg_dig = 7.600;

	//V
	//datasheet Amprobe 37XR-A
	float sVbg_dig = Vbg_dig * 0.001 + 5 * 0.001;

	//V
	//Amprobe 37XR-A
	float Vag_dig = 8.002;

	//V
	//datasheet Amprobe 37XR-A
	float sVag_dig = Vag_dig * 0.001 + 5 * 0.001;

	//Ohm
	float Rvolt_dig = (R1 * Vbg_dig) / (Vag_dig - Vbg_dig);

	//Ohm
	//errore propagato
	float sRvolt_dig = sqrt(pow(Vbg_dig / (Vag_dig - Vbg_dig), 2) * pow(sR1, 2) + pow(R1 * (Vag_dig - Vbg_dig) + Vbg_dig * R1, 2) / pow(Vag_dig - Vbg_dig, 4) * pow(sVbg_dig, 2) + pow(R1 * Vbg_dig, 2) / pow(Vbg_dig - Vag_dig, 4) * pow(sVag_dig, 2));


	std::cout << "Vbg_dig: (" << setprecision(7) << Vbg_dig << "+-" << sVbg_dig << ")" << "\n";
	std::cout << "Vag_dig: (" << setprecision(7) << Vag_dig << "+-" << sVag_dig << ")" << "\n";
	std::cout << "Rvolt_dig: (" << setprecision(7) << Rvolt_dig << "+-" << sRvolt_dig << ")" << "\n";
	std::cout << "sRvolt_dig / Rvolt_dig: (" << setprecision(7) << sRvolt_dig / Rvolt_dig << ")" << "\n";
	std::cout << endl;


	std::cout << "____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "_______________________________________________________________________________________________RESISTENZA INTERNA DI UN AMPEROMETRO ANALOGICO_______________________________________________________________________________________________" << endl;
	std::cout << "____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

	//Ohm
	//ponte RLC
	float R2 = 260.2;

	//Ohm
	//errore sensibilità ponte RLC
	float sR2 = 0.1;

	//V
	//
	float Vvolt_dig = 0.2681;
	float sVvolt_dig = Vvolt_dig * 0.001 + 5 * 0.0001;

	//mA
	float iamp_analog = 25.625;
	//50 mA / 80 tacche* 1 tacca = 5/8 mA
	float siamp_analog = 50.000 / 80.000 * 1;
	//A
	iamp_analog = iamp_analog / 1000.00;
	siamp_analog = siamp_analog / 1000.00;

	//Ohm
	float Ramp_analog = Vvolt_dig / iamp_analog;
	float sRamp_analog = sqrt((pow(Vvolt_dig, 2) / pow(iamp_analog, 4)) * pow(siamp_analog, 2) + (1 / pow(iamp_analog, 2)) * pow(sVvolt_dig, 2));

	std::cout << "Vvolt_dig: (" << setprecision(7) << Vvolt_dig << "+-" << sVvolt_dig << ")" << endl;
	std::cout << "iamp_analog: (" << setprecision(7) << iamp_analog << "+-" << siamp_analog << ")" << endl;
	std::cout << "Ramp_analog: (" << setprecision(7) << Ramp_analog << "+-" << sRamp_analog << ")" << endl;
	std::cout << "sRamp_analog / Ramp_analog: " << setprecision(7) << sRamp_analog / Ramp_analog << "\n";
	std::cout << endl;


	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "_______________________________________________________________________________________________RESISTENZA INTERNA DI UN AMPEROMETRO DIGITALE______________________________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

	//non l'abbiamo fatto

	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________CARATTERISTICA TENSIONE CORRENTE DI UNA LAMPADINA E LEGGE DI CORPO NERO__________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

	double z_critico = 1.96;

	//Ohm
	const float R20 = 12.3;
	const float sR20 = 0.01 * 12.3 + 0.1; //sto errore non so quanto sia corretto, perchè nei datasheet del multimetro digitale 
										//non c'è scritto l'errore su questo range. ma tanto non compare da nessuna parte sto R20
										//c'era già nella macro della lampadina presa dal materiale didattico
	const int nmisure = 24;

	double q_teorica = 3.75;
	double sq_teorica = 0;

	//V
	//letto sul display a 4 cifre del generatore di tensione DC; tanto l'errore su questa misura non mi serve;
	float Vgen[] = {0.51,0.99,1.49,2,2.48,3,3.51,4,4.48,5,5.5,6,6.48,7,7.5,8.01,8.49,9.02,9.48,10.01,10.47,11,11.5,12.01};
	float sVgen[nmisure];
	for(int i = 0; i < nmisure; ++i)
	{
		sVgen[i] = 0.01;
	}

	//V
	//misurato con Amprobe 37XR-A
	float V[] = {0.3602,0.7718,1.207,1.653,2.090,2.557,3.021,3.469,3.918,4.395,4.860,5.329,5.781,6.264,6.785,7.212,7.677,8.182,8.622,9.138,9.584,10.08,10.57,11.06};

	//mA
	//misurato con Amprobe 37XR-A
	float i[] = {13.43,19.99,25.56,30.51,34.92,39.24,43.20,46.78,50.19,53.66,56.89,59.99,62.92,65.92,68.76,71.57,74.22,77.03,79.41,82.15,84.76,87.53,89.96,92.38};

	float sV[nmisure];
	float si[nmisure];

	float R[nmisure];
	float sR[nmisure];

	float P[nmisure];
	float sP[nmisure];

	for(int j = 0; j < nmisure; ++j)
	{//Amprobe 37XR-A
	//V
		if(V[j] < 1)               sV[j] = V[j] * 0.001 + 5 * 0.0001;
		if(V[j] >= 1 && V[j] < 10) sV[j] = V[j] * 0.001 + 5 * 0.001;
		if(V[j] >= 10)             sV[j] = V[j] * 0.001 + 5 * 0.01;

		si[j] = i[j] * 0.005 + 5 * 0.01;

		//Ohm
		//se il numero V[j] / i[j] è in KOhm, allora il numero V[j] / i[j] * 1000 è in Ohm. stesso discorso per l'errore;
		R[j] = (V[j] / i[j]) * 1000;
		sR[j] = sqrt((pow(V[j], 2) / pow(i[j], 4)) * pow(si[j], 2) + (1 / pow(i[j], 2)) * pow(sV[j], 2)) * 1000;

		//mW
		P[j] = V[j] * i[j];
		sP[j] = sqrt(pow(V[j], 2) * pow(si[j], 2) + pow(i[j], 2) * pow(sV[j], 2));
	}

	//setprecision(n) setta tutti valori successivi nello stream con precisione alla n-esima cifra 
	for(int j = 0; j < nmisure; j++)
	{
		std::cout << "Misure numero " << j << ":"
			<< " \t V = (" << setprecision(5) << V[j] << "+-" << setprecision(5) << sV[j] << ") V,"
			<< " \t i = (" << setprecision(5) << i[j] << "+-" << setprecision(5) << si[j] << ") mA,"
			<< " \t R = (" << setprecision(5) << R[j] << "+-" << setprecision(5) << sR[j] << ") Ohm,"
			<< " \t P = (" << setprecision(5) << P[j] << "+-" << setprecision(5) << sP[j] << ") mW."
			<< " \t sV/V = " << setprecision(3) << sV[j] / V[j]
			<< " \t si/i = " << setprecision(3) << si[j] / i[j]
			<< " \t sR/R = " << setprecision(3) << sR[j] / R[j]
			<< " \t sP/P = " << setprecision(3) << sP[j] / P[j]
			<< endl;
	}

	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "________________________________________________________________________________________________________________GRAFICI DEI DATI___________________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

	//grafico i(V)
	TCanvas * ciV = new TCanvas("ciV1", "grafico 1", 0, 0, 1280, 720);
	ciV->SetFillColor(0);
	ciV->cd();

	TGraphErrors * giV = new TGraphErrors(nmisure, V, i, sV, si);
	giV->SetMarkerSize(0.4);
	giV->SetMarkerStyle(20);
	giV->SetTitle("i vs V");
	giV->GetXaxis()->SetTitle("V (V)");
	giV->GetYaxis()->SetTitle("i (mA)");
	giV->Draw("AP");

	//grafico P(R)
	TCanvas * cPR = new TCanvas("cPR1", "grafico 2", 0, 0, 1280, 720);
	cPR->SetFillColor(0);
	cPR->cd();
	TGraphErrors * gPR = new TGraphErrors(nmisure, R, P, sR, sP);
	gPR->SetMarkerSize(1);
	gPR->SetMarkerStyle(1);
	gPR->SetTitle("P vs R");
	gPR->GetXaxis()->SetTitle("R (#Omega)");
	gPR->GetYaxis()->SetTitle("P (mW)");
	gPR->Draw("AP");


	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________FIT P = aR^4_____________________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

	//fit P = aR^4
	TCanvas * cPR2 = new TCanvas("cPR2", "grafico 3", 0, 0, 1280, 720);
	cPR2->SetFillColor(0);
	cPR2->cd();

	TGraphErrors * gPR2 = new TGraphErrors(nmisure, R, P, sR, sP);
	gPR2->SetMarkerSize(0.4);
	gPR2->SetMarkerStyle(20);
	gPR2->SetTitle("#scale[0.8]{Funzione di fit:    P^{ }=^{ }p_{0}R^{4}}");
	gPR2->GetXaxis()->SetTitle("R (#Omega)");
	gPR2->GetYaxis()->SetTitle("P (mW)");
	gPR2->Draw("AP");

	std::cout << "\n\n --- Ipotesi  [0] * pow(x,4) --- \n" << endl;
	TF1 * funz2 = new TF1("funz2", "[0] * pow(x,4)", 0, 130);
	funz2->SetParNames("p_{0}(mW #Omega^{-4})");
	funz2->SetParameter(0, 5e-06);
	//funz2->SetParLimits(0,4.9e-06,5.1e-06); //con fitpanel->set parameters->fisso p0 = 5.0e-06 sembra graficamente migliore a occhio il fit ma il chi^2 è più alto. non serve fissare un range.
	funz2->SetLineColor(3);
	gPR2->Fit(funz2, "ERM+");

	std::cout << "Chi^2:" << funz2->GetChisquare() << ", number of DoF: " << funz2->GetNDF() << " (Probability: " << funz2->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	cPR2->SaveAs("LAMPADINA 1.png");


	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "_______________________________________________________________________________________________________________FIT P = mR^q________________________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;


	//fit P = mR^q
	TCanvas * cPR3 = new TCanvas("cPR3", "grafico 4", 0, 0, 1280, 720);
	cPR3->SetFillColor(0);
	cPR3->cd();

	TGraphErrors * gPR3 = new TGraphErrors(nmisure, R, P, sR, sP);
	gPR3->SetMarkerSize(0.4);
	gPR3->SetMarkerStyle(20);
	gPR3->SetTitle("#scale[0.95]{Funzione di fit:    P^{ }=^{ }mR^{q}}");
	gPR3->GetXaxis()->SetTitle("R (#Omega)");
	gPR3->GetYaxis()->SetTitle("P (mW)");
	gPR3->Draw("AP");

	std::cout << "\n\n --- Ipotesi  [0] * pow(x,[1]) --- \n" << endl;
	TF1 * funz3 = new TF1("funz3", "[0] * pow(x,[1])", 0, 130);
	funz3->SetParNames("m^{ }(mW #Omega^{-q})", "q^{ }(adim)");
	funz3->SetParameter(0, 0.00002398);
	funz3->SetParameter(1, 3.659);
	funz3->SetParLimits(0, 0.000020, 0.000025);
	funz3->SetParLimits(1, 3.65, 3.66);
	funz3->SetLineColor(4);
	gPR3->Fit(funz3, "VERM+");//V

	std::cout << "Chi^2:" << funz3->GetChisquare() << ", number of DoF: " << funz3->GetNDF() << " (Probability: " << funz3->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	cPR3->SaveAs("LAMPADINA 2.png");

	//test Z 

	double q_sperimentale3 = funz3->GetParameter(1);
	double sq_sperimentale3 = funz3->GetParError(1);

	std::cout << "q teorica: (" << q_teorica << "+-" << sq_teorica << ")" << endl;
	std::cout << "q sperimentale: (" << q_sperimentale3 << "+-" << sq_sperimentale3 << ")" << endl;

	double z3 = fabs(q_teorica - q_sperimentale3) / sqrt(pow(sq_sperimentale3, 2) + pow(sq_teorica, 2));

	std::cout << endl;
	if(z3 <= z_critico) std::cout << setprecision(5) << "z: " << z3 << " < " << z_critico << endl;
	if(z3 > z_critico) std::cout << setprecision(5) << "z: " << z3 << " > " << z_critico << endl;


	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________________________________________________________FIT P = P_4(R)_______________________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;


	//fit polinomiale di quarto grado
	TCanvas * cPR4 = new TCanvas("cPR4", "grafico 5", 0, 0, 1280, 720);
	cPR4->SetFillColor(0);
	cPR4->cd();

	TGraphErrors * gPR4 = new TGraphErrors(nmisure, R, P, sR, sP);
	gPR4->SetMarkerSize(0.4);
	gPR4->SetMarkerStyle(20);
	gPR4->SetTitle("#scale[0.80]{Funzione di fit:    P^{ }=^{ }p_{0}^{ }+^{ }p_{1}R^{ }+^{ }p_{2}R^{2}^{ }+^{ }p_{3}R^{3}^{ }+^{ }p_{4}R^{4}}");
	gPR4->GetXaxis()->SetTitle("R (#Omega)");
	gPR4->GetYaxis()->SetTitle("P (mW)");
	gPR4->Draw("AP");

	std::cout << "\n\n --- Ipotesi  [0] + [1] * x + [2] * pow(x,2) + [3] * pow(x,3) + [4]*pow(x,4) --- \n" << endl;
	TF1 * funz4 = new TF1("funz4", "[0] + [1] * x + [2] * pow(x,2) + [3] * pow(x,3) + [4] * pow(x,4)", 0, 130);
	funz4->SetLineColor(5);
	funz4->SetParNames("p_{0} (mW)", "p_{1} (mW #Omega^{-1})", "p_{2} (mW #Omega^{-2})", "p_{3} (mW #Omega^{-3})", "p_{4} (mW #Omega^{-4})");
	gPR4->Fit(funz4, "VRM+");// "V" *Option_t che sta per 'verbose', cioè mi stampa in maniera dettagliata tutto il processo di fitting. in questo caso HESSE sembra fallire ma MIGRAD funziona.

	std::cout << "Chi^2:" << funz4->GetChisquare() << ", number of DoF: " << funz4->GetNDF() << " (Probability: " << funz4->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	cPR4->SaveAs("LAMPADINA 3.png");


	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________________________________________________________FIT P = a(R-b)^4_____________________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

	//fit P = a(R-b)^4
	TCanvas * cPR5 = new TCanvas("cPR5", "grafico 6", 0, 0, 1280, 720);
	cPR5->SetFillColor(0);
	cPR5->cd();

	TGraphErrors * gPR5 = new TGraphErrors(nmisure, R, P, sR, sP);
	gPR5->SetMarkerSize(0.4);
	gPR5->SetMarkerStyle(20);
	gPR5->SetTitle("#scale[0.8]{Funzione di fit:    P^{ }=^{ }p_{0}(R^{ }-^{ }p_{1})^{4}}");
	gPR5->GetXaxis()->SetTitle("R (#Omega)");
	gPR5->GetYaxis()->SetTitle("P (mW)");
	gPR5->Draw("AP");

	std::cout << "\n\n --- Ipotesi  [0]*pow(x-[1],4) --- \n" << endl;
	TF1 * funz5 = new TF1("funz5", "[0]*pow(x-[1],4)", 0, 130);
	funz5->SetParNames("p_{0} (mW #Omega^{-4})", "p_{1} (#Omega)");
	funz5->SetLineColor(6);
	gPR5->Fit(funz5, "ERM+");

	std::cout << "Chi^2:" << funz5->GetChisquare() << ", number of DoF: " << funz5->GetNDF() << " (Probability: " << funz5->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	cPR5->SaveAs("LAMPADINA 4.png");


	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________FIT I = sV^(q-1/q+1)____________________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;


	//fit I = sV^(q-1/q+1)
	TCanvas * ciV2 = new TCanvas("ciV2", "grafico 7", 0, 0, 1280, 720);
	ciV2->SetFillColor(0);
	ciV2->cd();

	TGraphErrors * giV2 = new TGraphErrors(nmisure, V, i, sV, si);
	giV2->SetMarkerSize(0.4);
	giV2->SetMarkerStyle(20);
	giV2->SetTitle("#scale[0.95]{Funzione di fit:    i^{ }=^{ }sV^{(q-1)/(q+1)}}");
	giV2->GetXaxis()->SetTitle("V (V)");
	giV2->GetYaxis()->SetTitle("i (mA)");
	giV2->Draw("AP");

	std::cout << "\n\n --- Ipotesi  [0] * pow(x,([1] - 1) / ([1] + 1)) --- \n" << endl;
	TF1 * funz6 = new TF1("funz6", "[0] * pow(x,([1] - 1) / ([1] + 1))", 0, 15);
	funz6->SetParNames("s #left(mA V^{- #frac{q-1}{q+1}}#right)", "q (adim)");
	funz6->SetParameter(0, 1);
	funz6->SetParameter(1, 1);
	//funz6->SetParameterLimits(0,1,10);
	//funz6->SetParameterLimits(0,1,10);
	funz6->SetLineColor(7);
	giV2->Fit(funz6, "ERM+");

	std::cout << "Chi^2:" << funz6->GetChisquare() << ", number of DoF: " << funz6->GetNDF() << " (Probability: " << funz6->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	ciV2->SaveAs("LAMPADINA 5.png");


	//test Z 

	double q_sperimentale6 = funz6->GetParameter(1);
	double sq_sperimentale6 = funz6->GetParError(1);

	std::cout << "q teorica: (" << q_teorica << "+-" << sq_teorica << ")" << endl;
	std::cout << "q sperimentale: (" << q_sperimentale6 << "+-" << sq_sperimentale6 << ")" << endl;

	double z6 = fabs(q_teorica - q_sperimentale6) / sqrt(pow(sq_sperimentale6, 2) + pow(sq_teorica, 2));

	std::cout << endl;
	if(z6 <= z_critico) std::cout << setprecision(5) << "z: " << z6 << " < " << z_critico << endl;
	if(z6 > z_critico) std::cout << setprecision(5) << "z: " << z6 << " > " << z_critico << endl;

	//__________________________________________________________________________________________________________________________________________________________________________________________________________________________
	//valori ed errori su csv
	std::ofstream out("LAMPADINA_CSV.csv");
	out << "Vgen (V)" << endl;
	for(auto & value : Vgen)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "sVgen (V)" << endl;
	for(auto & value : sVgen)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "V (V)" << endl;
	for(auto & value : V)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "sV (V)" << endl;
	for(auto & value : sV)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "i (mA)" << endl;
	for(auto & value : i)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "si (mA)" << endl;
	for(auto & value : si)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "R (Ohm)" << endl;
	for(auto & value : R)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "sR (Ohm)" << endl;
	for(auto & value : sR)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "P (mW)" << endl;
	for(auto & value : P)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "sP (mW)" << endl;
	for(auto & value : sP)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

}

#include <iostream>
#include <TGraphErrors.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <iomanip>

void rcpassaalto()
{
	std::cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££" << endl;

	gStyle->SetOptFit(1111);
	gStyle->SetOptTitle(1);
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1111);
	gStyle->SetStatBorderSize(0);
	gStyle->SetStatX(0.481);
	gStyle->SetStatY(0.880);

	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________________________________________________________________DATI_________________________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

	std::cout << fixed;

	const float PI = 3.141592;

	const int nmisure = 28;

	//Ohm
	//ponte RLC
	float R = 61.36e+3;
	float sR = R * 0.005 + 8 * 0.001; //Amprobe 37XR-A 1000 Ohm to 1000 kOhm ranges: ± (0.5 % rdg + 8 dgts)

	//F
	//sensibilità ponte RLC
	float C = 2.7e-9;
	float sC = 0.05 * C; //tolleranza scritta sul corpo del condensatore;

	//Hz
	//lette sull'oscilloscopio nel menu misure
	float f[] = {101,200,300,400,500,600,706,806,849,902,920,941,960,980,1007,1019,1038,1066,1079,1102,1209,1496,2020,3023,6100,9000,15170,32000};

	//Hz
	//ci siamo dimenticati di prendere l'errore sul periodo sul menu misure dell'oscilloscopio (da cui ricavare l'errore sulle frequenze); 
	//ipotizziamo un errore dell'1%:
	float sf[nmisure];
	for(int j = 0; j < nmisure; j++)
	{
		sf[j] = f[j] / 100;
	}

	//V
	//menu misure oscilloscopio
	float Vo[] = {0.608,1.18,1.72,2.22,2.68,3.06,3.42,3.68,3.80,3.82,3.98,4.00,4.08,4.12,4.16,4.16,4.20,4.24,4.28,4.32,4.48,4.80,5.20,5.44,5.76,5.80,5.84,5.84};//lette sull'oscilloscopio nel menu misure

	//V
	//menu misure oscilloscopio
	float Vi[] = {5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92,5.92};

	//V
	//scala dell'oscilloscopio sulle ampiezze: un tacca divisa 5
	float sVi[] = {0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4};

	//V
	//scala dell'oscilloscopio sulle ampiezze: un tacca divisa 5
	float sVo[] = {0.02,0.04,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1};

	//adim
	float A[nmisure];
	float sA[nmisure];
	for(int j = 0; j < nmisure; j++)
	{
		A[j] = Vo[j] / Vi[j];
		sA[j] = sqrt((pow(Vo[j], 2) / pow(Vi[j], 4)) * pow(sVi[j], 2) + (1 / pow(Vi[j], 2)) * pow(sVo[j], 2));
	};

	//Hz
	float f_T_teorica = 1 / (2 * PI * R * C);

	//errore propagato dagli errori di R e C
	float sf_T_teorica = 1 / (2 * PI) * sqrt(1 / (pow(R, 4) * pow(C, 2)) * pow(sR, 2) + 1 / (pow(R, 2) * pow(C, 4)) * pow(sC, 2));

	for(int j = 0; j < nmisure; j++)
	{
		std::cout << "Measurement number " << j << ":"
			<< " \t f = (" << setprecision(5) << f[j] << " +- " << setprecision(4) << sf[j] << ") Hz,"
			<< " \t Vo = (" << setprecision(5) << Vo[j] << " +- " << setprecision(4) << sVo[j] << ") V,"
			<< " \t Vi = (" << setprecision(5) << Vi[j] << " +- " << setprecision(4) << sVi[j] << ") V,"
			<< " \t A = (" << setprecision(5) << A[j] << " +- " << setprecision(4) << sA[j] << ") adim."
			<< " \t f/sf " << setprecision(4) << f[j]
			<< " \t Vo/sVo " << setprecision(4) << sVo[j] / Vo[j]
			<< " \t Vi/sVi " << setprecision(4) << sVi[j] / Vi[j]
			<< " \t sf/f " << setprecision(4) << sf[j] / f[j]
			<< " \t sA/A " << setprecision(4) << sA[j] / A[j]
			<< endl;
	}

	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "____________________________________________________________________________________________________________FIT CON ERRORI 0.01*f SU f_____________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

	//primo fit, con errore sulle frequenze stimato all'1%
	TCanvas * cAf1 = new TCanvas("cAf1", "grafico1", 0, 0, 1280, 720);
	cAf1->SetFillColor(0);
	cAf1->SetLogx();
	cAf1->cd();

	TGraphErrors * gAf1 = new TGraphErrors(nmisure, f, A, sf, sA);
	gAf1->SetMarkerSize(0.3);
	gAf1->SetMarkerStyle(20);
	gAf1->SetTitle("#scale[0.85]{Funzione di fit:    A^{ }=^{ }1 / #sqrt{1 + f_{T}^{2} / f^{2}}}");
	gAf1->GetXaxis()->SetTitle("f^{ }(Hz)");
	gAf1->GetYaxis()->SetTitle("A^{ }(adim)");
	gAf1->Draw("AP");

	std::cout << " \n\n ----- Ipotesi 1 / sqrt(1 + [0] / pow(x,2)) ------ \n " << endl;
	TF1 * funz1 = new TF1("funz1", "1 / sqrt(1 + [0] / pow(x,2))", 0, 400000);
	funz1->SetParNames("f_{T}^{2} (Hz^{2})");
	funz1->SetParameter(0, pow(f_T_teorica, 2));
	funz1->SetLineColor(2);
	gAf1->Fit(funz1, "ERM+");

	std::cout << "Chi^2:" << funz1->GetChisquare() << ", number of DoF: " << funz1->GetNDF() << " (Probability: " << funz1->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	cAf1->SaveAs("FILTRORCPASSAALTO 1.png");

	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "_________________________________________________________________________________________________________FIT CON ERRORI PIU' PICCOLI SU f__________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

	//dato che il chi quadro del fit precedente è molto piccolo potremmo aver sovrastimato gli errori; 
	//dato che quello su f lo abbiamo scelto noi proviamo a rifare il fit con un errore su f dieci volte più piccolo
	//anche con errori più piccoli dell'1% su f il chiquadro non cambia

	//Hz
	float sf2[nmisure];
	for(int j = 0; j < nmisure; j++)
	{
		sf2[j] = f[j] / 1e4;
	}

	//con errori più piccoli su f
	TCanvas * cAf2 = new TCanvas("cAf2", "grafico2", 0, 0, 1280, 720);
	cAf2->SetFillColor(0);
	cAf2->SetLogx();
	cAf2->cd();

	TGraphErrors * gAf2 = new TGraphErrors(nmisure, f, A, sf2, sA);
	gAf2->SetMarkerSize(0.4);
	gAf2->SetMarkerStyle(20);
	gAf2->SetTitle("#scale[0.85]{Funzione di fit:    A^{ }=^{ }1 / #sqrt{1 + f_{T}^{2} / f^{2}}}");
	gAf2->GetXaxis()->SetTitle("f^{ }(Hz)");
	gAf2->GetYaxis()->SetTitle("A^{ }(adim)");
	gAf2->Draw("AP");

	std::cout << " \n\n ----- Ipotesi 1 / sqrt(1 + [0] / pow(x,2)) ------ \n " << endl;
	TF1 * funz2 = new TF1("funz1", "1 / sqrt(1 + [0] / pow(x,2))", 0, 400000);
	funz2->SetParNames("f_{T}^{2} (Hz^{2})");
	funz2->SetParameter(0, pow(f_T_teorica, 2));
	funz2->SetLineColor(3);
	gAf2->Fit(funz2, "ERM+");

	std::cout << "Chi^2:" << funz2->GetChisquare() << ", number of DoF: " << funz2->GetNDF() << " (Probability: " << funz2->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
	cAf2->SaveAs("FILTRORCPASSAALTO 2.png");

	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "_____________________________________________________________________________________________________________________TEST Z________________________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;

	float p0 = funz1->GetParameter(0);
	float sp0 = funz1->GetParError(0);

	//[0] è il quadrato della frequenza di taglio sperimentale
	float f_T_sperimentale = sqrt(p0);
	float sf_T_sperimentale = sp0 / (2 * f_T_sperimentale);//errore propagato


	std::cout << "R: (" << R << "+-" << sR << ")" << endl;
	std::cout << "C: (" << setprecision(13) << C << "+-" << sC << ")" << endl;
	std::cout << "frequenza di taglio teorica: (" << f_T_teorica << "+-" << sf_T_teorica << ")" << endl;
	std::cout << "frequenza di taglio sperimentale: (" << f_T_sperimentale << "+-" << sf_T_sperimentale << ")" << endl;

	float z = (f_T_teorica - f_T_sperimentale) / sqrt(pow(sf_T_sperimentale, 2) + pow(sf_T_teorica, 2));
	float z_critico = 1.96;
	if(z <= z_critico) std::cout << setprecision(3) << "z: " << z << " < " << z_critico << endl;
	if(z > z_critico) std::cout << setprecision(3) << "z: " << z << " > " << z_critico << endl;

	//__________________________________________________________________________________________________________________________________________________________________________________________________________________________
	std::ofstream out("FILTRORCPASSAALTO_CSV.csv");
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

	out << "Vo (V)" << endl;
	
	for(auto & value : Vo)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "sVo (V)" << endl;
	
	for(auto & value : sVo)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "Vi (V)" << endl;
	
	for(auto & value : Vi)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "sVi (V)" << endl;
	
	for(auto & value : sVi)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "A (adim)" << endl;
	
	for(auto & value : A)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;

	out << "sA (adim)" << endl;
	
	for(auto & value : sA)
	{
		out << fixed << showpoint << setprecision(7) << value << endl;
	}
	out << endl;
}


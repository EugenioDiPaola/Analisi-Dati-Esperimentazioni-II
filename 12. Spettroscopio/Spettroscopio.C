#include <iostream>           
#include <TGraphErrors.h>     
#include <TAxis.h>            
#include <TCanvas.h>          
#include <TF1.h>              
#include <iomanip>           
#include <cmath>
#include <math.h>



void spettroscopio()
{
	std::cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££" << endl;

	gStyle->SetOptFit();
	gStyle->SetStatBorderSize(0);
	gStyle->SetStatX(0.9);
	gStyle->SetStatY(0.87);

	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________LEGGE DI CAUCHY AL PRIMO ORDINE CON LAMBDA DALLA DEIFFRAZIONE CON RETICOLO______________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________" << endl;


	float lambda[] = {435, 543, 572, 575, 616, 563, 578, 615};
	float s_lambda[] = {2, 2, 1, 1, 2, 3, 0, 1};

	float n[] = {1.6408, 1.6231, 1.6199, 1.6197, 1.6187, 1.6226, 1.6211, 1.6187};//{1.6408, 1.6231, 1.6199, 1.6197, 1.6187, 1.6226, 1.6211, 1.6187};
	float s_n[] = {0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001};

	vector<float> lambda_v = vector<float>(begin(lambda), end(lambda));

	TGraphErrors * g_n_lambda = new TGraphErrors(lambda_v.size(), lambda_v.data(), n, s_lambda, s_n);
	g_n_lambda->SetMarkerSize(0.4);
	g_n_lambda->SetMarkerStyle(20);
	g_n_lambda->SetTitle("#scale[0.55]{Funzione di fit:    n_{prisma} = A + #frac{B}{#lambda^{2}_{reticolo}}}");
	g_n_lambda->GetXaxis()->SetTitle("#lambda_{reticolo} (nm)");
	g_n_lambda->GetYaxis()->SetTitle("n_{prisma} (adim)");


	std::cout << "\n\n --- Ipotesi   y = [0] + [1] / x * x --- \n" << endl;
	TF1 * fz_n_lambda = new TF1("fz_n_lambda", "[0] + [1] / pow(x,2)", *min_element(begin(lambda_v), end(lambda_v)), *max_element(begin(lambda_v), end(lambda_v)));
	fz_n_lambda->SetParNames("A (adim)", "B (nm^{2})");
	fz_n_lambda->SetParameter(0, 1.5966);
	fz_n_lambda->SetParameter(1, 8368.7);
	//https://www.wolframalpha.com/input/?i=equation+system&assumption=%22FSelect%22+-%3E+%7B%7B%22SolveSystemOf2EquationsCalculator%22%7D,+%22dflt%22%7D&assumption=%7B%22F%22,+%22SolveSystemOf2EquationsCalculator%22,+%22equation1%22%7D+-%3E%221.6408+%3D+x+%2B+y+%2F+(435%5E2)%22&assumption=%7B%22F%22,+%22SolveSystemOf2EquationsCalculator%22,+%22equation2%22%7D+-%3E%221.6187+%3D+x+%2B+y+%2F+(615%5E2)%22


	fz_n_lambda->SetLineColor(4);
	g_n_lambda->Fit(fz_n_lambda, "ERM+");


	std::cout << "Chi^2:" << fz_n_lambda->GetChisquare() << ", number of DoF: " << fz_n_lambda->GetNDF() << " (Probability: " << fz_n_lambda->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------------------------" << endl;



	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "___________________________________________LEGGE DI CAUCHY AL SECONDO ORDINE CON LAMBDA DALLA DEIFFRAZIONE CON RETICOLO____________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________" << endl;


	TGraphErrors * g_n_lambda2 = new TGraphErrors(lambda_v.size(), lambda_v.data(), n, s_lambda, s_n);
	g_n_lambda2->SetMarkerSize(0.4);
	g_n_lambda2->SetMarkerStyle(20);
	g_n_lambda2->SetTitle("#scale[0.55]{Funzione di fit:    n_{prisma} = A + #frac{B}{#lambda^{2}_{reticolo}} + #frac{C}{#lambda^{4}_{reticolo}}}");
	g_n_lambda2->GetXaxis()->SetTitle("#lambda_{reticolo} (nm)");
	g_n_lambda2->GetYaxis()->SetTitle("n_{prisma} (adim)");


	std::cout << "\n\n --- Ipotesi   y = [0] + [1] / x*x + [2] / x * x * x * x --- \n" << endl;
	TF1 * fz_n_lambda2 = new TF1("fz_n_lambda", "[0] + [1] / pow(x,2) + [2] / pow(x,4)", *min_element(begin(lambda_v), end(lambda_v)), *max_element(begin(lambda_v), end(lambda_v)));
	fz_n_lambda2->SetParNames("A (adim)", "B (nm^{2})", "C (nm^{4})");
	fz_n_lambda2->SetParameter(0, 1.6);

	fz_n_lambda2->SetLineColor(6);
	g_n_lambda2->Fit(fz_n_lambda2, "ERM+");


	std::cout << "Chi^2:" << fz_n_lambda2->GetChisquare() << ", number of DoF: " << fz_n_lambda2->GetNDF() << " (Probability: " << fz_n_lambda2->GetProb() << ")." << endl;
	std::cout << "-----------------------------------------------------------------------------------------------------------------------------------------------" << endl;





	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "____________________________________________________LEGGE DI CAUCHY AL PRIMO ORDINE CON LAMBDA DALLO SPETTROFOTOMETRO______________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________" << endl;

	float lambda3[] = {436, 545, 574, 577, 614, 568, 616};
	float s_lambda3[] = {5, 5, 5, 5, 5, 5, 5,};

	float n3[] = {1.6408, 1.6231, 1.6199, 1.6197, 1.6187, 1.6226, 1.6187};
	float s_n3[] = {0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001};

	vector<float> lambda3_v = vector<float>(begin(lambda3), end(lambda3));

	TGraphErrors * g_n_lambda3 = new TGraphErrors(lambda3_v.size(), lambda3_v.data(), n3, s_lambda3, s_n3);
	g_n_lambda3->SetMarkerSize(0.4);
	g_n_lambda3->SetMarkerStyle(20);
	g_n_lambda3->SetTitle("#scale[0.55]{Funzione di fit:    n_{prisma} = A + #frac{B}{#lambda^{2}_{spettrofotometro}}}");
	g_n_lambda3->GetXaxis()->SetTitle("#lambda_{spettrofotometro} (nm)");
	g_n_lambda3->GetYaxis()->SetTitle("n_{prisma} (adim)");


	std::cout << "\n\n --- Ipotesi   y = [0] + [1] / x * x--- \n" << endl;
	TF1 * fz_n_lambda3 = new TF1("fz_n_lambda", "[0] + [1] / pow(x,2)", *min_element(begin(lambda3_v), end(lambda3_v)), *max_element(begin(lambda3_v), end(lambda3_v)));
	fz_n_lambda3->SetParNames("A (adim)", "B (nm^{2})");

	float guess_par0_3 = 1.5965;
	float guess_par1_3 = 8418.6;
	fz_n_lambda3->SetParameter(0, guess_par0_3);
	fz_n_lambda3->SetParameter(1, guess_par1_3);


	fz_n_lambda3->SetParLimits(0, guess_par0_3 - guess_par0_3 / 10, guess_par0_3 + guess_par0_3 / 10);
	fz_n_lambda3->SetParLimits(1, guess_par1_3 - guess_par1_3 / 10, guess_par1_3 + guess_par1_3 / 10);


	fz_n_lambda3->SetLineColor(7);
	g_n_lambda3->Fit(fz_n_lambda3, "ERM+");


	std::cout << "Chi^2:" << fz_n_lambda3->GetChisquare() << ", number of DoF: " << fz_n_lambda3->GetNDF() << " (Probability: " << fz_n_lambda3->GetProb() << ")." << endl;
	std::cout << "-----------------------------------------------------------------------------------------------------------------------------------------------" << endl;


	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "____________________________________________________LEGGE DI CAUCHY AL SECONDO ORDINE CON LAMBDA DALLO SPETTRO-FOTOMETRO___________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________" << endl;

	float lambda4[] = {436, 545, 574, 577, 614, 568, 616};
	float s_lambda4[] = {5, 5, 5, 5, 5, 5, 5,};

	vector<float> lambda4_v = vector<float>(begin(lambda4), end(lambda4));

	TGraphErrors * g_n_lambda4 = new TGraphErrors(lambda4_v.size(), lambda4_v.data(), n3, s_lambda4, s_n3);
	g_n_lambda4->SetMarkerSize(0.4);
	g_n_lambda4->SetMarkerStyle(20);
	g_n_lambda4->SetTitle("#scale[0.55]{Funzione di fit:    n_{prisma} = A + #frac{B}{#lambda^{2}_{spettrofotometro}} + #frac{C}{#lambda^{4}_{spettrofotometro}} }");
	g_n_lambda4->GetXaxis()->SetTitle("#lambda (nm)");
	g_n_lambda4->GetYaxis()->SetTitle("n_{prisma} (adim)");


	std::cout << "\n\n --- Ipotesi   y = [0] + [1] / x * x + [2] / pow(x,4)--- \n" << endl;
	TF1 * fz_n_lambda4 = new TF1("fz_n_lambda", "[0] + [1] / pow(x,2) + [2] / pow(x,4)", *min_element(begin(lambda4_v), end(lambda4_v)), *max_element(begin(lambda4_v), end(lambda4_v)));
	fz_n_lambda4->SetParNames("A (adim)", "B (nm^{2})", "C (nm^{4})");

	float guess_par0_4 = 1.5965;
	float guess_par1_4 = 8418.6;
	fz_n_lambda4->SetParameter(0, guess_par0_4);
	fz_n_lambda4->SetParameter(1, guess_par1_4);


	fz_n_lambda4->SetParLimits(0, guess_par0_4 - guess_par0_4 / 10, guess_par0_4 + guess_par0_4 / 10);
	fz_n_lambda4->SetParLimits(1, guess_par1_4 - guess_par1_4 / 10, guess_par1_4 + guess_par1_4 / 10);


	fz_n_lambda4->SetLineColor(8);
	g_n_lambda4->Fit(fz_n_lambda4, "ERM+");


	std::cout << "Chi^2:" << fz_n_lambda4->GetChisquare() << ", number of DoF: " << fz_n_lambda4->GetNDF() << " (Probability: " << fz_n_lambda4->GetProb() << ")." << endl;
	std::cout << "-----------------------------------------------------------------------------------------------------------------------------------------------" << endl;


	TCanvas * c_n_lambda = new TCanvas("c_n_lambda", "grafico1", 0, 0, 1280, 720);
	c_n_lambda->SetFillColor(0);
	c_n_lambda->cd();

	g_n_lambda->Draw("AP");

	TCanvas * c_n_lambda2 = new TCanvas("c_n_lambda2", "grafico 2", 0, 0, 1280, 720);
	c_n_lambda2->SetFillColor(0);
	c_n_lambda2->cd();

	g_n_lambda2->Draw("AP");

	TCanvas * c_n_lambda3 = new TCanvas("c_n_lambda3", "grafico 3", 0, 0, 1280, 720);
	c_n_lambda3->SetFillColor(0);
	c_n_lambda3->cd();

	g_n_lambda3->Draw("AP");


	TCanvas * c_n_lambda4 = new TCanvas("c_n_lambda4", "grafico 4", 0, 0, 1280, 720);
	c_n_lambda4->SetFillColor(0);
	c_n_lambda4->cd();

	g_n_lambda4->Draw("AP");
}

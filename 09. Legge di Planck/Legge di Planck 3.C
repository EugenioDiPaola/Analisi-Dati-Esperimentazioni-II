#include <iostream>   
#include <iomanip>  
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>

//ROOT classes
#include <TF1.h>
#include <TCanvas.h>
#include <TGraph.h> 
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h> 
#include <TMultiGraph.h>     
#include <TAxis.h>                   
#include "TVirtualFitter.h"  
#include <TLegend.h>
#include <TLatex.h>
#include <TColor.h>

void vectorPrint(string VECTOR_NAME, vector<float> & VECTOR)
{
	int counter = 1;
	std::cout << endl << VECTOR_NAME + ": " << endl;
	for(auto & x : VECTOR)
	{
		std::cout << counter << ": " << x << endl;
		counter++;
	}
	std::cout << endl;
}


//I method: i vs Vc, Vc0 ascissa dell'intersezione tra il fit della discesa e il fit del plateau.
//2nd method: Vc vs i, fitto i due punti sperimentali a cavallo dello 0 di corrente, Vc0 è il valore di tensione per cui la corrente, l'ascissa, è nulla, dunque Vc0 è l'intercetta della retta di fit;
//terzo method: i vs Vc, Vc0 è l'ascissa della prima corrente che si discosta di più di 3 sigma dal fit del plateau.

vector<float> s_iConstructor(vector<float> VECTOR, float ERROR)
{

	vector<float> return_vec;
	for(int i = 0; i < VECTOR.size(); i++)
	{
		return_vec.push_back(ERROR);
	}

	return return_vec;
}


vector<float> sVcConstructor(vector<float> VECTOR)
{

	vector<float> return_vec;
	for(auto & element : VECTOR)
	{
		if(element < 10)
		{
			return_vec.push_back(0.1);
		}
		if(element > 10 && element < 1e2)
		{
			return_vec.push_back(0.1);
		}
		if(element >= 1e2 && element < 1e3)
		{
			return_vec.push_back(0.1);
		}
		if(element >= 1e3 && element <= 1e4)
		{
			return_vec.push_back(1);
		}
	}

	return return_vec;
}


vector<float> computeVc0AndErrorsThirdMethod(float LAMBDA, int COLOR, TCanvas * CANVAS, vector<float> V_C, vector<float> I, vector<float> S_V_C, vector<float> S_I, float ERROR)
{

	int lambda_int = (int) LAMBDA;
	string title_canvas_str = "grafico " + to_string(lambda_int) + "(nm)";
	char title_canvas_char[title_canvas_str.size() + 1];
	std::copy(title_canvas_str.begin(), title_canvas_str.end(), title_canvas_char);
	title_canvas_char[title_canvas_str.size()] = '\0';

	CANVAS->cd();
	CANVAS->SetTitle(title_canvas_char);

	TGraphErrors * g_iVc = new TGraphErrors(V_C.size(), V_C.data(), I.data(), S_V_C.data(), S_I.data());
	g_iVc->SetMarkerSize(0.4);
	g_iVc->SetMarkerStyle(20);

	string title_graph_str = "#scale[0.8]{Ricerca di V_{C}_{0} per #color[" + to_string(COLOR) + "]{#lambda = " + to_string(lambda_int) + "(nm)}, III method}";
	char title_graph_char[title_graph_str.size() + 1];
	std::copy(title_graph_str.begin(), title_graph_str.end(), title_graph_char);
	title_graph_char[title_graph_str.size()] = '\0';

	g_iVc->SetTitle(title_graph_char);
	g_iVc->GetXaxis()->SetTitle("V_{C} (mV)");
	g_iVc->GetYaxis()->SetTitle("i (nA)");
	g_iVc->GetXaxis()->SetLimits(0, *max_element(V_C.begin(), V_C.end()));

	/*
	for(int i = 0; i < I.size(); i++){
		string label_str = "#scale[0.5]{" + to_string(i) + "}";
		char label_char[label_str.size() + 1];
		std::copy(label_str.begin(), label_str.end(), label_char);
		label_char[label_str.size()] = '\0';
		TLatex * tlatex_obj = new TLatex(g_iVc->GetX()[i], g_iVc->GetY()[i],label_char);
		g_iVc->GetListOfFunctions()->Add(tlatex_obj);
	}
	*/

	float min_current = *min_element(begin(I), end(I));

	TF1 * fz_iVc = new TF1("fz_iVc", "[0]", 0, 4e3);
	fz_iVc->SetParNames("q (nA^{-1} mV)", "q (mV)");
	fz_iVc->SetLineColor(COLOR);
	fz_iVc->SetLineWidth(1);

	fz_iVc->SetParameter(0, min_current);
	fz_iVc->SetParLimits(0, min_current - min_current / 50, min_current + min_current / 50);

	g_iVc->Fit(fz_iVc, "RM+", " ", *(V_C.end() - 5), *V_C.end());

	((TF1 *) (g_iVc->FindObject("fz_iVc")))->SetRange(0, 4e3);

	g_iVc->Draw("AP");

	std::cout << "Chi^2 :" << fz_iVc->GetChisquare() << ", number of DoF : " << fz_iVc->GetNDF() << " (Probability : " << fz_iVc->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;


	float q = fz_iVc->GetParameter(0);

	vector<float>::iterator it = (I.end() - 6);
	std::cout << "(I.end() - 6): " << *it << endl;
	int index_from_end = 0;


	//std::cout << "q: " << q << endl;

	//std::cout  << "*it - q: " << *it - q << endl;
	//std::cout  << "3 * (*it / 100): " << 3 * (ERROR) << endl;

	//std::cout << "before while" << endl;
	while(*it - q < 3 * ERROR)
	{
		//std::cout << "*it - q" << *it - q << endl;
		it--;

		//std::cout << "index_from_end: " << index_from_end << endl;

		index_from_end++;
		//std::cout << "*it: " << *it << endl;

		//std::cout << "index_from_end: " << index_from_end << endl;
	}

	float Vc0_1 = *(V_C.end() - 6 - index_from_end + 1);
	float Vc0_2 = *(V_C.end() - 6 - index_from_end);
	float Vc0_3 = *(V_C.end() - 6 - index_from_end - 1);

	vector<float> Vc0_vec{Vc0_2, abs(Vc0_2 - Vc0_1), abs(Vc0_3 - Vc0_2)};

	std::cout << "-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-,-.-.-.-.-.-.-,-.-.-.-.-.-.-.-.-.-.-.-.-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-" << endl;
	std::cout << "third method, lambda: " << LAMBDA << endl;
	vectorPrint(" ", Vc0_vec);

	return Vc0_vec;
}


vector<float> computeVc0AndErrorsSecondMethod(float LAMBDA, int COLOR, TCanvas * CANVAS, vector<float> V_C, vector<float> I)
{//scambiare Vc con i

	int lambda_int = (int) LAMBDA;
	string title_canvas_str = "grafico " + to_string(lambda_int) + "(nm)";
	char title_canvas_char[title_canvas_str.size() + 1];
	std::copy(title_canvas_str.begin(), title_canvas_str.end(), title_canvas_char);
	title_canvas_char[title_canvas_str.size()] = '\0';

	CANVAS->cd();
	CANVAS->SetTitle(title_canvas_char);

	TGraph * gVc_i = new TGraphErrors(I.size(), I.data(), V_C.data());
	gVc_i->SetMarkerSize(0.4);
	gVc_i->SetMarkerStyle(20);

	string title_graph_str = "#scale[0.8]{Ricerca di V_{C}_{0} per #color[" + to_string(COLOR) + "]{#lambda = " + to_string(lambda_int) + "(nm)}, II method}";
	char title_graph_char[title_graph_str.size() + 1];
	std::copy(title_graph_str.begin(), title_graph_str.end(), title_graph_char);
	title_graph_char[title_graph_str.size()] = '\0';

	gVc_i->SetTitle(title_graph_char);
	gVc_i->GetYaxis()->SetTitle("V_{C} (mV)");
	gVc_i->GetXaxis()->SetTitle("i (nA)");

	float width1 = abs(*min_element(I.begin(), I.end()));

	gVc_i->GetXaxis()->SetLimits(-1 * width1, width1 * 5);

	for(int i = 0; i < I.size(); i++)
	{
		string label_str = "#scale[0.5]{" + to_string(i) + "}";
		char label_char[label_str.size() + 1];
		std::copy(label_str.begin(), label_str.end(), label_char);
		label_char[label_str.size()] = '\0';
		TLatex * tlatex_obj = new TLatex(gVc_i->GetX()[i], gVc_i->GetY()[i], label_char);
		gVc_i->GetListOfFunctions()->Add(tlatex_obj);
	}

	vector<float>::iterator it = I.begin();
	int number_of_point = 0;
	while(*it > 0)
	{
		++it;
		++number_of_point;
	}

	float guess_par1 = (V_C[number_of_point] - V_C[number_of_point - 1]) / (I[number_of_point] - I[number_of_point - 1]);
	float guess_par0 = V_C[number_of_point - 1] - guess_par1 * I[number_of_point - 1];

	float above_null_current = *(it - 1);
	float below_null_current = *it;

	TF1 * fzVc_i = new TF1("fzVc_i", "[0] + [1] * x", -0.1, 1.4);
	fzVc_i->SetParNames("m (nA^{-1} mV)", "q (mV)");
	fzVc_i->SetLineColor(COLOR);
	fzVc_i->SetLineWidth(2);

	fzVc_i->SetParameter(0, guess_par0);
	fzVc_i->SetParLimits(0, guess_par0 - guess_par0 / 50, guess_par0 + guess_par0 / 50);

	fzVc_i->SetParameter(1, guess_par1);
	fzVc_i->SetParLimits(1, guess_par1 - guess_par1 / 50, guess_par1 + guess_par1 / 50);

	gVc_i->Fit(fzVc_i, "RM+", " ", below_null_current, above_null_current);

	((TF1 *) (gVc_i->FindObject("fzVc_i")))->SetRange(-0.1, 0.5);

	gVc_i->Draw("AP");

	std::cout << "Chi^2 :" << fzVc_i->GetChisquare() << ", number of DoF : " << fzVc_i->GetNDF() << " (Probability : " << fzVc_i->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;

	vector<float> return_vec{(float) fzVc_i->GetParameter(0), (float) fzVc_i->GetParError(0)};

	std::cout << "-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-,-.-.-.-.-.-.-,-.-.-.-.-.-.-.-.-.-.-.-.-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-" << endl;
	std::cout << "second method, lambda: " << LAMBDA << endl;
	std::cout << "q: " << fzVc_i->GetParameter(0) << endl;
	std::cout << "sq: " << fzVc_i->GetParError(0) << endl;

	return return_vec;
}


float computeVc0FirstMethod(float LAMBDA, int COLOR, int LINE_STYLE, TCanvas * CANVAS, TMultiGraph * MULTIGRAPH, vector<float> V_C, vector<float> I, vector<float> S_V_C, vector<float> S_I, int BEGIN_INCL, int END_INCL, int BEGIN_ORIZ, int END_ORIZ)
{

	CANVAS->cd();

	if(BEGIN_INCL < 0)
	{
		BEGIN_INCL = 0;
	};
	if(BEGIN_ORIZ < 0)
	{
		BEGIN_ORIZ = 0;
	};
	if(END_INCL > V_C.size() - 1)
	{
		END_INCL = V_C.size() - 1;
	};
	if(END_ORIZ > V_C.size() - 1)
	{
		END_ORIZ = V_C.size() - 1;
	};

	vector<float> Vc_incl(V_C.cbegin() + BEGIN_INCL, V_C.cbegin() + END_INCL + 1); //range constructor prende [,); l'ultimo elemento è escluso dal range constructor proprio per come è definito
	vector<float> sVc_incl(S_V_C.cbegin() + BEGIN_INCL, S_V_C.cbegin() + END_INCL + 1);
	vector<float> i_incl(I.cbegin() + BEGIN_INCL, I.cbegin() + END_INCL + 1);
	vector<float> s_i_incl(S_I.cbegin() + BEGIN_INCL, S_I.cbegin() + END_INCL + 1);

	vector<float> Vc_oriz(V_C.cbegin() + BEGIN_ORIZ, V_C.cbegin() + END_ORIZ + 1);
	vector<float> sVc_oriz(S_V_C.cbegin() + BEGIN_ORIZ, S_V_C.cbegin() + END_ORIZ + 1);
	vector<float> i_oriz(I.cbegin() + BEGIN_ORIZ, I.cbegin() + END_ORIZ + 1);
	vector<float> s_i_oriz(S_I.cbegin() + BEGIN_ORIZ, S_I.cbegin() + END_ORIZ + 1);


	TGraph * g_iVc_incl = new TGraphErrors(END_INCL - BEGIN_INCL + 1, Vc_incl.data(), i_incl.data());
	g_iVc_incl->SetMarkerSize(0.4);
	g_iVc_incl->SetMarkerStyle(20);

	for(int i = 0; i < Vc_incl.size(); i++)
	{
		string label_str = "#scale[0.5]{{}^{ }" + to_string(i + BEGIN_INCL) + "}";
		char label_char[label_str.size() + 1];
		std::copy(label_str.begin(), label_str.end(), label_char);
		label_char[label_str.size()] = '\0';
		TLatex * tlatex_obj = new TLatex(g_iVc_incl->GetX()[i], g_iVc_incl->GetY()[i], label_char);
		g_iVc_incl->GetListOfFunctions()->Add(tlatex_obj);
	}

	TGraphErrors * g_iVc_oriz = new TGraphErrors(END_ORIZ - BEGIN_ORIZ + 1, Vc_oriz.data(), i_oriz.data());
	g_iVc_oriz->SetMarkerSize(0.4);
	g_iVc_oriz->SetMarkerStyle(20);

	for(int i = 0; i < Vc_oriz.size(); i++)
	{
		string label_str = "#scale[0.5]{{}^{ }" + to_string(i + BEGIN_ORIZ) + "}";
		char label_char[label_str.size() + 1];
		std::copy(label_str.begin(), label_str.end(), label_char);
		label_char[label_str.size()] = '\0';
		TLatex * tlatex_obj = new TLatex(g_iVc_oriz->GetX()[i], g_iVc_oriz->GetY()[i], label_char);
		g_iVc_oriz->GetListOfFunctions()->Add(tlatex_obj);
	}

	TF1 * fz_iVc_incl = new TF1("fz_iVc_incl", "[0] + [1] * x", -100, 5000);
	fz_iVc_incl->SetParNames("m (nA mV^{-1})", "q (nA)");
	fz_iVc_incl->SetLineColor(COLOR);
	fz_iVc_incl->SetLineWidth(2);
	fz_iVc_incl->SetLineStyle(LINE_STYLE);

	float guess_par1_incl = (I[BEGIN_INCL] - I[END_INCL]) / (V_C[BEGIN_INCL] - V_C[END_INCL]);
	float guess_par0_incl = I[END_INCL] - guess_par1_incl * V_C[END_INCL];

	fz_iVc_incl->SetParameter(0, guess_par0_incl);
	fz_iVc_incl->SetParLimits(0, guess_par0_incl - guess_par0_incl / 50, guess_par0_incl + guess_par0_incl / 50);

	fz_iVc_incl->SetParameter(1, guess_par1_incl);
	fz_iVc_incl->SetParLimits(1, guess_par1_incl - guess_par1_incl / 50, guess_par1_incl + guess_par1_incl / 50);


	g_iVc_incl->Fit(fz_iVc_incl, "WRM+");

	std::cout << "Chi^2 :" << fz_iVc_incl->GetChisquare() << ", number of DoF : " << fz_iVc_incl->GetNDF() << " (Probability : " << fz_iVc_incl->GetProb() << ")." << endl;
	std::cout << "-----------------------------------------------------------------------------------------------------------------------------------------------" << endl;

	TF1 * fz_iVc_oriz = new TF1("fz_iVc_oriz", "[0]", -100, 5000);
	fz_iVc_oriz->SetParNames("r(nA)");
	fz_iVc_oriz->SetLineColor(COLOR);
	fz_iVc_oriz->SetLineWidth(2);
	fz_iVc_oriz->SetLineStyle(LINE_STYLE);

	float guess_par1_oriz = (I[BEGIN_ORIZ] - I[END_ORIZ]) / (V_C[BEGIN_ORIZ] - V_C[END_ORIZ]);
	float guess_par0_oriz = I[END_ORIZ] - guess_par1_oriz * V_C[END_ORIZ];

	fz_iVc_oriz->SetParameter(0, guess_par0_oriz);
	fz_iVc_oriz->SetParLimits(0, guess_par0_oriz - guess_par0_oriz / 50, guess_par0_oriz + guess_par0_oriz / 50);

	fz_iVc_oriz->SetParameter(1, guess_par1_oriz);
	fz_iVc_oriz->SetParLimits(1, guess_par1_oriz - guess_par1_oriz / 50, guess_par1_oriz + guess_par1_oriz / 50);

	g_iVc_oriz->Fit(fz_iVc_oriz, "WRM+");

	std::cout << "Chi^2 :" << fz_iVc_oriz->GetChisquare() << ", number of DoF : " << fz_iVc_oriz->GetNDF() << " (Probability : " << fz_iVc_oriz->GetProb() << ")." << endl;
	std::cout << "-----------------------------------------------------------------------------------------------------------------------------------------------" << endl;

	MULTIGRAPH->Add(g_iVc_incl);
	MULTIGRAPH->Add(g_iVc_oriz);

	int lambda_int = (int) LAMBDA;
	string title_str = "#scale[0.8]{Ricerca di V_{C}_{0} per #color[" + to_string(COLOR) + "]{#lambda = " + to_string(lambda_int) + "(nm)}, I method}";
	char title_char[title_str.size() + 1];
	std::copy(title_str.begin(), title_str.end(), title_char);
	title_char[title_str.size()] = '\0';

	MULTIGRAPH->SetTitle(title_char);

	float r = fz_iVc_oriz->GetParameter(0);
	float q = fz_iVc_incl->GetParameter(0);
	float m = fz_iVc_incl->GetParameter(1);

	std::cout << "()()()()(()()()()()()()()())()()()()()()()()()()()()()(()()()()()()()()()()()()()(()()()()()()()()()()()()()(()()()()()()()()()()()()()(()()()()()()()()()()()()()(()()()()()()(" << endl;
	std::cout << "s_r" << fz_iVc_oriz->GetParError(0) << endl;
	std::cout << "s_q" << fz_iVc_incl->GetParError(0) << endl;
	std::cout << "s_m" << fz_iVc_incl->GetParError(1) << endl;

	float Vc0 = (r - q) / m;

	return Vc0;
}


vector<float> computeVc0AndErrorsFirstMethod(int ORIGIN, int COLOR, float LAMBDA, TCanvas * CANVAS1, TCanvas * CANVAS2, vector<float> V_C, vector<float> I, vector<float> S_V_C, vector<float> S_I, int BEGIN_INCL, int END_INCL, int BEGIN_ORIZ, int END_ORIZ, float MIN, float MAX)
{

	CANVAS1->cd();

	int lambda_int = (int) LAMBDA;
	string title_canvas_str = "grafico " + to_string(lambda_int) + "(nm)";
	char title_canvas_char[title_canvas_str.size() + 1];
	std::copy(title_canvas_str.begin(), title_canvas_str.end(), title_canvas_char);
	title_canvas_char[title_canvas_str.size()] = '\0';

	TGraphErrors * gI_V_C = new TGraphErrors(V_C.size(), V_C.data(), I.data(), S_V_C.data(), S_I.data());
	gI_V_C->SetMarkerSize(0.4);
	gI_V_C->SetMarkerStyle(20);

	string title_str = "#scale[1]{i vs V_{C} per #color[" + to_string(COLOR) + "]{#lambda = " + to_string(lambda_int) + "(nm)}}";
	char title_char[title_str.size() + 1];
	std::copy(title_str.begin(), title_str.end(), title_char);
	title_char[title_str.size()] = '\0';

	gI_V_C->SetTitle(title_char);
	gI_V_C->GetXaxis()->SetTitle("V_{C} (mV)");
	gI_V_C->GetYaxis()->SetTitle("i (nA)");
	gI_V_C->GetXaxis()->SetLimits(ORIGIN, *std::max_element(V_C.begin(), V_C.end()) + *std::max_element(V_C.begin(), V_C.end()) / 10);
	gI_V_C->GetHistogram()->SetMinimum(MIN);
	gI_V_C->GetHistogram()->SetMaximum(MAX);

	//gI_V_C->GetHistogram()->SetMinimum(*std::min_element(I.begin(), I.end()) + *std::min_element(I.begin(), I.end()) / 20);
	//gI_V_C->GetHistogram()->SetMaximum(*std::max_element(I.begin(), I.end()) + *std::max_element(I.begin(), I.end()) / 20);     


	for(int i = 0; i < V_C.size(); i++)
	{
		string label_str = "#scale[0.5]{{}^{" + to_string(i) + "}}";
		char label_char[label_str.size() + 1];
		std::copy(label_str.begin(), label_str.end(), label_char);
		label_char[label_str.size()] = '\0';
		TLatex * tlatex_obj = new TLatex(gI_V_C->GetX()[i], gI_V_C->GetY()[i], label_char);
		gI_V_C->GetListOfFunctions()->Add(tlatex_obj);
	}

	gI_V_C->Draw("AP");

	CANVAS2->cd();
	CANVAS2->SetTitle(title_canvas_char);

	/*string title_str1 = "fits " + to_string(lambda_int);
	char title_char1[title_str1.size() + 1];
	std::copy(title_str1.begin(), title_str1.end(), title_char1);
	title_char1[title_str1.size()] = '\0';

	TCanvas * c_iVc_linearfits = new TCanvas(title_char1, title_char1, 0, 0, 1280, 720);;
	c_iVc_linearfits->SetFillColor(0);
	c_iVc_linearfits->cd();
	//c_iVc_linearfits->SetAstat(0); //non funziona */

	TMultiGraph * mg_iVc_linearfits = new TMultiGraph();
	mg_iVc_linearfits->GetXaxis()->SetTitle("V_{C} (mV)");
	mg_iVc_linearfits->GetYaxis()->SetTitle("i (nA)");

	//int color = 3;

	float Vc0_left;
	Vc0_left = computeVc0FirstMethod(LAMBDA, COLOR, 2, CANVAS2, mg_iVc_linearfits, V_C, I, S_V_C, S_I, (BEGIN_INCL - 1), (END_INCL - 1), (BEGIN_ORIZ - 1), (END_ORIZ - 1));

	float Vc0_middle;
	Vc0_middle = computeVc0FirstMethod(LAMBDA, COLOR, 1, CANVAS2, mg_iVc_linearfits, V_C, I, S_V_C, S_I, (BEGIN_INCL), (END_INCL), (BEGIN_ORIZ), (END_ORIZ));

	float Vc0_right;
	Vc0_right = computeVc0FirstMethod(LAMBDA, COLOR, 9, CANVAS2, mg_iVc_linearfits, V_C, I, S_V_C, S_I, (BEGIN_INCL + 1), (END_INCL + 1), (BEGIN_ORIZ + 1), (END_ORIZ + 1));


	string file_name_str = "i vs Vc per lambda = " + to_string(lambda_int) + "(nm).png";
	char file_name_char[file_name_str.size() + 1];
	std::copy(file_name_str.begin(), file_name_str.end(), file_name_char);
	file_name_char[file_name_str.size()] = '\0';

	mg_iVc_linearfits->Draw("AP");

	vector<float> pre_return_vec;
	pre_return_vec.push_back(Vc0_left);
	pre_return_vec.push_back(Vc0_middle);
	pre_return_vec.push_back(Vc0_right);

	std::sort(pre_return_vec.begin(), pre_return_vec.end());

	vector<float> return_vec;
	return_vec.push_back(pre_return_vec[1]);
	return_vec.push_back(pre_return_vec[1] - pre_return_vec[0]);
	return_vec.push_back(pre_return_vec[2] - pre_return_vec[1]);

	vectorPrint(" ", return_vec);

	std::cout << "-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-,-.-.-.-.-.-.-,-.-.-.-.-.-.-.-.-.-.-.-.-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-,-" << endl;
	std::cout << "first method, lambda: " << LAMBDA << endl;
	vectorPrint(" ", return_vec);

	return return_vec;


}


vector<float> computePlanckConstantAndExtractionWork(TCanvas * CANVAS1, TCanvas * CANVAS2, vector<float> COLORS, vector<float> LAMBDAS, vector<float> S_LEFT_LAMBDAS, vector<float> S_RIGHT_LAMBDAS, vector<float> V_C_0, vector<float> S_LEFT_V_C_0, vector<float> S_RIGHT_V_C_0)
{

	//ms^-1
	//fissata
	float c = 299792458;

	//C
	//noto
	float e = 1.602e-19;

	//c (ms^-1) / lambda (nm) * 1e9 = nu (Hz)
	vector<float> nu;
	vector<float> s_left_nu;
	vector<float> s_right_nu;

	for(auto & LAMBDAS_i : LAMBDAS)
	{
		nu.push_back((c / LAMBDAS_i) * 1e9);
	}
	for(int LAMBDAS_i = 0; LAMBDAS_i < LAMBDAS.size(); LAMBDAS_i++)
	{
		s_left_nu.push_back(((c * pow(1 / LAMBDAS[LAMBDAS_i], 2) * S_LEFT_LAMBDAS[LAMBDAS_i])) * 1e9);
	}
	for(int LAMBDAS_i = 0; LAMBDAS_i < LAMBDAS.size(); LAMBDAS_i++)
	{
		s_right_nu.push_back(((c * pow(1 / LAMBDAS[LAMBDAS_i], 2) * S_RIGHT_LAMBDAS[LAMBDAS_i])) * 1e9);
	}

	vector<float> eVc0;
	vector<float> s_left_eVc0;
	vector<float> s_right_eVc0;



	for(vector<float>::iterator V_C_0_it = V_C_0.begin(); V_C_0_it < V_C_0.end(); V_C_0_it++)
	{
		eVc0.push_back(e * (*V_C_0_it) / 1e3);
	}
	for(vector<float>::iterator S_LEFT_V_C_0_it = S_LEFT_V_C_0.begin(); S_LEFT_V_C_0_it < S_LEFT_V_C_0.end(); S_LEFT_V_C_0_it++)
	{
		s_left_eVc0.push_back(e * (*S_LEFT_V_C_0_it) / 1e3);
	}
	for(vector<float>::iterator S_RIGHT_V_C_0_it = S_RIGHT_V_C_0.begin(); S_RIGHT_V_C_0_it < S_RIGHT_V_C_0.end(); S_RIGHT_V_C_0_it++)
	{
		s_right_eVc0.push_back(e * (*S_RIGHT_V_C_0_it) / 1e3);
	}


	std::cout << "eVc0 eVc0 eVc0 eVc0 eVc0 eVc0 eVc0 eVc0 eVc0 " << endl;
	vectorPrint("V_C_0", V_C_0);


	vectorPrint("eVc0", eVc0);
	//eV
	vector<float> We_Cs_teorico{-1.95, -2.16};//4.26, 4.52, 4.64, 4.74};

	//Js
	float h_teorico = 6.626e-34;



	CANVAS1->Divide(2, 1, 0, 0);
	CANVAS2->Divide(2, 2, 1, 1);

	CANVAS1->cd(1);

	//CANVAS1->cd();

	//grafico #1 Cesio, We = 1.95 (eV)
	TGraphAsymmErrors * g_eVc0_nu_Cs1 = new TGraphAsymmErrors(nu.size(), nu.data(), eVc0.data(), s_left_nu.data(), s_right_nu.data(), s_left_eVc0.data(), s_right_eVc0.data());
	g_eVc0_nu_Cs1->SetMarkerSize(0.4);
	g_eVc0_nu_Cs1->SetMarkerStyle(20);
	g_eVc0_nu_Cs1->SetTitle("#scale[0.8]{Funzione di fit:    eV_{C}_{0} = h#nu - W_{e}}");
	g_eVc0_nu_Cs1->GetYaxis()->SetTitle("e #upoint V_{C}_{0} (J)");//("e #upoint V_{C}_{0} (meV)");
	g_eVc0_nu_Cs1->GetXaxis()->SetTitle("#nu (Hz)");

	int i = 0;
	vector<float>::iterator it_V_C_0 = V_C_0.begin();
	for(vector<float>::iterator it = COLORS.begin(); it < COLORS.end(); ++it)
	{
		string label_str = "#scale[1]{#color[" + to_string((int) (*it)) + "]{#bullet^{#scale[0.5]{V_{C}_{0} = " + to_string((int) (*it_V_C_0)) + "(mV)}}}}";
		char label_char[label_str.size() + 1];
		std::copy(label_str.begin(), label_str.end(), label_char);
		label_char[label_str.size()] = '\0';
		TLatex * tlatex_obj = new TLatex(g_eVc0_nu_Cs1->GetX()[i], g_eVc0_nu_Cs1->GetY()[i], label_char);
		g_eVc0_nu_Cs1->GetListOfFunctions()->Add(tlatex_obj);
		i++;
		it_V_C_0++;
	}

	float x_end = *std::max_element(std::begin(nu), std::end(nu)) + *std::max_element(std::begin(nu), std::end(nu)) / 10;

	TF1 * fz_eVc0_nuCs1 = new TF1("fz_eVc0_nuCs1", "[0] * x + [1] * 1.602e-19", 0, x_end);
	fz_eVc0_nuCs1->SetParNames("h (Js)", "W_{e}^{Cs} (eV)");

	fz_eVc0_nuCs1->SetParameter(0, h_teorico);
	fz_eVc0_nuCs1->SetParameter(1, We_Cs_teorico[0]);

	//fz_eVc0_nuCs1->SetParLimits(0, h_teorico - h_teorico / 10, h_teorico + h_teorico / 10);
	fz_eVc0_nuCs1->SetParLimits(1, We_Cs_teorico[0] - We_Cs_teorico[0] / 10, We_Cs_teorico[0] + We_Cs_teorico[0] / 10);

	fz_eVc0_nuCs1->SetLineColor(921);

	float min_nu = *min_element(nu.begin(), nu.end());
	float max_nu = *max_element(nu.begin(), nu.end());

	g_eVc0_nu_Cs1->GetXaxis()->SetLimits(min_nu - min_nu / 15, max_nu + max_nu / 8);
	g_eVc0_nu_Cs1->Fit(fz_eVc0_nuCs1, "ERM+");

	std::cout << "Chi^2:" << fz_eVc0_nuCs1->GetChisquare() << ", number of DoF: " << fz_eVc0_nuCs1->GetNDF() << " (Probability: " << fz_eVc0_nuCs1->GetProb() << ")." << endl;
	std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

	g_eVc0_nu_Cs1->Draw("AP");


	CANVAS1->cd(2);

	//CANVAS2->cd();

	//grafico #2 Cesio, We = 2.16 (eV)
	TGraphAsymmErrors * g_eVc0_nu_Cs2 = new TGraphAsymmErrors(nu.size(), nu.data(), eVc0.data(), s_left_nu.data(), s_right_nu.data(), s_left_eVc0.data(), s_right_eVc0.data());
	g_eVc0_nu_Cs2->SetMarkerSize(0.4);
	g_eVc0_nu_Cs2->SetMarkerStyle(20);
	g_eVc0_nu_Cs2->SetTitle("#scale[0.8]{Funzione di fit:    eV_{C}_{0} = h#nu - W_{e}}");
	g_eVc0_nu_Cs2->GetYaxis()->SetTitle("e #upoint V_{C}_{0} (J)");//("e #upoint V_{C}_{0} (meV)");
	g_eVc0_nu_Cs2->GetXaxis()->SetTitle("#nu (Hz)");

	int j = 0;
	it_V_C_0 = V_C_0.begin();
	for(vector<float>::iterator it = COLORS.begin(); it < COLORS.end(); ++it)
	{
		string label_str = "#scale[1]{#color[" + to_string((int) (*it)) + "]{#bullet^{#scale[0.5]{V_{C}_{0} = " + to_string((int) (*it_V_C_0)) + "(mV)}}}}";
		char label_char[label_str.size() + 1];
		std::copy(label_str.begin(), label_str.end(), label_char);
		label_char[label_str.size()] = '\0';
		TLatex * tlatex_obj = new TLatex(g_eVc0_nu_Cs2->GetX()[j], g_eVc0_nu_Cs2->GetY()[j], label_char);
		g_eVc0_nu_Cs2->GetListOfFunctions()->Add(tlatex_obj);
		j++;
		it_V_C_0++;
	}


	TF1 * fz_eVc0_nuCs2 = new TF1("fz_eVc0_nuCs2", "[0] * x + [1] * 1.602e-19", 0, x_end);
	fz_eVc0_nuCs2->SetParNames("h (Js)", "W_{e}^{Cs} (eV)");

	fz_eVc0_nuCs2->SetParameter(0, h_teorico);
	fz_eVc0_nuCs2->SetParameter(1, We_Cs_teorico[1]);

	//fz_eVc0_nuCs2->SetParLimits(0, h_teorico - h_teorico / 10, h_teorico + h_teorico / 10);
	fz_eVc0_nuCs2->SetParLimits(1, We_Cs_teorico[1] - We_Cs_teorico[1] / 10, We_Cs_teorico[1] + We_Cs_teorico[1] / 10);

	fz_eVc0_nuCs2->SetLineColor(921);

	g_eVc0_nu_Cs2->GetXaxis()->SetLimits(min_nu - min_nu / 15, max_nu + max_nu / 8);
	g_eVc0_nu_Cs2->Fit(fz_eVc0_nuCs2, "ERM+");

	std::cout << "Chi^2:" << fz_eVc0_nuCs2->GetChisquare() << ", number of DoF: " << fz_eVc0_nuCs2->GetNDF() << " (Probability: " << fz_eVc0_nuCs2->GetProb() << ")." << endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

	g_eVc0_nu_Cs2->Draw("AP");


	TCanvas * c1 = new TCanvas("c1", "multipads", 900, 700);

	c1->Divide(2, 2, 0.0001, 0.0001);
	TH2F * h1 = new TH2F("h1", "test1", 10, 0, 1, 20, 0, 20);
	TH2F * h2 = new TH2F("h2", "test2", 10, 0, 1, 20, 0, 100);
	TH2F * h3 = new TH2F("h3", "test3", 10, 0, 1, 20, -1, 1);
	TH2F * h4 = new TH2F("h4", "test4", 10, 0, 1, 20, 0, 1000);

	c1->cd(1);
	gPad->SetTickx(2);
	h1->Draw();

	c1->cd(2);
	gPad->SetTickx(2);
	gPad->SetTicky(2);
	h2->GetYaxis()->SetLabelOffset(0.01);
	h2->Draw();

	c1->cd(3);
	h3->Draw();

	c1->cd(4);
	gPad->SetTicky(2);
	h4->Draw();


	vector<float> return_vec;

	return_vec.push_back(fz_eVc0_nuCs1->GetParameter(0));
	return_vec.push_back(fz_eVc0_nuCs1->GetParError(0));
	return_vec.push_back(fz_eVc0_nuCs1->GetParameter(1));
	return_vec.push_back(fz_eVc0_nuCs1->GetParError(1));
	return_vec.push_back(fz_eVc0_nuCs2->GetParameter(0));
	return_vec.push_back(fz_eVc0_nuCs2->GetParError(0));
	return_vec.push_back(fz_eVc0_nuCs2->GetParameter(1));
	return_vec.push_back(fz_eVc0_nuCs2->GetParError(1));

	return return_vec;
}


void planck3()
{

	gStyle->SetOptFit();


	vector<TCanvas *> c_ptr_vec = vector<TCanvas *>();
	//creo un vector di puntatori a TCanvas. poi creerò ogni volta, fuori dalla mia funzione per calcolare Vc0, un TCanvas con new, e passerò il suo puntatore alla funzione, aggiungendo a 
	//da passare come argomenti alle mie varie funzioni,, perchè se invece li dichiaro dentro le funzioni quando esco ho che i puntatori a quegli oggetti allocati nella heap 
	//vengono distrutti e avre i un memory leak

	std::cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££" << endl;

	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "________________________________________________________________________________DATI GREZZI_______________________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	//nm
	//spettrofotometro e Avantes
	float lambda430 = 430;
	float lambda432 = 432;
	float lambda464 = 464;
	float lambda477 = 477;
	float lambda551 = 551;
	float lambda552 = 552;
	float lambda561 = 561;
	float lambda590 = 590;
	float lambda592 = 592;
	float lambda607 = 607;
	float lambda621 = 621;
	float lambda639 = 639;
	float lambda656 = 656;

	float s_left_lambda430 = 28;
	float s_left_lambda432 = 27;
	float s_left_lambda464 = 13;
	float s_left_lambda477 = 11;
	float s_left_lambda551 = 19;
	float s_left_lambda552 = 24;
	float s_left_lambda561 = 7;
	float s_left_lambda590 = 17;
	float s_left_lambda592 = 8;
	float s_left_lambda607 = 12;
	float s_left_lambda621 = 10;
	float s_left_lambda639 = 9;
	float s_left_lambda656 = 9;

	float s_right_lambda430 = 52;
	float s_right_lambda432 = 51;
	float s_right_lambda464 = 10;
	float s_right_lambda477 = 11;
	float s_right_lambda551 = 22;
	float s_right_lambda552 = 26;
	float s_right_lambda561 = 5;
	float s_right_lambda590 = 16;
	float s_right_lambda592 = 5;
	float s_right_lambda607 = 7;
	float s_right_lambda621 = 5;
	float s_right_lambda639 = 6;
	float s_right_lambda656 = 13;



	//colori
	float color430 = 600;
	float color432 = 600;
	float color464 = 857;
	float color477 = 867;
	float color551 = 827;
	float color552 = 827;
	float color561 = 828;
	float color590 = 800;
	float color592 = 800;
	float color607 = 797;
	float color621 = 807;
	float color639 = 632;
	float color656 = 633;



	//nA
	//picoamperometro Keithley 6485
	float i430[] = {0.24925, 0.13342, 0.07488, 0.03820, 0.02388, 0.01395, 0.00844, 0.00502, 0.00265, 0.00126, 0.00034, -0.00025, -0.00052, -0.00062, -0.00063, -0.00070, -0.00070, -0.00075, -0.00079, -0.00078, -0.00080, -0.00077, -0.00082, -0.00081, -0.00082, -0.00085, -0.00086, -0.00091};
	float i432[] = {1.44205, 1.22023, 1.03151, 0.85753, 0.69949, 0.56435, 0.43209, 0.33108, 0.24289, 0.13131, 0.05955, 0.02894, 0.01162, 0.00385, -0.00070, -0.00129, -0.00222, -0.00264, -0.00330, -0.00396, -0.00443, -0.00490, -0.00592, -0.00591, -0.00621, -0.00675, -0.00710, -0.00720, -0.00732, -0.00737, -0.00743, -0.00740, -0.00741, -0.00742, -0.00745, -0.00747, -0.00750, -0.00749};
	float i464[] = {0.32187, 0.24864, 0.14146, 0.05759, 0.01645, -0.00011, -0.00086, -0.0012, -0.00161, -0.00184, -0.00233, -0.00271, -0.00297, -0.00319, -0.00337, -0.00349, -0.00363, -0.00369, -0.00377, -0.00382, -0.0039, -0.00393, -0.00394, -0.00397, -0.00407, -0.00412, -0.00422, -0.00425, -0.00445, -0.00447, -0.00453, -0.00466};
	float i477[] = {1.42159, 1.18098, 0.97085, 0.76679, 0.59988, 0.44704, 0.31903, 0.22249, 0.13666, 0.06688, 0.02397, 0.01337, 0.00698, 0.00253, 0.00068, -0.00009, -0.00096, -0.00147, -0.00208, -0.00251, -0.00296, -0.00328, -0.00353,  -0.00372, -0.00399, -0.00414, -0.00421, -0.00427, -0.00444, -0.00459, -0.00462, -0.00471, -0.00468, -0.00477, -0.00476, -0.00480, -0.00482, -0.00484, -0.00488, -0.00486, -0.00492, -0.00486, -0.00492};
	float i551[] = {0.00149, 0.00088, 0.00071, 0.00039, 0.00028, 0.00025, 0.00014, 0.00008, 0.00004, 0.00001, -0.00001, -0.00004, -0.00008, -0.00012, -0.00015, -0.00016, -0.00018, -0.00018, -0.00020, -0.00022, -0.00023, -0.00023, -0.00024, -0.00024, -0.00027, -0.00028, -0.00029};
	float i552[] = {1.37768, 1.05872, 0.78763, 0.56905, 0.39723, 0.24794, 0.13863, 0.05666, 0.01485, 0.00210, -0.00083, -0.00143, -0.00202, -0.00252, -0.00283, -0.00298, -0.00310, -0.00338, -0.00332, -0.00350, -0.00349, -0.00353, -0.00351, -0.00355, -0.00355,-0.00355, -0.00363, -0.00360, -0.00359, -0.00360, -0.00358};
	float i561[] = {0.00113, 0.00053, 0.00015, 0.00011, -0.00002, -0.00003, -0.00005, -0.00010, -0.00010, -0.00012, -0.00015, -0.00014, -0.00017, -0.00016, -0.00019, -0.00020, -0.00023, -0.00021, -0.00024, -0.00023, -0.00023, -0.00024};
	float i590[] = {0.00175, 0.00064, 0.00050, 0.00026, 0.00004, -0.00005, -0.00010, -0.00015, -0.00019, -0.00020, -0.00021, -0.00022, -0.00023, -0.00023, -0.00023, -0.00025, -0.00026, -0.00027, -0.00028, -0.00029, -0.00032, -0.00032};
	float i592[] = {1.44125, 1.17509, 1.00578, 0.75620, 0.59730, 0.47672, 0.36510, 0.28436, 0.19616, 0.12877, 0.07305, 0.02848, 0.01320, 0.00498, 0.00115, -0.00034, -0.00116, -0.00177, -0.00236, -0.00283, -0.00309, -0.00326, -0.00340, -0.00349, -0.00353, -0.00356, -0.00361, -0.00363, -0.00362, -0.00364, -0.00362, -0.00365, -0.00367, -0.00369, -0.00369, -0.00369, -0.00372, -0.00372, -0.00373, -0.00373};
	float i607[] = {1.41866, 1.21512, 0.99699, 0.82279, 0.66419, 0.51716, 0.40090, 0.29129, 0.19903, 0.13573, 0.07832, 0.03099, 0.01194, 0.00256, 0.00079, -0.00030, -0.00119, -0.0017, -0.00203, -0.00234, -0.00253, -0.00265, -0.00275, -0.00286, -0.00283, -0.00286, -0.00288, -0.00288, -0.00289, -0.00291, -0.00291, -0.00295, -0.00294, -0.00297, -0.00298, -0.00298, -0.00299};
	float i621[] = {1.42369, 1.14779, 0.91383, 0.71759, 0.53516, 0.38618, 0.25095, 0.16134, 0.08109, 0.02823, 0.01443, 0.00650, 0.00267, 0.00104, -0.00045, -0.00133, -0.00176, -0.00216, -0.00231, -0.00249, -0.00266, -0.00265, -0.00270, -0.00272, -0.00273, -0.00276, -0.00275, -0.00273, -0.00274, -0.00277, -0.00275, -0.00279, -0.00275, -0.00278, -0.00275, -0.00275, -0.00280};
	float i639[] = {1.45730, 1.16953, 0.88494, 0.67999, 0.48789, 0.33343, 0.21242, 0.11357, 0.04688, 0.00978, 0.00242, 0.00101, -0.00048, -0.00138, -0.00197, -0.00229, -0.00253, -0.00270, -0.00274, -0.00282, -0.00286, -0.00286, -0.00288, -0.00291, -0.00292, -0.00293, -0.00296, -0.00292, -0.00293, -0.00294, -0.00295, -0.00295, -0.00296, -0.00297};
	float i656[] = {0.0028, -0.00031, -0.00055, -0.00074, -0.00083, -0.00088, -0.00095, -0.00099, -0.00103, -0.00104, -0.00107, -0.00111, -0.00111, -0.00111, -0.00114, -0.00119, -0.00119, -0.0012, -0.0012, -0.00122, -0.00122, -0.00125, -0.00126};


	//V
	//Amprobe 37XR-A
	float Vc430[] = {1.3, 100.4, 193.6, 308.1, 398.0, 506.2, 606.1, 702.8, 804.4, 903.7, 1000, 1116, 1203, 1258, 1287, 1320, 1350, 1379, 1406, 1421, 1457, 1483, 1512, 1525, 1575, 1702, 1908, 2293};
	float Vc432[] = {0.0013, 0.1058, 0.2020, 0.3016, 0.4041, 0.5030, 0.6122, 0.7077, 0.8037, 0.9572, 1.105, 1.205, 1.308, 1.402, 1.500, 1.523, 1.557, 1.580, 1.612, 1.650, 1.685, 1.723, 1.786, 1.824, 1.873, 1.986, 2.093, 2.174, 2.298, 2.383, 2.510, 2.643, 2.796, 2.966, 3.312, 3.673, 4.017, 4.247};
	float Vc464[] = {1.3, 116, 302.2, 499.8, 701.2, 993, 1028, 1049, 1081, 1102, 1150, 1200, 1250, 1298, 1349, 1397, 1452, 1498, 1555, 1607, 1658, 1700, 1749, 1809, 1898, 1999, 2124, 2195, 2442, 2600, 2786, 3013};
	float Vc477[] = {0.0023, 0.1056, 0.1987, 0.3023, 0.4000, 0.5042, 0.6070, 0.6996, 0.7994, 0.9035, 1.005, 1.051, 1.097, 1.152, 1.195, 1.220, 1.254, 1.280, 1.318, 1.350, 1.382, 1.417, 1.451, 1.481, 1.517, 1.545, 1.576, 1.609, 1.654, 1.687, 1.734, 1.767, 1.804, 1.852, 1.901, 2.001, 2.258, 2.492, 2.755, 3.009, 3.471, 3.914, 4.244};
	float Vc551[] = {1.3, 107.1, 150.0, 251.4, 303.2, 334.9, 401.7, 460.2, 507.6, 552.9, 601.2, 654.2, 751.6, 851.3, 900.4, 951.9, 997.0, 1050, 1156, 1303, 1450, 1602, 1807, 2004, 2412, 2602, 2806};
	float Vc552[] = {0.0025, 0.1019, 0.2026, 0.3007, 0.3958, 0.5005, 0.5995, 0.6994, 0.7969, 0.8996, 0.9721, 1.012, 1.051, 1.099, 1.135, 1.168, 1.200, 1.242, 1.281, 1.329, 1.358, 1.403, 1.500, 1.590, 1.801, 2.012, 2.407, 2.793, 3.218, 3.694, 4.244};
	float Vc561[] = {1.3, 150.9, 266.6, 299.8, 379.8, 427.1, 471.5, 504.2, 553.2, 604.1, 654.1, 700.7, 750.3, 800.0, 905.5, 999.0, 1149, 1301, 1459, 1562, 1751, 2004};
	float Vc590[] = {0.9, 129.5, 150.0, 199.2, 267.0, 303.9, 353.2, 402.2, 472.4, 505.9, 550.3, 603.0, 650.2, 699.0, 754.6, 1001, 1242, 1513, 1752, 2005, 2606, 2959};
	float Vc592[] = {0.0012, 0.0728, 0.1202, 0.1984, 0.2555, 0.3043, 0.3560, 0.3985, 0.4523, 0.5009, 0.5495, 0.6048, 0.6383, 0.6780, 0.7191, 0.7479, 0.7716, 0.7969, 0.8281, 0.8610, 0.8923,0.9201, 0.9511, 0.9806, 1.008, 1.041, 1.071, 1.101, 1.161, 1.223, 1.285, 1.341, 1.405,1.507, 2.011, 2.507, 3.005, 3.508, 3.893, 4.243};
	float Vc607[] = {0.0020, 0.0495, 0.1029, 0.1509, 0.1994, 0.2510, 0.2983, 0.3507, 0.4041, 0.4485, 0.4984, 0.5553, 0.5968, 0.6505, 0.6760, 0.7011, 0.7324, 0.7605, 0.7902, 0.8209, 0.8516, 0.8796, 0.9063, 0.9418, 0.972, 1.002, 1.054, 1.127, 1.200, 1.306, 1.499, 2.000, 2.495, 3.028, 3.504, 3.996, 4.246};
	float Vc621[] = {0.0026, 0.0634, 0.1215, 0.1783, 0.2397, 0.3001, 0.3667, 0.4201, 0.4798, 0.5401, 0.5699, 0.6025, 0.6335, 0.6565, 0.6916, 0.7248, 0.7502, 0.7845, 0.8148, 0.8421, 0.8693, 0.9014, 0.9338, 0.9617, 1.001, 1.054, 1.100, 1.204, 1.351, 1.509, 1.751, 1.984, 2.499, 2.990, 3.492, 3.788, 4.244};
	float Vc639[] = {0.0016, 0.0581, 0.1229, 0.1792, 0.2412, 0.3028, 0.3615, 0.4222, 0.4775, 0.5441, 0.5902, 0.6085, 0.6400, 0.6712, 0.7011, 0.7308, 0.7602, 0.7904, 0.8204, 0.8516, 0.8822, 0.9415, 1.002, 1.100, 1.201, 1.348, 1.499, 1.755, 2.005, 2.485, 3.005, 3.431, 3.818, 4.244};
	float Vc656[] = {1.3, 205.7, 251.5, 301.8, 325.2, 349.3, 373.8, 404.8, 425, 451.4, 476, 498.1, 525, 549.3, 600.4, 801.5, 850.1, 900, 997, 1100, 1196, 1300, 1501};


	//correzione di un fattore 100 per le correnti gruppo D7 ch non tornano (forse errore nel leggere l'unità di misura sul picoamperometro)
	for(float * it = begin(i551); it < end(i551); ++it)
	{
		(*it) *= 1e2;
	}
	for(float * it = begin(i561); it < end(i561); ++it)
	{
		(*it) *= 1e2;
	}
	for(float * it = begin(i590); it < end(i590); ++it)
	{
		(*it) *= 1e2;
	}
	for(float * it = begin(i656); it < end(i656); ++it)
	{
		(*it) *= 1e2;
	}



	//conversione in mV delle tensioni del gruppo 2
	for(float * it = begin(Vc432); it < end(Vc432); ++it)
	{
		(*it) *= 1e3;
	}
	for(float * it = begin(Vc477); it < end(Vc477); ++it)
	{
		(*it) *= 1e3;
	}
	for(float * it = begin(Vc552); it < end(Vc552); ++it)
	{
		(*it) *= 1e3;
	}
	for(float * it = begin(Vc592); it < end(Vc592); ++it)
	{
		(*it) *= 1e3;
	}
	for(float * it = begin(Vc607); it < end(Vc607); ++it)
	{
		(*it) *= 1e3;
	}
	for(float * it = begin(Vc621); it < end(Vc621); ++it)
	{
		(*it) *= 1e3;
	}
	for(float * it = begin(Vc639); it < end(Vc639); ++it)
	{
		(*it) *= 1e3;
	}



	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "________________________________________________________________MANIPOLAZIONI PRELIMINARI SUI DATI GREZZI_________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	vector<float> i430_vec(std::begin(i430), std::end(i430));
	vector<float> i432_vec(std::begin(i432), std::end(i432));
	vector<float> i464_vec(std::begin(i464), std::end(i464));
	vector<float> i477_vec(std::begin(i477), std::end(i477));
	vector<float> i551_vec(std::begin(i551), std::end(i551));
	vector<float> i552_vec(std::begin(i552), std::end(i552));
	vector<float> i561_vec(std::begin(i561), std::end(i561));
	vector<float> i590_vec(std::begin(i590), std::end(i590));
	vector<float> i592_vec(std::begin(i592), std::end(i592));
	vector<float> i607_vec(std::begin(i607), std::end(i607));
	vector<float> i621_vec(std::begin(i621), std::end(i621));
	vector<float> i639_vec(std::begin(i639), std::end(i639));
	vector<float> i656_vec(std::begin(i656), std::end(i656));

	vector<float> Vc430_vec(std::begin(Vc430), std::end(Vc430));
	vector<float> Vc432_vec(std::begin(Vc432), std::end(Vc432));
	vector<float> Vc464_vec(std::begin(Vc464), std::end(Vc464));
	vector<float> Vc477_vec(std::begin(Vc477), std::end(Vc477));
	vector<float> Vc551_vec(std::begin(Vc551), std::end(Vc551));
	vector<float> Vc552_vec(std::begin(Vc552), std::end(Vc552));
	vector<float> Vc561_vec(std::begin(Vc561), std::end(Vc561));
	vector<float> Vc590_vec(std::begin(Vc590), std::end(Vc590));
	vector<float> Vc592_vec(std::begin(Vc592), std::end(Vc592));
	vector<float> Vc607_vec(std::begin(Vc607), std::end(Vc607));
	vector<float> Vc621_vec(std::begin(Vc621), std::end(Vc621));
	vector<float> Vc639_vec(std::begin(Vc639), std::end(Vc639));
	vector<float> Vc656_vec(std::begin(Vc656), std::end(Vc656));

	//creazione errori
	float s_i_gruppoD7 = 0.00001;
	float s_i_gruppo2 = 0.001;

	vector<float> s_i430_vec = s_iConstructor(i430_vec, s_i_gruppoD7);
	vector<float> s_i432_vec = s_iConstructor(i432_vec, s_i_gruppo2);
	vector<float> s_i464_vec = s_iConstructor(i464_vec, s_i_gruppoD7);
	vector<float> s_i477_vec = s_iConstructor(i477_vec, s_i_gruppoD7);
	vector<float> s_i551_vec = s_iConstructor(i551_vec, s_i_gruppoD7);
	vector<float> s_i552_vec = s_iConstructor(i552_vec, s_i_gruppo2);
	vector<float> s_i561_vec = s_iConstructor(i561_vec, s_i_gruppoD7);
	vector<float> s_i590_vec = s_iConstructor(i590_vec, s_i_gruppoD7);
	vector<float> s_i592_vec = s_iConstructor(i592_vec, s_i_gruppo2);
	vector<float> s_i607_vec = s_iConstructor(i607_vec, s_i_gruppo2);
	vector<float> s_i621_vec = s_iConstructor(i621_vec, s_i_gruppo2);
	vector<float> s_i639_vec = s_iConstructor(i639_vec, s_i_gruppo2);
	vector<float> s_i656_vec = s_iConstructor(i656_vec, s_i_gruppoD7);

	vector<float> sVc430_vec = sVcConstructor(Vc430_vec);
	vector<float> sVc432_vec = sVcConstructor(Vc432_vec);
	vector<float> sVc464_vec = sVcConstructor(Vc464_vec);
	vector<float> sVc477_vec = sVcConstructor(Vc477_vec);
	vector<float> sVc551_vec = sVcConstructor(Vc551_vec);
	vector<float> sVc552_vec = sVcConstructor(Vc552_vec);
	vector<float> sVc561_vec = sVcConstructor(Vc561_vec);
	vector<float> sVc590_vec = sVcConstructor(Vc590_vec);
	vector<float> sVc592_vec = sVcConstructor(Vc592_vec);
	vector<float> sVc607_vec = sVcConstructor(Vc607_vec);
	vector<float> sVc621_vec = sVcConstructor(Vc621_vec);
	vector<float> sVc639_vec = sVcConstructor(Vc639_vec);
	vector<float> sVc656_vec = sVcConstructor(Vc656_vec);


	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________GRAFICO Vc vs i, TUTTI I LED NELLA STESSA SCALA_____________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TGraphErrors * g_i430_Vc430 = new TGraphErrors(i430_vec.size(), i430_vec.data(), Vc430_vec.data(), s_i430_vec.data(), sVc430_vec.data());
	g_i430_Vc430->SetMarkerColor(color430);
	g_i430_Vc430->SetMarkerStyle(20);
	g_i430_Vc430->SetMarkerSize(0.4);
	g_i430_Vc430->SetMarkerStyle(20);
	g_i430_Vc430->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i430_Vc430->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i432_Vc432 = new TGraphErrors(i432_vec.size(), i432_vec.data(), Vc432_vec.data(), 0, 0);
	g_i432_Vc432->SetMarkerColor(color432);
	g_i432_Vc432->SetMarkerStyle(20);
	g_i432_Vc432->SetMarkerSize(0.4);
	g_i432_Vc432->SetMarkerStyle(20);
	g_i432_Vc432->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i432_Vc432->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i464_Vc464 = new TGraphErrors(i464_vec.size(), i464_vec.data(), Vc464_vec.data(), s_i464_vec.data(), sVc464_vec.data());
	g_i464_Vc464->SetMarkerColor(color464);
	g_i464_Vc464->SetMarkerStyle(20);
	g_i464_Vc464->SetMarkerSize(0.4);
	g_i464_Vc464->SetMarkerStyle(20);
	g_i464_Vc464->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i464_Vc464->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i477_Vc477 = new TGraphErrors(i477_vec.size(), i477_vec.data(), Vc477_vec.data(), 0, 0);
	g_i477_Vc477->SetMarkerColor(color477);
	g_i477_Vc477->SetMarkerStyle(20);
	g_i477_Vc477->SetMarkerSize(0.4);
	g_i477_Vc477->SetMarkerStyle(20);
	g_i477_Vc477->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i477_Vc477->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i551_Vc551 = new TGraphErrors(i551_vec.size(), i551_vec.data(), Vc551_vec.data(), s_i551_vec.data(), sVc551_vec.data());
	g_i551_Vc551->SetMarkerColor(color551);
	g_i551_Vc551->SetMarkerStyle(20);
	g_i551_Vc551->SetMarkerSize(0.4);
	g_i551_Vc551->SetMarkerStyle(20);
	g_i551_Vc551->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i551_Vc551->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i552_Vc552 = new TGraphErrors(i552_vec.size(), i552_vec.data(), Vc552_vec.data(), 0, 0);
	g_i552_Vc552->SetMarkerColor(color552);
	g_i552_Vc552->SetMarkerStyle(20);
	g_i552_Vc552->SetMarkerSize(0.4);
	g_i552_Vc552->SetMarkerStyle(20);
	g_i552_Vc552->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i552_Vc552->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i561_Vc561 = new TGraphErrors(i561_vec.size(), i561_vec.data(), Vc561_vec.data(), s_i561_vec.data(), sVc561_vec.data());
	g_i561_Vc561->SetMarkerColor(color561);
	g_i561_Vc561->SetMarkerStyle(20);
	g_i561_Vc561->SetMarkerSize(0.4);
	g_i561_Vc561->SetMarkerStyle(20);
	g_i561_Vc561->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i561_Vc561->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i590_Vc590 = new TGraphErrors(i590_vec.size(), i590_vec.data(), Vc590_vec.data(), s_i590_vec.data(), sVc590_vec.data());
	g_i590_Vc590->SetMarkerColor(color590);
	g_i590_Vc590->SetMarkerStyle(20);
	g_i590_Vc590->SetMarkerSize(0.4);
	g_i590_Vc590->SetMarkerStyle(20);
	g_i590_Vc590->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i590_Vc590->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i592_Vc592 = new TGraphErrors(i592_vec.size(), i592_vec.data(), Vc592_vec.data(), 0, 0);
	g_i592_Vc592->SetMarkerColor(color592);
	g_i592_Vc592->SetMarkerStyle(20);
	g_i592_Vc592->SetMarkerSize(0.4);
	g_i592_Vc592->SetMarkerStyle(20);
	g_i592_Vc592->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i592_Vc592->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i607_Vc607 = new TGraphErrors(i607_vec.size(), i607_vec.data(), Vc607_vec.data(), 0, 0);
	g_i607_Vc607->SetMarkerColor(color607);
	g_i607_Vc607->SetMarkerStyle(20);
	g_i607_Vc607->SetMarkerSize(0.4);
	g_i607_Vc607->SetMarkerStyle(20);
	g_i607_Vc607->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i607_Vc607->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i621_Vc621 = new TGraphErrors(i621_vec.size(), i621_vec.data(), Vc621_vec.data(), 0, 0);
	g_i621_Vc621->SetMarkerColor(color621);
	g_i621_Vc621->SetMarkerStyle(20);
	g_i621_Vc621->SetMarkerSize(0.4);
	g_i621_Vc621->SetMarkerStyle(20);
	g_i621_Vc621->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i621_Vc621->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i639_Vc639 = new TGraphErrors(i639_vec.size(), i639_vec.data(), Vc639_vec.data(), 0, 0);
	g_i639_Vc639->SetMarkerColor(color639);
	g_i639_Vc639->SetMarkerStyle(20);
	g_i639_Vc639->SetMarkerSize(0.4);
	g_i639_Vc639->SetMarkerStyle(20);
	g_i639_Vc639->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i639_Vc639->GetYaxis()->SetTitle("i (nA)");

	TGraphErrors * g_i656_Vc656 = new TGraphErrors(i656_vec.size(), i656_vec.data(), Vc656_vec.data(), s_i656_vec.data(), sVc656_vec.data());
	g_i656_Vc656->SetMarkerColor(color656);
	g_i656_Vc656->SetMarkerStyle(20);
	g_i656_Vc656->SetMarkerSize(0.4);
	g_i656_Vc656->SetMarkerStyle(20);
	g_i656_Vc656->GetXaxis()->SetTitle("V_{C} (mV)");
	g_i656_Vc656->GetYaxis()->SetTitle("i (nA)");




	TCanvas * cVc_i_gruppo2 = new TCanvas("cVc_i_gruppo2", "cVc_i_gruppo2", 0, 0, 1280, 720);
	cVc_i_gruppo2->cd();

	string mgVc_i_gruppo2_title_str432 = "#color[" + to_string((int) color432) + "]{#bullet #lambda_{p} = " + to_string((int) lambda432) + " (nm)}  ";
	string mgVc_i_gruppo2_title_str477 = "#color[" + to_string((int) color477) + "]{#bullet #lambda_{p} = " + to_string((int) lambda477) + " (nm)}  ";
	string mgVc_i_gruppo2_title_str552 = "#color[" + to_string((int) color552) + "]{#bullet #lambda_{p} = " + to_string((int) lambda552) + " (nm)}  ";
	string mgVc_i_gruppo2_title_str592 = "#color[" + to_string((int) color592) + "]{#bullet #lambda_{p} = " + to_string((int) lambda590) + " (nm)}  ";
	string mgVc_i_gruppo2_title_str607 = "#color[" + to_string((int) color607) + "]{#bullet #lambda_{p} = " + to_string((int) lambda607) + " (nm)}  ";
	string mgVc_i_gruppo2_title_str621 = "#color[" + to_string((int) color621) + "]{#bullet #lambda_{p} = " + to_string((int) lambda621) + " (nm)}  ";
	string mgVc_i_gruppo2_title_str639 = "#color[" + to_string((int) color639) + "]{#bullet #lambda_{p} = " + to_string((int) lambda639) + " (nm)}  ";


	string title_canvas_str_gruppo2 = "V_{C} vs i   "
		+ mgVc_i_gruppo2_title_str432
		+ mgVc_i_gruppo2_title_str477
		+ mgVc_i_gruppo2_title_str552
		+ mgVc_i_gruppo2_title_str592
		+ mgVc_i_gruppo2_title_str607
		+ mgVc_i_gruppo2_title_str621
		+ mgVc_i_gruppo2_title_str639;

	char title_canvas_char_gruppo2[title_canvas_str_gruppo2.size() + 1];
	std::copy(title_canvas_str_gruppo2.begin(), title_canvas_str_gruppo2.end(), title_canvas_char_gruppo2);
	title_canvas_char_gruppo2[title_canvas_str_gruppo2.size()] = '\0';

	TMultiGraph * mgVc_i_gruppo2 = new TMultiGraph("mgVc_i_gruppo2", title_canvas_char_gruppo2);
	mgVc_i_gruppo2->GetXaxis()->SetTitle("i (nA)");
	mgVc_i_gruppo2->GetYaxis()->SetTitle("V_{C} (mV)");
	mgVc_i_gruppo2->Add(g_i432_Vc432);
	mgVc_i_gruppo2->Add(g_i477_Vc477);
	mgVc_i_gruppo2->Add(g_i552_Vc552);
	mgVc_i_gruppo2->Add(g_i592_Vc592);
	mgVc_i_gruppo2->Add(g_i607_Vc607);
	mgVc_i_gruppo2->Add(g_i621_Vc621);
	mgVc_i_gruppo2->Add(g_i639_Vc639);

	mgVc_i_gruppo2->Draw("AP");





	TCanvas * cVc_i_gruppoD7_1st_dataset = new TCanvas("cVc_i_gruppoD7_1st_dataset", "cVc_i_gruppoD7_1st_dataset", 0, 0, 1280, 720);
	cVc_i_gruppoD7_1st_dataset->cd();

	string mgVc_i_gruppoD7_1st_dataset_title_str551 = "#color[" + to_string((int) color551) + "]{#bullet #lambda_{p} = " + to_string((int) lambda551) + " (nm)}  ";
	string mgVc_i_gruppoD7_1st_dataset_title_str561 = "#color[" + to_string((int) color561) + "]{#bullet #lambda_{p} = " + to_string((int) lambda561) + " (nm)}  ";
	string mgVc_i_gruppoD7_1st_dataset_title_str590 = "#color[" + to_string((int) color607) + "]{#bullet #lambda_{p} = " + to_string((int) lambda607) + " (nm)}  ";
	string mgVc_i_gruppoD7_1st_dataset_title_str656 = "#color[" + to_string((int) color656) + "]{#bullet #lambda_{p} = " + to_string((int) lambda656) + " (nm)}  ";

	string title_canvas_str_gruppoD7_1st_dataset = "V_{C} vs i   "
		+ mgVc_i_gruppoD7_1st_dataset_title_str551
		+ mgVc_i_gruppoD7_1st_dataset_title_str561
		+ mgVc_i_gruppoD7_1st_dataset_title_str590
		+ mgVc_i_gruppoD7_1st_dataset_title_str656;



	char title_canvas_char_gruppoD7_1st_dataset[title_canvas_str_gruppoD7_1st_dataset.size() + 1];
	std::copy(title_canvas_str_gruppoD7_1st_dataset.begin(), title_canvas_str_gruppoD7_1st_dataset.end(), title_canvas_char_gruppoD7_1st_dataset);
	title_canvas_char_gruppoD7_1st_dataset[title_canvas_str_gruppoD7_1st_dataset.size()] = '\0';

	TMultiGraph * mgVc_i_gruppoD7_1st_dataset = new TMultiGraph("mgVc_i_gruppoD7_1st_dataset", title_canvas_char_gruppoD7_1st_dataset);
	mgVc_i_gruppoD7_1st_dataset->GetXaxis()->SetTitle("i (nA)");
	mgVc_i_gruppoD7_1st_dataset->GetYaxis()->SetTitle("V_{C} (mV)");
	mgVc_i_gruppoD7_1st_dataset->Add(g_i551_Vc551);
	mgVc_i_gruppoD7_1st_dataset->Add(g_i561_Vc561);
	mgVc_i_gruppoD7_1st_dataset->Add(g_i590_Vc590);
	mgVc_i_gruppoD7_1st_dataset->Add(g_i656_Vc656);

	mgVc_i_gruppoD7_1st_dataset->Draw("AP");






	TCanvas * cVc_i_gruppoD7_2nd_dataset = new TCanvas("cVc_i_gruppoD7_2nd_dataset", "cVc_i_gruppoD7_2nd_dataset", 0, 0, 1280, 720);
	cVc_i_gruppoD7_2nd_dataset->cd();

	string mgVc_i_gruppoD7_2nd_dataset_title_str430 = "#color[" + to_string((int) color430) + "]{#bullet #lambda_{p} = " + to_string((int) lambda430) + " (nm)}  ";
	string mgVc_i_gruppoD7_2nd_dataset_title_str464 = "#color[" + to_string((int) color464) + "]{#bullet #lambda_{p} = " + to_string((int) lambda464) + " (nm)}  ";

	string title_canvas_str_gruppoD7_2nd_dataset = "V_{C} vs i   "
		+ mgVc_i_gruppoD7_2nd_dataset_title_str430
		+ mgVc_i_gruppoD7_2nd_dataset_title_str464;


	char title_canvas_char_gruppoD7_2nd_dataset[title_canvas_str_gruppoD7_2nd_dataset.size() + 1];
	std::copy(title_canvas_str_gruppoD7_2nd_dataset.begin(), title_canvas_str_gruppoD7_2nd_dataset.end(), title_canvas_char_gruppoD7_2nd_dataset);
	title_canvas_char_gruppoD7_2nd_dataset[title_canvas_str_gruppoD7_2nd_dataset.size()] = '\0';

	TMultiGraph * mgVc_i_gruppoD7_2nd_dataset = new TMultiGraph("mgVc_i_gruppoD7_2nd_dataset", title_canvas_char_gruppoD7_2nd_dataset);
	mgVc_i_gruppoD7_2nd_dataset->GetXaxis()->SetTitle("i (nA)");
	mgVc_i_gruppoD7_2nd_dataset->GetYaxis()->SetTitle("V_{C} (mV)");
	mgVc_i_gruppoD7_2nd_dataset->Add(g_i430_Vc430);
	mgVc_i_gruppoD7_2nd_dataset->Add(g_i464_Vc464);

	mgVc_i_gruppoD7_2nd_dataset->Draw("AP");






	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 430 _________________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_Vc0_and_errors_1st_method430_1 = new TCanvas("c_Vc0_and_errors_1st_method430_1", "c_Vc0_and_errors_1st_method430_1", 0, 0, 1280, 720);
	TCanvas * c_Vc0_and_errors_1st_method430_2 = new TCanvas("c_Vc0_and_errors_1st_method430_2", "c_Vc0_and_errors_1st_method430_2", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_1st_method430_vec = computeVc0AndErrorsFirstMethod(-20, color430, lambda430, c_Vc0_and_errors_1st_method430_1, c_Vc0_and_errors_1st_method430_2, Vc430_vec, i430_vec, sVc430_vec, s_i430_vec, 0, 2, 20, 37, -0.01, 0.3);


	TCanvas * c_Vc0_and_errors_2nd_method430 = new TCanvas("c_Vc0_and_errors_2nd_method430", "c_Vc0_and_errors_2nd_method430", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_2nd_method430_vec = computeVc0AndErrorsSecondMethod(lambda430, color430, c_Vc0_and_errors_2nd_method430, Vc430_vec, i430_vec);


	TCanvas * c_Vc0_and_errors_3rd_method430 = new TCanvas("c_Vc0_and_errors_3rd_method430", "c_Vc0_and_errors_3rd_method430", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_3rd_method430_vec = computeVc0AndErrorsThirdMethod(lambda430, color430, c_Vc0_and_errors_3rd_method430, Vc430_vec, i430_vec, sVc430_vec, s_i430_vec, 0.00001);



	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 432 _________________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_Vc0_and_errors_1st_method432_1 = new TCanvas("c_Vc0_and_errors_1st_method432_1", "c_Vc0_and_errors_1st_method432_1", 0, 0, 1280, 720);
	TCanvas * c_Vc0_and_errors_1st_method432_2 = new TCanvas("c_Vc0_and_errors_1st_method432_2", "c_Vc0_and_errors_1st_method432_2", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_1st_method432_vec = computeVc0AndErrorsFirstMethod(-20, color432, lambda432, c_Vc0_and_errors_1st_method432_1, c_Vc0_and_errors_1st_method432_2, Vc430_vec, i432_vec, sVc430_vec, s_i432_vec, 0, 2, 20, 37, -0.07, 1.5);


	TCanvas * c_Vc0_and_errors_2nd_method432 = new TCanvas("c_Vc0_and_errors_2nd_method432", "c_Vc0_and_errors_2nd_method432", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_2nd_method432_vec = computeVc0AndErrorsSecondMethod(lambda432, color432, c_Vc0_and_errors_2nd_method432, Vc432_vec, i432_vec);


	TCanvas * c_Vc0_and_errors_3rd_method432 = new TCanvas("c_Vc0_and_errors_3rd_method432", "c_Vc0_and_errors_3rd_method432", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_3rd_method432_vec = computeVc0AndErrorsThirdMethod(lambda432, color432, c_Vc0_and_errors_3rd_method432, Vc432_vec, i432_vec, sVc432_vec, s_i432_vec, 0.001);



	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 464 _________________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_Vc0_and_errors_1st_method464_1 = new TCanvas("c_Vc0_and_errors_1st_method464_1", "c_Vc0_and_errors_1st_method464_1", 0, 0, 1280, 720);
	TCanvas * c_Vc0_and_errors_1st_method464_2 = new TCanvas("c_Vc0_and_errors_1st_method464_2", "c_Vc0_and_errors_1st_method464_2", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_1st_method464_vec = computeVc0AndErrorsFirstMethod(-20, color464, lambda464, c_Vc0_and_errors_1st_method464_1, c_Vc0_and_errors_1st_method464_2, Vc430_vec, i464_vec, sVc430_vec, s_i464_vec, 0, 2, 20, 37, -0.01, 0.3);


	TCanvas * c_Vc0_and_errors_2nd_method464 = new TCanvas("c_Vc0_and_errors_2nd_method464", "c_Vc0_and_errors_2nd_method464", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_2nd_method464_vec = computeVc0AndErrorsSecondMethod(lambda464, color464, c_Vc0_and_errors_2nd_method464, Vc464_vec, i464_vec);


	TCanvas * c_Vc0_and_errors_3rd_method464 = new TCanvas("c_Vc0_and_errors_3rd_method464", "c_Vc0_and_errors_3rd_method464", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_3rd_method464_vec = computeVc0AndErrorsThirdMethod(lambda464, color464, c_Vc0_and_errors_3rd_method464, Vc464_vec, i464_vec, sVc464_vec, s_i464_vec, 0.00001);



	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 477(nm)______________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_Vc0_and_errors_1st_method477_1 = new TCanvas("c_Vc0_and_errors_1st_method477_1", "c_Vc0_and_errors_1st_method477_1", 0, 0, 1280, 720);
	TCanvas * c_Vc0_and_errors_1st_method477_2 = new TCanvas("c_Vc0_and_errors_1st_method477_2", "c_Vc0_and_errors_1st_method477_2", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_1st_method477_vec = computeVc0AndErrorsFirstMethod(-20, color477, lambda477, c_Vc0_and_errors_1st_method477_1, c_Vc0_and_errors_1st_method477_2, Vc477_vec, i477_vec, sVc477_vec, s_i477_vec, 0, 2, 20, 37, -0.07, 1.5);


	TCanvas * c_Vc0_and_errors_2nd_method477 = new TCanvas("c_Vc0_and_errors_2nd_method477", "c_Vc0_and_errors_2nd_method477", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_2nd_method477_vec = computeVc0AndErrorsSecondMethod(lambda477, color477, c_Vc0_and_errors_2nd_method477, Vc477_vec, i477_vec);


	TCanvas * c_Vc0_and_errors_3rd_method477 = new TCanvas("c_Vc0_and_errors_3rd_method477", "c_Vc0_and_errors_3rd_method477", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_3rd_method477_vec = computeVc0AndErrorsThirdMethod(lambda477, color477, c_Vc0_and_errors_3rd_method477, Vc477_vec, i477_vec, sVc477_vec, s_i477_vec, 0.001);



	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 551(nm)______________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_Vc0_and_errors_1st_method551_1 = new TCanvas("c_Vc0_and_errors_1st_method551_1", "c_Vc0_and_errors_1st_method551_1", 0, 0, 1280, 720);
	TCanvas * c_Vc0_and_errors_1st_method551_2 = new TCanvas("c_Vc0_and_errors_1st_method551_2", "c_Vc0_and_errors_1st_method551_2", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_1st_method551_vec = computeVc0AndErrorsFirstMethod(-20, color551, lambda551, c_Vc0_and_errors_1st_method551_1, c_Vc0_and_errors_1st_method551_2, Vc430_vec, i430_vec, sVc430_vec, s_i430_vec, 0, 2, 20, 37, -0.01, 0.3);


	TCanvas * c_Vc0_and_errors_2nd_method551 = new TCanvas("c_Vc0_and_errors_2nd_method551", "c_Vc0_and_errors_2nd_method551", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_2nd_method551_vec = computeVc0AndErrorsSecondMethod(lambda551, color551, c_Vc0_and_errors_2nd_method551, Vc551_vec, i551_vec);


	TCanvas * c_Vc0_and_errors_3rd_method551 = new TCanvas("c_Vc0_and_errors_3rd_method551", "c_Vc0_and_errors_3rd_method551", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_3rd_method551_vec = computeVc0AndErrorsThirdMethod(lambda551, color551, c_Vc0_and_errors_3rd_method551, Vc551_vec, i551_vec, sVc551_vec, s_i551_vec, 0.001);



	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 552(nm)______________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_Vc0_and_errors_1st_method552_1 = new TCanvas("c_Vc0_and_errors_1st_method552_1", "c_Vc0_and_errors_1st_method552_1", 0, 0, 1280, 720);
	TCanvas * c_Vc0_and_errors_1st_method552_2 = new TCanvas("c_Vc0_and_errors_1st_method552_2", "c_Vc0_and_errors_1st_method552_2", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_1st_method552_vec = computeVc0AndErrorsFirstMethod(-20, color552, lambda552, c_Vc0_and_errors_1st_method552_1, c_Vc0_and_errors_1st_method552_2, Vc552_vec, i552_vec, sVc552_vec, s_i552_vec, 0, 2, 14, 30, -0.07, 1.5);


	TCanvas * c_Vc0_and_errors_2nd_method552 = new TCanvas("c_Vc0_and_errors_2nd_method552", "c_Vc0_and_errors_2nd_method552", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_2nd_method552_vec = computeVc0AndErrorsSecondMethod(lambda552, color552, c_Vc0_and_errors_2nd_method552, Vc552_vec, i552_vec);


	TCanvas * c_Vc0_and_errors_3rd_method552 = new TCanvas("c_Vc0_and_errors_3rd_method552", "c_Vc0_and_errors_3rd_method552", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_3rd_method552_vec = computeVc0AndErrorsThirdMethod(lambda552, color552, c_Vc0_and_errors_3rd_method552, Vc552_vec, i552_vec, sVc552_vec, s_i552_vec, 0.001);




	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 561(nm)______________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_Vc0_and_errors_1st_method561_1 = new TCanvas("c_Vc0_and_errors_1st_method561_1", "c_Vc0_and_errors_1st_method561_1", 0, 0, 1280, 720);
	TCanvas * c_Vc0_and_errors_1st_method561_2 = new TCanvas("c_Vc0_and_errors_1st_method561_2", "c_Vc0_and_errors_1st_method561_2", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_1st_method561_vec = computeVc0AndErrorsFirstMethod(-20, color561, lambda561, c_Vc0_and_errors_1st_method561_1, c_Vc0_and_errors_1st_method561_2, Vc561_vec, i561_vec, sVc561_vec, s_i561_vec, 0, 2, 14, 30, -0.01, 0.3);


	TCanvas * c_Vc0_and_errors_2nd_method561 = new TCanvas("c_Vc0_and_errors_2nd_method561", "c_Vc0_and_errors_2nd_method561", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_2nd_method561_vec = computeVc0AndErrorsSecondMethod(lambda561, color561, c_Vc0_and_errors_2nd_method561, Vc561_vec, i561_vec);


	TCanvas * c_Vc0_and_errors_3rd_method561 = new TCanvas("c_Vc0_and_errors_3rd_method561", "c_Vc0_and_errors_3rd_method561", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_3rd_method561_vec = computeVc0AndErrorsThirdMethod(lambda561, color561, c_Vc0_and_errors_3rd_method561, Vc561_vec, i561_vec, sVc561_vec, s_i561_vec, 0.001);



	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 590 (nm)_____________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_Vc0_and_errors_1st_method590_1 = new TCanvas("c_Vc0_and_errors_1st_method590_1", "c_Vc0_and_errors_1st_method590_1", 0, 0, 1280, 720);
	TCanvas * c_Vc0_and_errors_1st_method590_2 = new TCanvas("c_Vc0_and_errors_1st_method590_2", "c_Vc0_and_errors_1st_method590_2", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_1st_method590_vec = computeVc0AndErrorsFirstMethod(-20, color590, lambda590, c_Vc0_and_errors_1st_method590_1, c_Vc0_and_errors_1st_method590_2, Vc590_vec, i590_vec, sVc590_vec, s_i590_vec, 0, 2, 10, 39, -0.01, 0.3);


	TCanvas * c_Vc0_and_errors_2nd_method590 = new TCanvas("c_Vc0_and_errors_2nd_method590", "c_Vc0_and_errors_2nd_method590", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_2nd_method590_vec = computeVc0AndErrorsSecondMethod(lambda590, color590, c_Vc0_and_errors_2nd_method590, Vc590_vec, i590_vec);


	TCanvas * c_Vc0_and_errors_3rd_method590 = new TCanvas("c_Vc0_and_errors_3rd_method590", "c_Vc0_and_errors_3rd_method590", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_3rd_method590_vec = computeVc0AndErrorsThirdMethod(lambda590, color590, c_Vc0_and_errors_3rd_method590, Vc590_vec, i590_vec, sVc590_vec, s_i590_vec, 0.001);



	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 592 (nm)_____________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_Vc0_and_errors_1st_method592_1 = new TCanvas("c_Vc0_and_errors_1st_method592_1", "c_Vc0_and_errors_1st_method592_1", 0, 0, 1280, 720);
	TCanvas * c_Vc0_and_errors_1st_method592_2 = new TCanvas("c_Vc0_and_errors_1st_method592_2", "c_Vc0_and_errors_1st_method592_2", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_1st_method592_vec = computeVc0AndErrorsFirstMethod(-20, color592, lambda592, c_Vc0_and_errors_1st_method592_1, c_Vc0_and_errors_1st_method592_2, Vc592_vec, i592_vec, sVc592_vec, s_i592_vec, 0, 2, 20, 39, -0.07, 1.5);


	TCanvas * c_Vc0_and_errors_2nd_method592 = new TCanvas("c_Vc0_and_errors_2nd_method592", "c_Vc0_and_errors_2nd_method592", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_2nd_method592_vec = computeVc0AndErrorsSecondMethod(lambda592, color592, c_Vc0_and_errors_2nd_method592, Vc592_vec, i592_vec);


	TCanvas * c_Vc0_and_errors_3rd_method592 = new TCanvas("c_Vc0_and_errors_3rd_method592", "c_Vc0_and_errors_3rd_method592", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_3rd_method592_vec = computeVc0AndErrorsThirdMethod(lambda592, color592, c_Vc0_and_errors_3rd_method592, Vc592_vec, i592_vec, sVc592_vec, s_i592_vec, 0.001);


	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 607 (nm)______________________________________________________________________________" << endl;
	std::cout << "___________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_Vc0_and_errors_1st_method607_1 = new TCanvas("c_Vc0_and_errors_1st_method607_1", "c_Vc0_and_errors_1st_method607_1", 0, 0, 1280, 720);
	TCanvas * c_Vc0_and_errors_1st_method607_2 = new TCanvas("c_Vc0_and_errors_1st_method607_2", "c_Vc0_and_errors_1st_method607_2", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_1st_method607_vec = computeVc0AndErrorsFirstMethod(-20, color607, lambda607, c_Vc0_and_errors_1st_method607_1, c_Vc0_and_errors_1st_method607_2, Vc607_vec, i607_vec, sVc607_vec, s_i607_vec, 0, 2, 16, 36, -0.07, 1.5);

	TCanvas * c_Vc0_and_errors_2nd_method607 = new TCanvas("c_Vc0_and_errors_2nd_method607", "c_Vc0_and_errors_2nd_method607", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_2nd_method607_vec = computeVc0AndErrorsSecondMethod(lambda607, color607, c_Vc0_and_errors_2nd_method607, Vc607_vec, i607_vec);


	TCanvas * c_Vc0_and_errors_3rd_method607 = new TCanvas("c_Vc0_and_errors_3rd_method607", "c_Vc0_and_errors_3rd_method607", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_3rd_method607_vec = computeVc0AndErrorsThirdMethod(lambda607, color607, c_Vc0_and_errors_3rd_method607, Vc607_vec, i607_vec, sVc607_vec, s_i607_vec, 0.001);


	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "_____________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 621(nm)_______________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_Vc0_and_errors_1st_method621_1 = new TCanvas("c_Vc0_and_errors_1st_method621_1", "c_Vc0_and_errors_1st_method621_1", 0, 0, 1280, 720);
	TCanvas * c_Vc0_and_errors_1st_method621_2 = new TCanvas("c_Vc0_and_errors_1st_method621_2", "c_Vc0_and_errors_1st_method621_2", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_1st_method621_vec = computeVc0AndErrorsFirstMethod(-20, color621, lambda621, c_Vc0_and_errors_1st_method621_1, c_Vc0_and_errors_1st_method621_2, Vc621_vec, i621_vec, sVc621_vec, s_i621_vec, 0, 2, 15, 36, -0.07, 1.5);


	TCanvas * c_Vc0_and_errors_2nd_method621 = new TCanvas("c_Vc0_and_errors_2nd_method621", "c_Vc0_and_errors_2nd_method621", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_2nd_method621_vec = computeVc0AndErrorsSecondMethod(lambda621, color621, c_Vc0_and_errors_2nd_method621, Vc621_vec, i621_vec);

	TCanvas * c_Vc0_and_errors_3rd_method621 = new TCanvas("c_Vc0_and_errors_3rd_method621", "c_Vc0_and_errors_3rd_method621", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_3rd_method621_vec = computeVc0AndErrorsThirdMethod(lambda621, color621, c_Vc0_and_errors_3rd_method621, Vc621_vec, i621_vec, sVc621_vec, s_i621_vec, 0.001);



	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "_____________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 639(nm)_______________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_Vc0_and_errors_1st_method639_1 = new TCanvas("c_Vc0_and_errors_1st_method639_1", "c_Vc0_and_errors_1st_method639_1", 0, 0, 1280, 720);
	TCanvas * c_Vc0_and_errors_1st_method639_2 = new TCanvas("c_Vc0_and_errors_1st_method639_2", "c_Vc0_and_errors_1st_method639_2", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_1st_method639_vec = computeVc0AndErrorsFirstMethod(-20, color639, lambda639, c_Vc0_and_errors_1st_method639_1, c_Vc0_and_errors_1st_method639_2, Vc639_vec, i639_vec, sVc639_vec, s_i639_vec, 0, 2, 14, 33, -0.07, 1.5);


	TCanvas * c_Vc0_and_errors_2nd_method639 = new TCanvas("c_Vc0_and_errors_2nd_method639", "c_Vc0_and_errors_2nd_method639", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_2nd_method639_vec = computeVc0AndErrorsSecondMethod(lambda639, color639, c_Vc0_and_errors_2nd_method639, Vc639_vec, i639_vec);


	TCanvas * c_Vc0_and_errors_3rd_method639 = new TCanvas("c_Vc0_and_errors_3rd_method639", "c_Vc0_and_errors_3rd_method639", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_3rd_method639_vec = computeVc0AndErrorsThirdMethod(lambda639, color639, c_Vc0_and_errors_3rd_method639, Vc639_vec, i639_vec, sVc639_vec, s_i639_vec, 0.001);



	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "_____________________________________________________________RICERCA DI Vc0, LED A LAMBDA = 656(nm)_______________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_Vc0_and_errors_1st_method656_1 = new TCanvas("c_Vc0_and_errors_1st_method656_1", "c_Vc0_and_errors_1st_method656_1", 0, 0, 1280, 720);
	TCanvas * c_Vc0_and_errors_1st_method656_2 = new TCanvas("c_Vc0_and_errors_1st_method656_2", "c_Vc0_and_errors_1st_method656_2", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_1st_method656_vec = computeVc0AndErrorsFirstMethod(-20, color656, lambda656, c_Vc0_and_errors_1st_method656_1, c_Vc0_and_errors_1st_method656_2, Vc656_vec, i656_vec, sVc656_vec, s_i656_vec, 0, 2, 14, 33, -0.01, 0.3);


	TCanvas * c_Vc0_and_errors_2nd_method656 = new TCanvas("c_Vc0_and_errors_2nd_method656", "c_Vc0_and_errors_2nd_method656", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_2nd_method656_vec = computeVc0AndErrorsSecondMethod(lambda656, color656, c_Vc0_and_errors_2nd_method656, Vc656_vec, i656_vec);


	TCanvas * c_Vc0_and_errors_3rd_method656 = new TCanvas("c_Vc0_and_errors_3rd_method656", "c_Vc0_and_errors_3rd_method656", 0, 0, 1280, 720);
	vector<float> Vc0_and_errors_3rd_method656_vec = computeVc0AndErrorsThirdMethod(lambda656, color656, c_Vc0_and_errors_3rd_method656, Vc656_vec, i656_vec, sVc656_vec, s_i656_vec, 0.001);




	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "_________________________________________________________________RICERCA DI HBAR, DATI GRUPPO 2___________________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	//lambda ed errori
	vector<float> lambda_gruppo2_vec{lambda432, lambda477, lambda552, lambda592, lambda607, lambda621, lambda639};
	vector<float> s_left_lambda_gruppo2_vec{s_left_lambda432, s_left_lambda477, s_left_lambda552, s_left_lambda592, s_left_lambda607, s_left_lambda621, s_left_lambda639};
	vector<float> s_right_lambda_gruppo2_vec{s_right_lambda432, s_right_lambda477, s_right_lambda552, s_right_lambda592, s_right_lambda607, s_right_lambda621, s_right_lambda639};
	vector<float> colors_gruppo2_vec{color432, color477, color552, color592, color607, color621, color639};

	//Vc0 ed errori
		//I method
	vector<float> Vc0_1st_method_gruppo2_vec;
	Vc0_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method432_vec[0]);
	Vc0_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method477_vec[0]);
	Vc0_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method552_vec[0]);
	Vc0_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method592_vec[0]);
	Vc0_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method607_vec[0]);
	Vc0_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method621_vec[0]);
	Vc0_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method639_vec[0]);

	vector<float> s_Vc0_left_1st_method_gruppo2_vec;
	s_Vc0_left_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method432_vec[1]);
	s_Vc0_left_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method477_vec[1]);
	s_Vc0_left_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method552_vec[1]);
	s_Vc0_left_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method592_vec[1]);
	s_Vc0_left_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method607_vec[1]);
	s_Vc0_left_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method621_vec[1]);
	s_Vc0_left_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method639_vec[1]);

	vector<float> s_Vc0_right_1st_method_gruppo2_vec;
	s_Vc0_right_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method432_vec[2]);
	s_Vc0_right_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method477_vec[2]);
	s_Vc0_right_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method552_vec[2]);
	s_Vc0_right_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method592_vec[2]);
	s_Vc0_right_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method607_vec[2]);
	s_Vc0_right_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method621_vec[2]);
	s_Vc0_right_1st_method_gruppo2_vec.push_back(Vc0_and_errors_1st_method639_vec[2]);

	std::cout << "-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-" << endl;
	vectorPrint("Vc0_1st_method_gruppo2_vec", Vc0_1st_method_gruppo2_vec);
	vectorPrint("s_Vc0_left_1st_method_gruppo2_vec", s_Vc0_left_1st_method_gruppo2_vec);
	vectorPrint("s_Vc0_right_1st_method_gruppo2_vec", s_Vc0_right_1st_method_gruppo2_vec);


	TCanvas * c_planck_constant_and_extraction_work_1st_method1_gruppo2_meV = new TCanvas("c_planck_constant_and_extraction_work_1st_method1_gruppo2_meV", "dataset gruppo2 1st method meV", 0, 0, 1280, 720);
	TCanvas * c_planck_constant_and_extraction_work_1st_method2_gruppo2_J = new TCanvas("c_planck_constant_and_extraction_work_1st_method2_gruppo2_J", "dataset gruppo2 1st method J", 0, 0, 1280, 720);
	vector<float> c_planck_constant_and_extraction_work_1st_method_gruppo2 = computePlanckConstantAndExtractionWork(c_planck_constant_and_extraction_work_1st_method1_gruppo2_meV, c_planck_constant_and_extraction_work_1st_method2_gruppo2_J, colors_gruppo2_vec, lambda_gruppo2_vec, s_left_lambda_gruppo2_vec, s_right_lambda_gruppo2_vec, Vc0_1st_method_gruppo2_vec, s_Vc0_left_1st_method_gruppo2_vec, s_Vc0_right_1st_method_gruppo2_vec);




	//2nd method
	vector<float> Vc0_2nd_method_gruppo2_vec;
	Vc0_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method432_vec[0]);
	Vc0_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method477_vec[0]);
	Vc0_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method552_vec[0]);
	Vc0_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method592_vec[0]);
	Vc0_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method607_vec[0]);
	Vc0_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method621_vec[0]);
	Vc0_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method639_vec[0]);

	vector<float> s_Vc0_left_2nd_method_gruppo2_vec;
	s_Vc0_left_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method432_vec[1]);
	s_Vc0_left_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method477_vec[1]);
	s_Vc0_left_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method552_vec[1]);
	s_Vc0_left_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method592_vec[1]);
	s_Vc0_left_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method607_vec[1]);
	s_Vc0_left_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method621_vec[1]);
	s_Vc0_left_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method639_vec[1]);

	vector<float> s_Vc0_right_2nd_method_gruppo2_vec;
	s_Vc0_right_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method432_vec[1]);
	s_Vc0_right_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method477_vec[1]);
	s_Vc0_right_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method552_vec[1]);
	s_Vc0_right_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method592_vec[1]);
	s_Vc0_right_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method607_vec[1]);
	s_Vc0_right_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method621_vec[1]);
	s_Vc0_right_2nd_method_gruppo2_vec.push_back(Vc0_and_errors_2nd_method639_vec[1]);

	std::cout << "-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-" << endl;
	vectorPrint("Vc0_2nd_method_gruppo2_vec", Vc0_2nd_method_gruppo2_vec);
	vectorPrint("s_Vc0_left_2nd_method_gruppo2_vec", s_Vc0_left_2nd_method_gruppo2_vec);
	vectorPrint("s_Vc0_right_2nd_method_gruppo2_vec", s_Vc0_right_2nd_method_gruppo2_vec);


	//2nd method
	TCanvas * c_planck_constant_and_extraction_work_2nd_method1_gruppo2_meV = new TCanvas("c_planck_constant_and_extraction_work_2nd_method1_gruppo2_meV", "dataset gruppo2 2st method meV", 0, 0, 1280, 720);
	TCanvas * c_planck_constant_and_extraction_work_2nd_method2_gruppo2_J = new TCanvas("c_planck_constant_and_extraction_work_2nd_method2_gruppo2_J", "dataset gruppo2 2st method J", 0, 0, 1280, 720);
	vector<float> planck_constant_and_extraction_work_2nd_method_gruppo2 = computePlanckConstantAndExtractionWork(c_planck_constant_and_extraction_work_2nd_method1_gruppo2_meV, c_planck_constant_and_extraction_work_2nd_method2_gruppo2_J, colors_gruppo2_vec, lambda_gruppo2_vec, s_left_lambda_gruppo2_vec, s_right_lambda_gruppo2_vec, Vc0_2nd_method_gruppo2_vec, s_Vc0_left_2nd_method_gruppo2_vec, s_Vc0_right_2nd_method_gruppo2_vec);


	//3rd method
	vector<float> Vc0_3rd_method_gruppo2_vec;
	Vc0_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method432_vec[0]);
	Vc0_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method477_vec[0]);
	Vc0_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method552_vec[0]);
	Vc0_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method592_vec[0]);
	Vc0_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method607_vec[0]);
	Vc0_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method621_vec[0]);
	Vc0_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method639_vec[0]);

	vector<float> s_Vc0_left_3rd_method_gruppo2_vec;
	s_Vc0_left_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method432_vec[1]);
	s_Vc0_left_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method477_vec[1]);
	s_Vc0_left_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method552_vec[1]);
	s_Vc0_left_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method592_vec[1]);
	s_Vc0_left_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method607_vec[1]);
	s_Vc0_left_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method621_vec[1]);
	s_Vc0_left_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method639_vec[1]);

	vector<float> s_Vc0_right_3rd_method_gruppo2_vec;
	s_Vc0_right_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method432_vec[2]);
	s_Vc0_right_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method477_vec[2]);
	s_Vc0_right_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method552_vec[2]);
	s_Vc0_right_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method592_vec[2]);
	s_Vc0_right_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method607_vec[2]);
	s_Vc0_right_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method621_vec[2]);
	s_Vc0_right_3rd_method_gruppo2_vec.push_back(Vc0_and_errors_3rd_method639_vec[2]);

	std::cout << "-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-" << endl;
	vectorPrint("Vc0_3rd_method_gruppo2_vec", Vc0_3rd_method_gruppo2_vec);
	vectorPrint("s_Vc0_left_3rd_method_gruppo2_vec", s_Vc0_left_3rd_method_gruppo2_vec);
	vectorPrint("s_Vc0_right_3rd_method_gruppo2_vec", s_Vc0_right_3rd_method_gruppo2_vec);


	//3rd method
	TCanvas * c_planck_constant_and_extraction_work_3rd_method1_gruppo2_meV = new TCanvas("c_planck_constant_and_extraction_work_3rd_method1_gruppo2_meV", "dataset gruppo2 2st method meV", 0, 0, 1280, 720);
	TCanvas * c_planck_constant_and_extraction_work_3rd_method2_gruppo2_J = new TCanvas("c_planck_constant_and_extraction_work_3rd_method2_gruppo2_J", "dataset gruppo2 2st method J", 0, 0, 1280, 720);
	vector<float> planck_constant_and_extraction_work_3rd_method_gruppo2 = computePlanckConstantAndExtractionWork(c_planck_constant_and_extraction_work_3rd_method1_gruppo2_meV, c_planck_constant_and_extraction_work_3rd_method2_gruppo2_J, colors_gruppo2_vec, lambda_gruppo2_vec, s_left_lambda_gruppo2_vec, s_right_lambda_gruppo2_vec, Vc0_3rd_method_gruppo2_vec, s_Vc0_left_3rd_method_gruppo2_vec, s_Vc0_right_3rd_method_gruppo2_vec);




	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "_________________________________________________________RICERCA DI HBAR, DATI GRUPPO D7, PRIMO DATASET___________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	//lambda ed errori
	vector<float> lambda_gruppoD7_1st_dataset_vec{lambda551, lambda561, lambda590, lambda656};
	vector<float> s_left_lambda_gruppoD7_1st_dataset_vec{s_left_lambda551, s_left_lambda561, s_left_lambda590, s_left_lambda656};
	vector<float> s_right_lambda_gruppoD7_1st_dataset_vec{s_right_lambda551, s_right_lambda561, s_right_lambda590, s_right_lambda656};
	vector<float> colors_gruppoD7_1st_dataset_vec{color551, color561, color590, color656};



	//gruppo D7 I dataset I method
	vector<float> Vc0_1st_method_gruppoD7_1st_dataset_vec;
	Vc0_1st_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_1st_method551_vec[0]);
	Vc0_1st_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_1st_method561_vec[0]);
	Vc0_1st_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_1st_method590_vec[0]);
	Vc0_1st_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_1st_method656_vec[0]);

	vector<float> s_Vc0_left_1st_method_gruppoD7_1st_dataset_vec;
	s_Vc0_left_1st_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_1st_method551_vec[1]);
	s_Vc0_left_1st_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_1st_method561_vec[1]);
	s_Vc0_left_1st_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_1st_method590_vec[1]);
	s_Vc0_left_1st_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_1st_method656_vec[1]);

	vector<float> s_Vc0_right_1st_method_gruppoD7_1st_dataset_vec;
	s_Vc0_right_1st_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_1st_method551_vec[2]);
	s_Vc0_right_1st_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_1st_method561_vec[2]);
	s_Vc0_right_1st_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_1st_method590_vec[2]);
	s_Vc0_right_1st_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_1st_method656_vec[2]);

	TCanvas * c_planck_constant_and_extraction_work_1st_method_gruppoD7_1st_dataset_meV = new TCanvas("c_planck_constant_and_extraction_work_1st_method_gruppoD7_1st_dataset_meV", "1st dataset gruppo D7 1st method meV", 0, 0, 1280, 720);
	TCanvas * c_planck_constant_and_extraction_work_1st_method2_gruppoD7_1st_dataset_J = new TCanvas("c_planck_constant_and_extraction_work_1st_method2_gruppoD7_1st_dataset_J", "1st dataset gruppo D7 1st method J", 0, 0, 1280, 720);
	vector<float> c_planck_constant_and_extraction_work_1st_method_gruppoD7_1st_dataset = computePlanckConstantAndExtractionWork(c_planck_constant_and_extraction_work_1st_method_gruppoD7_1st_dataset_meV, c_planck_constant_and_extraction_work_1st_method2_gruppoD7_1st_dataset_J, colors_gruppoD7_1st_dataset_vec, lambda_gruppoD7_1st_dataset_vec, s_left_lambda_gruppoD7_1st_dataset_vec, s_right_lambda_gruppoD7_1st_dataset_vec, Vc0_1st_method_gruppoD7_1st_dataset_vec, s_Vc0_left_1st_method_gruppoD7_1st_dataset_vec, s_Vc0_right_1st_method_gruppoD7_1st_dataset_vec);



	//gruppo D7 I dataset 2nd method
	vector<float> Vc0_2nd_method_gruppoD7_1st_dataset_vec;
	Vc0_2nd_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_2nd_method551_vec[0]);
	Vc0_2nd_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_2nd_method561_vec[0]);
	Vc0_2nd_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_2nd_method590_vec[0]);
	Vc0_2nd_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_2nd_method656_vec[0]);

	vector<float> s_Vc0_left_2nd_method_gruppoD7_1st_dataset_vec;
	s_Vc0_left_2nd_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_2nd_method551_vec[1]);
	s_Vc0_left_2nd_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_2nd_method561_vec[1]);
	s_Vc0_left_2nd_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_2nd_method590_vec[1]);
	s_Vc0_left_2nd_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_2nd_method656_vec[1]);

	vector<float> s_Vc0_right_2nd_method_gruppoD7_1st_dataset_vec;
	s_Vc0_right_2nd_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_2nd_method551_vec[1]);
	s_Vc0_right_2nd_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_2nd_method561_vec[1]);
	s_Vc0_right_2nd_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_2nd_method590_vec[1]);
	s_Vc0_right_2nd_method_gruppoD7_1st_dataset_vec.push_back(Vc0_and_errors_2nd_method656_vec[1]);

	TCanvas * c_planck_constant_and_extraction_work_2nd_method_gruppoD7_1st_dataset_meV = new TCanvas("c_planck_constant_and_extraction_work_2nd_method_gruppoD7_1st_dataset_meV", "1st dataset gruppo D7 2nd method meV", 0, 0, 1280, 720);
	TCanvas * c_planck_constant_and_extraction_work_2nd_method_gruppoD7_1st_dataset_J = new TCanvas("c_planck_constant_and_extraction_work_2nd_method_gruppoD7_1st_dataset_J", "1st dataset gruppo D7 2nd method J", 0, 0, 1280, 720);
	vector<float> planck_constant_and_extraction_work_2nd_method_gruppoD7_1st_dataset = computePlanckConstantAndExtractionWork(c_planck_constant_and_extraction_work_2nd_method_gruppoD7_1st_dataset_meV, c_planck_constant_and_extraction_work_2nd_method_gruppoD7_1st_dataset_J, colors_gruppoD7_1st_dataset_vec, lambda_gruppoD7_1st_dataset_vec, s_left_lambda_gruppoD7_1st_dataset_vec, s_right_lambda_gruppoD7_1st_dataset_vec, Vc0_2nd_method_gruppoD7_1st_dataset_vec, s_Vc0_left_2nd_method_gruppoD7_1st_dataset_vec, s_Vc0_right_2nd_method_gruppoD7_1st_dataset_vec);



	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "_________________________________________________________RICERCA DI HBAR, DATI GRUPPO D7, SECONDO DATASET_________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	//lambda ed errori
	vector<float> lambda_gruppoD7_2nd_dataset_vec{lambda430, lambda464,};
	vector<float> s_left_lambda_gruppoD7_2nd_dataset_vec{s_left_lambda430, s_left_lambda464};
	vector<float> s_right_lambda_gruppoD7_2nd_dataset_vec{s_right_lambda430, s_right_lambda464};
	vector<float> colors_gruppoD7_2nd_dataset_vec{color430, color464};


	//gruppo D7 2nd dataset I method
	vector<float> Vc0_1st_method_gruppoD7_2nd_dataset_vec;
	Vc0_1st_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_1st_method430_vec[0]);
	Vc0_1st_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_1st_method464_vec[0]);


	vector<float> s_Vc0_left_1st_method_gruppoD7_2nd_dataset_vec;
	s_Vc0_left_1st_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_1st_method430_vec[1]);
	s_Vc0_left_1st_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_1st_method464_vec[1]);

	vector<float> s_Vc0_right_1st_method_gruppoD7_2nd_dataset_vec;
	s_Vc0_right_1st_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_1st_method430_vec[2]);
	s_Vc0_right_1st_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_1st_method464_vec[2]);

	TCanvas * c_planck_constant_and_extraction_work_1st_method_gruppoD7_2nd_dataset_meV = new TCanvas("c_planck_constant_and_extraction_work_1st_method_gruppoD7_2nd_dataset_meV", "2nd dataset gruppo D7 2nd method meV", 0, 0, 1280, 720);
	TCanvas * c_planck_constant_and_extraction_work_1st_method2_gruppoD7_2nd_dataset_J = new TCanvas("c_planck_constant_and_extraction_work_1st_method2_gruppoD7_2nd_dataset_J", "2nd dataset gruppo D7 2nd method J", 0, 0, 1280, 720);
	vector<float> c_planck_constant_and_extraction_work_1st_method_gruppoD7_2nd_dataset = computePlanckConstantAndExtractionWork(c_planck_constant_and_extraction_work_1st_method_gruppoD7_2nd_dataset_meV, c_planck_constant_and_extraction_work_1st_method2_gruppoD7_2nd_dataset_J, colors_gruppoD7_2nd_dataset_vec, lambda_gruppoD7_2nd_dataset_vec, s_left_lambda_gruppoD7_2nd_dataset_vec, s_right_lambda_gruppoD7_2nd_dataset_vec, Vc0_1st_method_gruppoD7_2nd_dataset_vec, s_Vc0_left_1st_method_gruppoD7_2nd_dataset_vec, s_Vc0_right_1st_method_gruppoD7_2nd_dataset_vec);


	//gruppo D7 2nd dataset 2nd method
	vector<float> Vc0_2nd_method_gruppoD7_2nd_dataset_vec;
	Vc0_2nd_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_2nd_method430_vec[0]);
	Vc0_2nd_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_2nd_method464_vec[0]);

	vector<float> s_Vc0_left_2nd_method_gruppoD7_2nd_dataset_vec;
	s_Vc0_left_2nd_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_2nd_method430_vec[1]);
	s_Vc0_left_2nd_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_2nd_method464_vec[1]);

	vector<float> s_Vc0_right_2nd_method_gruppoD7_2nd_dataset_vec;
	s_Vc0_right_2nd_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_2nd_method430_vec[1]);
	s_Vc0_right_2nd_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_2nd_method464_vec[1]);

	TCanvas * c_planck_constant_and_extraction_work_2nd_method_gruppoD7_2nd_dataset_meV = new TCanvas("c_planck_constant_and_extraction_work_2nd_method_gruppoD7_2nd_dataset_meV", "c_planck_constant_and_extraction_work_2nd_method_gruppoD7_2nd_dataset_meV", 0, 0, 1280, 720);
	TCanvas * c_planck_constant_and_extraction_work_2nd_method_gruppoD7_2nd_dataset_J = new TCanvas("c_planck_constant_and_extraction_work_2nd_method_gruppoD7_2nd_dataset_J", "c_planck_constant_and_extraction_work_2nd_method_gruppoD7_2nd_dataset_J", 0, 0, 1280, 720);
	vector<float> planck_constant_and_extraction_work_2nd_method_gruppoD7_2nd_dataset = computePlanckConstantAndExtractionWork(c_planck_constant_and_extraction_work_2nd_method_gruppoD7_2nd_dataset_meV, c_planck_constant_and_extraction_work_2nd_method_gruppoD7_2nd_dataset_J, colors_gruppoD7_2nd_dataset_vec, lambda_gruppoD7_2nd_dataset_vec, s_left_lambda_gruppoD7_2nd_dataset_vec, s_right_lambda_gruppoD7_2nd_dataset_vec, Vc0_2nd_method_gruppoD7_2nd_dataset_vec, s_Vc0_left_2nd_method_gruppoD7_2nd_dataset_vec, s_Vc0_right_2nd_method_gruppoD7_2nd_dataset_vec);


	//3rd method
	vector<float> Vc0_3rd_method_gruppoD7_2nd_dataset_vec;
	Vc0_3rd_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_3rd_method430_vec[0]);
	Vc0_3rd_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_3rd_method464_vec[0]);

	vector<float> s_Vc0_left_3rd_method_gruppoD7_2nd_dataset_vec;
	s_Vc0_left_3rd_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_3rd_method430_vec[1]);
	s_Vc0_left_3rd_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_3rd_method464_vec[1]);

	vector<float> s_Vc0_right_3rd_method_gruppoD7_2nd_dataset_vec;
	s_Vc0_right_3rd_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_3rd_method430_vec[2]);
	s_Vc0_right_3rd_method_gruppoD7_2nd_dataset_vec.push_back(Vc0_and_errors_3rd_method464_vec[2]);

	std::cout << "-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-" << endl;
	vectorPrint("Vc0_3rd_method_gruppoD7_2nd_dataset_vec", Vc0_3rd_method_gruppoD7_2nd_dataset_vec);
	vectorPrint("s_Vc0_left_3rd_method_gruppoD7_2nd_dataset_vec", s_Vc0_left_3rd_method_gruppoD7_2nd_dataset_vec);
	vectorPrint("s_Vc0_right_3rd_method_gruppoD7_2nd_dataset_vec", s_Vc0_right_3rd_method_gruppoD7_2nd_dataset_vec);


	//3rd method
	TCanvas * c_planck_constant_and_extraction_work_3rd_method1_gruppoD7_2nd_dataset_meV = new TCanvas("c_planck_constant_and_extraction_work_3rd_method1_gruppoD7_2nd_dataset_meV", "dataset gruppoD7_2nd_dataset 2st method meV", 0, 0, 1280, 720);
	TCanvas * c_planck_constant_and_extraction_work_3rd_method2_gruppoD7_2nd_dataset_J = new TCanvas("c_planck_constant_and_extraction_work_3rd_method2_gruppoD7_2nd_dataset_J", "dataset gruppoD7_2nd_dataset 2st method J", 0, 0, 1280, 720);
	vector<float> planck_constant_and_extraction_work_3rd_method_gruppoD7_2nd_dataset = computePlanckConstantAndExtractionWork(c_planck_constant_and_extraction_work_3rd_method1_gruppoD7_2nd_dataset_meV, c_planck_constant_and_extraction_work_3rd_method2_gruppoD7_2nd_dataset_J, colors_gruppoD7_2nd_dataset_vec, lambda_gruppoD7_2nd_dataset_vec, s_left_lambda_gruppoD7_2nd_dataset_vec, s_right_lambda_gruppoD7_2nd_dataset_vec, Vc0_3rd_method_gruppoD7_2nd_dataset_vec, s_Vc0_left_3rd_method_gruppoD7_2nd_dataset_vec, s_Vc0_right_3rd_method_gruppoD7_2nd_dataset_vec);

}

/*
   map<int, vector<float>> = data_map;

   vector<float>::iterator it_Vc = V_C.begin();
   vector<float>::iterator it_i = I.begin();
   vector<float>::iterator it_sVc = sVc.begin();
   vector<float>::iterator it_s_i = s_i.begin();
   int i = 0;

   while(it_Vc < V_C.end()){
	   vector<float> Vc_i_sVc_s_i {*it_Vc, *it_i, *it_sVc, *s_i};
	   pair<int, vector<float>> pair_number_Vc_i_sVc_s_i = pair<int, vector<float>>(i, Vc_i_sVc_s_i);
	   data_map.insert(pair_number_Vc_i_sVc_s_i);
	   it_Vc++;
	   it_i++;
	   it_sVc++;
	   s_i++;
	   i++;
   }
   */

   //for(map<int, vector<float>>::iterator it = data_map.begin(); it < data_map; ++it)

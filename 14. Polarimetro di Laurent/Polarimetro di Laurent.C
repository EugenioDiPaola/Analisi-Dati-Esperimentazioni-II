#include <iostream>
#include <vector>
#include <string>
#include <cmath>

vector<float> recursiveFitHistogramWithGaussian1(TCanvas * CANVAS, vector<float> DATA, string TITLE, string X_AXIS_LABEL, float BIN_WIDTH, int N_OF_SIGMAS, float FROM, float TO)
{

	std::cout << endl << "C A L L E D" << endl << endl;

	float DATA_min = *min_element(begin(DATA), end(DATA));
	float DATA_max = *max_element(begin(DATA), end(DATA));

	std::cout << "DATA_min: " << DATA_min << endl;
	std::cout << "DATA_max: " << DATA_max << endl;

	vector<float> bins_low_edges_DATA;

	for(float i = DATA_min - 3 * BIN_WIDTH / 2; i < DATA_max + BIN_WIDTH / 2; i += BIN_WIDTH)
	{
		std::cout << i << endl;
		bins_low_edges_DATA.push_back(i);
	}

	std::cout << "bins_low_edges_DATA.size(): " << bins_low_edges_DATA.size() << endl;

	string title_canvas_str = TITLE;
	char title_canvas_char[title_canvas_str.size() + 1];
	std::copy(title_canvas_str.begin(), title_canvas_str.end(), title_canvas_char);
	title_canvas_char[title_canvas_str.size()] = '\0';


	float n_of_bins = bins_low_edges_DATA.size() - 1;

	TH1F * h_DATA = new TH1F("Stats", title_canvas_char, n_of_bins, bins_low_edges_DATA.data());
	for(auto & datum : DATA)
	{
		h_DATA->Fill(datum);
	}
	h_DATA->Fit("gaus");



	string x_axis_label_str = X_AXIS_LABEL;
	char x_axis_label_char[x_axis_label_str.size() + 1];
	std::copy(x_axis_label_str.begin(), x_axis_label_str.end(), x_axis_label_char);
	x_axis_label_char[x_axis_label_str.size()] = '\0';

	h_DATA->GetXaxis()->SetTitle(x_axis_label_char);
	h_DATA->GetYaxis()->SetTitle("frequenze (adim)");

	TF1 * fz_gauss_DATA = h_DATA->GetFunction("gaus");
	float chi2_fz_gaus_DATA = fz_gauss_DATA->GetChisquare();
	fz_gauss_DATA->SetRange(FROM, TO);

	std::cout << "chi2: " << chi2_fz_gaus_DATA << endl;

	int exit_or_no_exit = 0;

	float constant_of_fz_gauss_DATA = fz_gauss_DATA->GetParameter(0);
	float mean_of_fz_gauss_DATA = fz_gauss_DATA->GetParameter(1);
	float sigma_of_fz_gauss_DATA = fz_gauss_DATA->GetParameter(2);

	for(vector<float>::const_iterator it = DATA.begin(); it != DATA.end(); it++)
	{
		if(abs(*it - mean_of_fz_gauss_DATA) > N_OF_SIGMAS * sigma_of_fz_gauss_DATA)
		{
			DATA.erase(it);
			++exit_or_no_exit;
		}
	}

	if(exit_or_no_exit == 0)
	{
	};
	if(exit_or_no_exit != 0)
	{
		recursiveFitHistogramWithGaussian1(CANVAS, DATA, TITLE, X_AXIS_LABEL, BIN_WIDTH, N_OF_SIGMAS, FROM, TO);
	}

	float scarto_medio_assoluto = 0;
	for(auto & datum : DATA)
	{
		scarto_medio_assoluto += abs(datum - mean_of_fz_gauss_DATA);
	}
	scarto_medio_assoluto = scarto_medio_assoluto / DATA.size();


	vector<float> return_vec{constant_of_fz_gauss_DATA, mean_of_fz_gauss_DATA, sigma_of_fz_gauss_DATA, scarto_medio_assoluto};

	std::cout << "constant_of_fz_gauss_DATA: " << constant_of_fz_gauss_DATA << endl << "mean_of_fz_gauss_DATA: " << mean_of_fz_gauss_DATA << endl << "sigma_of_fz_gauss_DATA: "
		<< sigma_of_fz_gauss_DATA << endl << "scarto_medio_assoluto: " << scarto_medio_assoluto << endl;
	return return_vec;
}



vector<float> recursiveFitHistogramWithGaussian2(TCanvas * CANVAS, vector<float> DATA, string TITLE, string X_AXIS_LABEL, int N_OF_SIGMAS, float FROM, float TO)
{

	CANVAS->cd();
	std::cout << endl << "C A L L E D" << endl << endl;

	float DATA_min = *min_element(begin(DATA), end(DATA));
	float DATA_max = *max_element(begin(DATA), end(DATA));

	std::cout << "DATA_min: " << DATA_min << endl;
	std::cout << "DATA_max: " << DATA_max << endl;

	string title_canvas_str = TITLE;
	char title_canvas_char[title_canvas_str.size() + 1];
	std::copy(title_canvas_str.begin(), title_canvas_str.end(), title_canvas_char);
	title_canvas_char[title_canvas_str.size()] = '\0';


	int n_of_bins = round(sqrt(DATA.size()));
	std::cout << "AAAAAAAAA" << n_of_bins << endl;

	TH1F * h_DATA = new TH1F("Stats", title_canvas_char, n_of_bins, DATA_min, DATA_max);

	for(auto & datum : DATA)
	{
		h_DATA->Fill(datum);
	}
	h_DATA->Fit("gaus");


	string x_axis_label_str = X_AXIS_LABEL;
	char x_axis_label_char[x_axis_label_str.size() + 1];
	std::copy(x_axis_label_str.begin(), x_axis_label_str.end(), x_axis_label_char);
	x_axis_label_char[x_axis_label_str.size()] = '\0';

	h_DATA->GetXaxis()->SetTitle(x_axis_label_char);
	h_DATA->GetYaxis()->SetTitle("frequenze (adim)");

	TF1 * fz_gauss_DATA = h_DATA->GetFunction("gaus");
	float chi2_fz_gaus_DATA = fz_gauss_DATA->GetChisquare();
	//fz_gauss_DATA->SetRange(FROM, TO);

	std::cout << "chi2: " << chi2_fz_gaus_DATA << endl;

	int exit_or_no_exit = 0;

	float constant_of_fz_gauss_DATA = fz_gauss_DATA->GetParameter(0);
	float mean_of_fz_gauss_DATA = fz_gauss_DATA->GetParameter(1);
	float sigma_of_fz_gauss_DATA = fz_gauss_DATA->GetParameter(2);

	for(vector<float>::const_iterator it = DATA.begin(); it != DATA.end(); it++)
	{
		if(abs(*it - mean_of_fz_gauss_DATA) > N_OF_SIGMAS * sigma_of_fz_gauss_DATA)
		{
			DATA.erase(it);
			++exit_or_no_exit;
		}
	}

	if(exit_or_no_exit == 0)
	{
	};
	if(exit_or_no_exit != 0)
	{
		recursiveFitHistogramWithGaussian2(CANVAS, DATA, TITLE, X_AXIS_LABEL, N_OF_SIGMAS, FROM, TO);
	}


	TGraph * g_DATA = new TGraph();
	g_DATA->SetHistogram(h_DATA);
	g_DATA->GetXaxis()->SetRange(FROM, TO);

	float scarto_medio_assoluto = 0;
	for(auto & datum : DATA)
	{
		scarto_medio_assoluto += abs(datum - mean_of_fz_gauss_DATA);
	}
	scarto_medio_assoluto = scarto_medio_assoluto / DATA.size();


	vector<float> return_vec{constant_of_fz_gauss_DATA, mean_of_fz_gauss_DATA, sigma_of_fz_gauss_DATA, scarto_medio_assoluto};

	std::cout << "constant_of_fz_gauss_DATA: " << constant_of_fz_gauss_DATA << endl << "mean_of_fz_gauss_DATA: " << mean_of_fz_gauss_DATA << endl << "sigma_of_fz_gauss_DATA: "
		<< sigma_of_fz_gauss_DATA << endl << "scarto_medio_assoluto: " << scarto_medio_assoluto << endl;
	return return_vec;


}

using namespace std;

void polarimetro()
{

	std::cout << "£££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££££" << endl;

	gStyle->SetOptFit();

	//vettore di puntatori a TCanvas che stamperò poi alla fine. In realtà ROOT li stampa lo stesso anche se non chiamo Draw() da nessuna parte.
	vector<TCanvas *> c_ptr_vec = vector<TCanvas *>();


	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "____________________________________________________________________SENZA AMPOLLA, MASSIMA ILLUMINAZIONE__________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_empty_high = new TCanvas("c_empty_high", "c_empty_high", 0, 0, 1280, 720);
	c_ptr_vec.push_back(c_empty_high);

	float theta_empty_high[] = {86.25, 122.20, 110.15, 106.00, 112.60, 68.30, 87.60, 86.40, 90.10, 102.20, 115.70, 50.40, 49.80, 69.80, 90.00, 72.60, 63.45, 86.70, 87.90, 113.40, 71.05, 76.55, 74.55,
								 68.60, 66.40, 72.50, 70.75, 67.75, 67.50, 64.45,68.50, 68.60, 72.20, 74.30, 60.50, 75.25, 74.30, 77.40, 98.20, 77.55, 83.25, 75.50, 64.20, 94.70, 91.55, 65.80, 60.95,
								 98.05, 96.40, 41.90, 83.60, 64.55, 84.10, 59.70, 69.50, 48.60, 80.50, 49.70, 82.20, 57.3};

	vector<float> theta_empty_high_vec = vector<float>(begin(theta_empty_high), end(theta_empty_high));

	vector<float> empty_high_returned_vec = recursiveFitHistogramWithGaussian2(c_empty_high, theta_empty_high_vec, "Senza ampolla, alta illuminazione", "#theta (#circ)", 3, 10, 160);





	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________________SENZA AMPOLLA, MINIMA ILLUMINAZIONE_________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_empty_low = new TCanvas("c_empty_low", "c_empty_low", 0, 0, 1280, 720);
	c_ptr_vec.push_back(c_empty_low);

	float theta_empty_low[] = {0.15, 0, 0.05, 0.05, 0.1, 0.1, 0.05, 0.05, 0, 0, 0, 0.05, 0.05, 0, 0.05, 0.05, 0.1, 0.05, 0, 0.1, 0.1, 0.05, 0, 0.05, 0, 0.05, 0.05, 0.05, 0.05, 0.05, 0, -0.05, -0.05,
								0.1, 0.05, 0.05, 0, 0.15, 0.1, 0.1, 0.15, 0.1, 0.1, 0, 0.3, 0.1, 0, 0.05, 0.1, 0.05, -0.1, 0.05, 0.1, 0.15, 0, 0, 0, 0.15, 0, 0, 0, 0.05, 0.1, 0, 0.05, 0.1, 0, 0.05,
								0.15, 0.1, -0.05, 0.05, 0.05, 0, 0.1, 0.1, 0.05, 0.1, 0, 0, 0.05, 0.05, 0.05, 0.1, 0.05, 0, 0.05, 0.05, -0.05, 0.05, 0.05, 0, 0, 0.2, 0.1, 0.05, 0.05, 0, 0.15};

	vector<float> theta_empty_low_vec = vector<float>(begin(theta_empty_low), end(theta_empty_low));

	vector<float> empty_low_returned_vec = recursiveFitHistogramWithGaussian1(c_empty_low, theta_empty_low_vec, "Senza ampolla, minima illuminazione", "#theta (#circ)", 0.05, 3, -0.2, 0.4);




	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________PRIMA AMPOLLA LUNGA, MINIMA ILLUMINAZIONE____________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_long1_low = new TCanvas("c_long1_low", "c_long1_low", 0, 0, 1280, 720);
	c_ptr_vec.push_back(c_long1_low);

	float theta_long1_low[] = {4.55, 4.7, 4.5, 4.5, 4.6, 4.6, 4.35, 4.55, 4.45, 4.5, 4.65, 4.7, 4.4, 4.45, 4.7, 4, 4.4, 4.5, 4.3, 4.45, 4.6, 4.3, 4.3, 4.6, 4.45, 4.6, 4.65, 4.55, 4.45, 4.4, 4.5, 4.6,
								4.35, 4.55, 4.65, 4.55, 4.55, 4.55, 4.65, 4.6, 4.6, 4.35, 4.55, 4.6, 4.45, 4.5, 4.45, 4.45, 4.3, 4.5, 4.65, 4.45, 4.6, 4.55, 4.5, 4.55, 4.55, 4.5, 4.55, 4.6, 4.5, 4.4, 4.6,
								4.55, 4.35, 4.65, 4.45,	4.45, 4.55, 4.6, 4.55, 4.7, 4.6, 4.48, 4.65, 4.45, 4.6, 4.35, 4.5, 4.45, 4.5, 4.55, 4.5, 4.45, 4.55, 4.45, 4.35, 4.45, 4.6, 4.5, 4.65, 4.6, 4.55,
								4.55, 4.45, 4.5, 4.35, 4.4, 4.55};

	vector<float> theta_long1_low_vec = vector<float>(begin(theta_long1_low), end(theta_long1_low));

	vector<float> long1_low_returned_vec = recursiveFitHistogramWithGaussian1(c_long1_low, theta_long1_low_vec, "Ampolla lunga 1, minima illuminazione", "#theta (#circ)", 0.05, 3, 0, 8);




	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "______________________________________________________________SECONDA AMPOLLA LUNGA, MINIMA ILLUMINAZIONE_________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_long2_low = new TCanvas("c_long2_low", "c_long2_low", 0, 0, 1280, 720);
	c_ptr_vec.push_back(c_long2_low);

	float theta_long2_low[] = {-9.25, -9.23, -9.25, -9.25, -9.35, -9.25, -9.30, -9.15, -9.30, -9.20, -9.25, -9.20, -9.25, -9.40, -9.25, -9.15, -9.30, -9.25, -9.30, -9.15, -9.30, -9.25, -9.40, -9.30,
								-9.25, -9.20, -9.30, -9.20, -9.15, -9.30, -9.25, -9.50, -9.15, -9.10, -9.20, -9.15, -9.25, -9.25, -9.30, -9.35, -9.20, -9.30, -9.15, -9.20, -9.25, -9.25, -9.30, -9.25,
								-9.20, -9.15, -9.10, -9.05, -9.20, -9.25, -9.20, -9.15, -9.10, -9.25, -9.15,  -9.15, -9.15, -9.15, -9.00, -9.25, -9.20, -9.00, -9.25, -9.20, -9.10, -9.15, -9.10, -9.10,
								-9.25, -9.15, -9.10, -9.10, -9.10, -9.25, -9.05, -9.20, -9.05, -9.20, -9.30, -9.15, -9.00, -9.00, -9.15, -9.15, -9.10, -9.25, -9.20, -9.00, -9.20, -9.15, -9.15, -9.25,
								-9.00, -9.10, -9.05, -9.15};

	vector<float> theta_long2_low_vec = vector<float>(begin(theta_long2_low), end(theta_long2_low));

	vector<float> long2_low_returned_vec = recursiveFitHistogramWithGaussian1(c_long2_low, theta_long2_low_vec, "Ampolla lunga 2, minima illuminazione", "#theta (#circ)", 0.05, 3, -10, -8);




	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "________________________________________________________________AMPOLLA CORTA, MINIMA ILLUMINAZIONE_______________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_short_low = new TCanvas("c_short_low", "c_short_low", 0, 0, 1280, 720);
	c_ptr_vec.push_back(c_short_low);

	float theta_short_low[] = {2.00, 2.05, 2.10, 2.05, 2.10, 2.05, 2.00, 1.95, 2.05, 2.00, 2.00, 2.15, 2.00, 2.05, 2.15, 2.10, 2.10, 2.00, 2.00, 1.95, 2.10, 2.15, 2.00, 2.00, 2.10, 2.15, 2.10, 2.20,
							 2.10, 2.15, 2.00, 2.10, 2.20, 2.00, 2.10, 2.15, 1.95, 2.10, 2.05, 1.95, 2.00, 2.10, 1.95, 2.00, 2.00, 2.15, 2.00, 1.95, 2.00, 2.00, 2.00, 1.95, 2.00,2.15, 2.00, 2.00,
							 2.00, 2.05, 2.15, 2.05, 2.00, 2.05, 1.95, 2.00, 2.00, 1.95, 2.15, 2.10, 1.95, 1.95, 2.05, 2.15, 2.00, 2.10, 2.05, 2.00, 1.95, 2.00, 1.95, 2.15, 2.00, 2.05, 2.00, 2.15,
							 1.95, 2.00, 2.00, 2.00, 1.95, 2.05, 2.00, 1.95, 2.05, 2.10, 2.00, 2.15, 1.95, 2.05, 2.20, 1.95};

	vector<float> theta_short_low_vec = vector<float>(begin(theta_short_low), end(theta_short_low));

	vector<float> short_low_returned_vec = recursiveFitHistogramWithGaussian1(c_short_low, theta_short_low_vec, "Ampolla corta, minima illuminazione", "#theta (#circ)", 0.05, 3, -3, 6);




	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;
	std::cout << "_________________________________________________________________AMPOLLA CORTA, MASSIMA ILLUMINAZIONE_____________________________________________________________________________" << endl;
	std::cout << "__________________________________________________________________________________________________________________________________________________________________________________" << endl;

	TCanvas * c_short_high = new TCanvas("c_short_high", "c_short_high", 0, 0, 1280, 720);
	c_ptr_vec.push_back(c_short_high);

	float theta_short_high[] = {85.20, 85.20, 69.55, 89.80, 65.25, 63.35, 64.40, 76.40, 79.80, 79.50, 64.90, 68.30, 81.45, 88.10, 65.20, 67.50, 65.95, 68.95, 76.70, 79.80, 72.90, 67.80, 79.30, 74.20,
								 75.25, 71.35, 67.30, 79.40, 68.40, 77.40, 93.25, 69.25, 77.00, 78.90, 83.65, 69.30, 78.00, 76.60, 64.90, 64.55, 68.55, 72.30, 71.75, 86.10, 79.05, 62.55, 61.60, 94.30,
								 81.70, 81.15, 84.20, 91.40, 89.40, 71.75, 74.70, 83.90, 68.60, 52.80, 42.50, 52.90};

	vector<float> theta_short_high_vec = vector<float>(begin(theta_short_high), end(theta_short_high));

	vector<float> short_high_returned_vec = recursiveFitHistogramWithGaussian2(c_short_high, theta_short_high_vec, "Ampolla corta, massima illuminazione", "#theta (#circ)", 3, 10, 140);


	//disegnamo i Canvas	
	for(vector<TCanvas *>::const_iterator it = c_ptr_vec.begin(); it != c_ptr_vec.end(); it++)
	{
		(*it)->cd();
		(*it)->Draw();
	}
}

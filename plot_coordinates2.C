#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>    // std::max

void plot_coordinates2(){

  std::string base="/scratch/01230/fnova/output/";
std::string direc="2014_06_17b_gps__CHIPS__refl_0__at_42__depth_60__yes_mirror";

  //base="/scratch/01230/fnova/programs/";
  //direc="CHIPS";

  std::string volume;
  double time, deltatime, energy, length, x0_ID, y0_ID, z0_ID, x0_VETO, y0_VETO, z0_VETO, x0, y0, z0;
  std::vector<float> * x0s_ID = 0;
  std::vector<float> * y0s_ID = 0;
  std::vector<float> * z0s_ID = 0;
  std::vector<float> * x0s_VETO = 0;
  std::vector<float> * y0s_VETO = 0;
  std::vector<float> * z0s_VETO = 0;
  std::vector<float> * times = 0;
  std::vector<float> * energies = 0;
  std::vector<float> * lengths = 0;
  std::vector<float> * deltatimes = 0;


  TFile input_file((base+direc+"/coordinates.root").c_str());
  TFile output_file((base+direc+"/coordinates2.root").c_str(),"RECREATE");

  string veto_volume="veto_pmt";
  string ID_volume="pmt";

  TTree *input_tree_ID = (TTree*)input_file.Get("analysis_ID");

  TBranch * b_x0s_ID;// = (TBranch*)input_tree_ID->GetBranch("x0s_ID");
  input_tree_ID->SetBranchAddress("x0s_ID", &x0s_ID, &b_x0s_ID);
  TBranch * b_y0s_ID;// = (TBranch*)input_tree_ID->GetBranch("y0s_ID");
  input_tree_ID->SetBranchAddress("y0s_ID", &y0s_ID, &b_y0s_ID);
  TBranch * b_z0s_ID;// = (TBranch*)input_tree_ID->GetBranch("z0s_ID");
  input_tree_ID->SetBranchAddress("z0s_ID", &z0s_ID, &b_z0s_ID);
  TBranch * b_times;// = (TBranch*)input_tree_ID->GetBranch("times");
  input_tree_ID->SetBranchAddress("times", &times, &b_times);
  TBranch * b_deltatimes;// = (TBranch*)input_tree_ID->GetBranch("deltatimes");
  input_tree_ID->SetBranchAddress("deltatimes", &deltatimes, &b_deltatimes);


  int nbins_x = 29;
  int nbins_y = 29;
  int nbins_z = 30;
  int nbins_phi  = 92;
  int nbins_time = 100;
  double phi, radius;
  double z_limit=1.4;
  double r_limit=1.6;
  double tmin=0.;
  double tmax=100.;
  double labelsize=0.045;
  TH1F h_x0_ID("h_x0_ID","x0_ID;x0_ID (m);number of events",nbins_x,1,-1);
  h_x0_ID.GetXaxis()->SetLabelSize(labelsize);
  h_x0_ID.GetYaxis()->SetLabelSize(labelsize);
  TH1F h_y0_ID("h_y0_ID","y0_ID;y0_ID (m);number of events",nbins_y,1,-1);
  h_y0_ID.GetXaxis()->SetLabelSize(labelsize);
  h_y0_ID.GetYaxis()->SetLabelSize(labelsize);
  TH1F h_z0_ID("h_z0_ID","z0_ID;z0_ID (m);number of events",nbins_z,1,-1);
  h_z0_ID.GetXaxis()->SetLabelSize(labelsize);
  h_z0_ID.GetYaxis()->SetLabelSize(labelsize);
  TH2F h_y0_ID_vs_x0_ID("h_y0_ID_vs_x0_ID","y0_ID_vs_x0_ID;x0_ID (m);y0_ID (m)",nbins_x,-(r_limit+0.1),(r_limit+0.1),nbins_y,-(r_limit+0.1),(r_limit+0.1));
  h_y0_ID_vs_x0_ID.GetXaxis()->SetLabelSize(labelsize);
  h_y0_ID_vs_x0_ID.GetYaxis()->SetLabelSize(labelsize);
  TH2F h_y0_ID_vs_x0_ID_top("h_y0_ID_vs_x0_ID_top","y0_ID_vs_x0_ID_top;x0_ID (m);y0_ID (m)",nbins_x,-(r_limit+0.1),(r_limit+0.1),nbins_y,-(r_limit+0.1),(r_limit+0.1));
  h_y0_ID_vs_x0_ID_top.GetXaxis()->SetLabelSize(labelsize);
  h_y0_ID_vs_x0_ID_top.GetYaxis()->SetLabelSize(labelsize);
  TH2F h_y0_ID_vs_x0_ID_bottom("h_y0_ID_vs_x0_ID_bottom","y0_ID_vs_x0_ID_bottom;x0_ID (m);y0_ID (m)",nbins_x,-(r_limit+0.1),(r_limit+0.1),nbins_y,-(r_limit+0.1),(r_limit+0.1));
  h_y0_ID_vs_x0_ID_bottom.GetXaxis()->SetLabelSize(labelsize);
  h_y0_ID_vs_x0_ID_bottom.GetYaxis()->SetLabelSize(labelsize);
  TH2F h_z0_ID_vs_y0_ID("h_z0_ID_vs_y0_ID","z0_ID_vs_y0_ID;y0_ID (m);z0_ID (m)",nbins_y,-(r_limit+0.1),(r_limit+0.1),nbins_z,-(r_limit+0.1),(r_limit+0.1));
  h_z0_ID_vs_y0_ID.GetXaxis()->SetLabelSize(labelsize);
  h_z0_ID_vs_y0_ID.GetYaxis()->SetLabelSize(labelsize);
  TH2F h_z0_ID_vs_x0_ID("h_z0_ID_vs_x0_ID","z0_ID_vs_x0_ID;x0_ID (m);z0_ID (m)",nbins_x,-(r_limit+0.1),(r_limit+0.1),nbins_z,-(r_limit+0.1),(r_limit+0.1));
  h_z0_ID_vs_x0_ID.GetXaxis()->SetLabelSize(labelsize);
  h_z0_ID_vs_x0_ID.GetYaxis()->SetLabelSize(labelsize);
  //  TH3F h_z0_ID_vs_y0_ID_vs_x0_ID("h_z0_ID_vs_y0_ID_vs_x0_ID","z0_ID_vs_y0_ID_vs_x0_ID;x0_ID (m); y0_ID (m);z0_ID (m)",nbins_x,-(r_limit+0.1),(r_limit+0.1),nbins_y,-(r_limit+0.1),(r_limit+0.1),nbins_z,-(r_limit+0.1),(r_limit+0.1));
  TH2F h_z0_ID_vs_phi_ID("h_z0_ID_vs_phi_ID","z0_ID_vs_phi_ID;phi_ID (degrees);z0_ID (m)",nbins_phi,-290.,110.,nbins_z,-(z_limit+0.4),(z_limit+0.4));
  h_z0_ID_vs_phi_ID.GetXaxis()->SetLabelSize(labelsize);
  h_z0_ID_vs_phi_ID.GetYaxis()->SetLabelSize(labelsize);
  TH2F h_z0_ID_vs_phi_ID_upstream("h_z0_ID_vs_phi_ID_upstream","z0_ID_vs_phi_ID_upstream;phi_ID (degrees);z0_ID (m)",nbins_phi,-290.,110.,nbins_z,-(z_limit+0.4),(z_limit+0.4));
  h_z0_ID_vs_phi_ID_upstream.GetXaxis()->SetLabelSize(labelsize);
  h_z0_ID_vs_phi_ID_upstream.GetYaxis()->SetLabelSize(labelsize);
  TH2F h_z0_ID_vs_phi_ID_downstream("h_z0_ID_vs_phi_ID_downstream","z0_ID_vs_phi_ID_downstream;phi_ID (degrees);z0_ID (m)",nbins_phi,-290.,110.,nbins_z,-(z_limit+0.4),(z_limit+0.4));
  h_z0_ID_vs_phi_ID_downstream.GetXaxis()->SetLabelSize(labelsize);
  h_z0_ID_vs_phi_ID_downstream.GetYaxis()->SetLabelSize(labelsize);
  TH1F h_time("h_time","time; time(ns);number of events",nbins_time,tmin,tmax);
  h_time.GetXaxis()->SetLabelSize(labelsize);
  h_time.GetYaxis()->SetLabelSize(labelsize);
  TH1F h_deltatime("h_deltatime","deltatime; deltatime(ns);number of events",nbins_time,tmin,tmax);
  h_deltatime.GetXaxis()->SetLabelSize(labelsize);
  h_deltatime.GetYaxis()->SetLabelSize(labelsize);


  /// loop on tree entries and fill histograms
  Long64_t tentry;
  std::clog << " reading " <<  input_tree_ID->GetEntries() << " entries " << std::endl;
  for (Int_t ientry = 0; ientry < input_tree_ID->GetEntries(); ientry++) {

    if( ientry%1000000 == 0 ) std::clog << " reading entry " << ientry << std::endl;

    // load branches values
    tentry = input_tree_ID->LoadTree(ientry);
    b_x0s_ID->GetEntry(tentry);
    b_y0s_ID->GetEntry(tentry);
    b_z0s_ID->GetEntry(tentry);
    b_times->GetEntry(tentry);
    b_deltatimes->GetEntry(tentry);

    for(size_t i=0; i<x0s_ID->size(); i++){
      x0_ID = x0s_ID->at(i);
      y0_ID = y0s_ID->at(i);
      z0_ID = z0s_ID->at(i);
      time = times->at(i);
      deltatime = deltatimes->at(i);

      h_x0_ID.Fill(x0_ID);
      h_y0_ID.Fill(y0_ID);
      h_z0_ID.Fill(z0_ID);
      h_y0_ID_vs_x0_ID.Fill(x0_ID, y0_ID);
      h_z0_ID_vs_y0_ID.Fill(y0_ID, z0_ID);
      h_z0_ID_vs_x0_ID.Fill(x0_ID, z0_ID);
      h_time.Fill(time);
      h_deltatime.Fill(deltatime);
      //h_z0_ID_vs_y0_ID_vs_x0_ID.Fill(x0_ID, y0_ID, z0_ID);
      radius=sqrt(pow(x0_ID,2) + pow(y0_ID,2));
      phi = atan2(y0_ID, x0_ID)*180./acos(-1.);
      if( phi > 90. ) phi -= 360.;

      if( radius > r_limit ){  // side
	h_z0_ID_vs_phi_ID.Fill(phi, z0_ID);
	if( phi > - 90. )
	  h_z0_ID_vs_phi_ID_upstream.Fill(phi, z0_ID);
	else
	  h_z0_ID_vs_phi_ID_downstream.Fill(phi, z0_ID);
      }else{ // top and bottom
	if( z0_ID > z_limit ){
	  h_y0_ID_vs_x0_ID_top.Fill(x0_ID, y0_ID);
	}else{
	  h_y0_ID_vs_x0_ID_bottom.Fill(x0_ID, y0_ID);
	}
      }
    }
  }


  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TCanvas c("c","c",600,600);
  double BottomMargin = .17;   // fraction of pad height
  double LeftMargin = .1;
  double RightMargin = .18;
  c.SetBottomMargin(BottomMargin);
  c.SetLeftMargin(LeftMargin);
  c.SetRightMargin(RightMargin);

  string figname="figure/"+direc+"_"+h_y0_ID_vs_x0_ID.GetTitle()+".png";
  h_y0_ID_vs_x0_ID.Draw("colz");
  c.Print(figname.c_str());

  figname="figure/"+direc+"_"+h_z0_ID_vs_phi_ID.GetTitle()+".png";
  h_z0_ID_vs_phi_ID.Draw("colz");
  c.Print(figname.c_str());

  figname="figure/"+direc+"_"+h_z0_ID_vs_phi_ID_upstream.GetTitle()+".png";
  h_z0_ID_vs_phi_ID_upstream.Draw("colz");
  c.Print(figname.c_str());

  figname="figure/"+direc+"_"+h_z0_ID_vs_phi_ID_downstream.GetTitle()+".png";
  h_z0_ID_vs_phi_ID_downstream.Draw("colz");
  c.Print(figname.c_str());

  figname="figure/"+direc+"_"+h_y0_ID_vs_x0_ID_top.GetTitle()+".png";
  h_y0_ID_vs_x0_ID_top.Draw("colz");
  c.Print(figname.c_str());

  figname="figure/"+direc+"_"+h_y0_ID_vs_x0_ID_bottom.GetTitle()+".png";
  h_y0_ID_vs_x0_ID_bottom.Draw("colz");
  c.Print(figname.c_str());

  string figname="figure/"+direc+"_"+h_time.GetTitle()+".png";
  h_time.Draw();
  c.Print(figname.c_str());


  TCanvas c_display("c_display","c_display",600,600);
  c_display.SetBottomMargin(BottomMargin);
  c_display.SetLeftMargin(LeftMargin);
  c_display.SetRightMargin(RightMargin);

  TPad * topPad;
  TPad * sidePad;
  TPad * bottomPad;

  double zmax_top = h_y0_ID_vs_x0_ID_top.GetMaximum();
  double zmax_side = h_z0_ID_vs_phi_ID.GetMaximum();
  double zmax_bottom = h_y0_ID_vs_x0_ID_bottom.GetMaximum();
  double the_max = std::max(zmax_top, zmax_side);
  the_max = std::max(the_max, zmax_bottom);
  h_y0_ID_vs_x0_ID_top.GetZaxis()->SetRangeUser(0, the_max);
  h_z0_ID_vs_phi_ID.GetZaxis()->SetRangeUser(0, the_max);
  h_y0_ID_vs_x0_ID_bottom.GetZaxis()->SetRangeUser(0, the_max);
  double xmin=0.33;
  double xmax=0.67;
  double Xmin=0.005;
  double Xmax=0.995;
  double ymaxtop=0.995;
  double ymintop=0.66;
  double ymaxside=0.66;
  double yminside=0.33;
  double ymaxbottom=0.33;
  double yminbottom=0.005;
  c_display.Divide(1,3);
  topPad =  (TPad*)c_display.GetListOfPrimitives()->FindObject("c_display_1");
  topPad->SetPad(xmin, ymintop, xmax, ymaxtop);
  sidePad =  (TPad*)c_display.GetListOfPrimitives()->FindObject("c_display_2");
  sidePad->SetPad(Xmin, yminside, Xmax, ymaxside);
  bottomPad =  (TPad*)c_display.GetListOfPrimitives()->FindObject("c_display_3");
  bottomPad->SetPad(xmin, yminbottom, xmax, ymaxbottom);

  topPad->cd();
  h_y0_ID_vs_x0_ID_top.Draw("col");
  sidePad->cd();
  h_z0_ID_vs_phi_ID.Draw("colz");
  bottomPad->cd();
  h_y0_ID_vs_x0_ID_bottom.Draw("col");
  TPaletteAxis *palette = (TPaletteAxis*)h_z0_ID_vs_phi_ID.GetListOfFunctions()->FindObject("palette");
  palette->SetX1NDC(0.9);
  palette->SetX2NDC(0.95);
  palette->SetY1NDC(0.2);
  palette->SetY2NDC(0.8);
  c_display.Modified();
  c_display.Update();

  figname="figure/"+direc+"_display.eps";
  c_display.Print(figname.c_str());


  h_x0_ID.Write();
  h_y0_ID.Write();
  h_z0_ID.Write();
  h_y0_ID_vs_x0_ID.Write();
  h_y0_ID_vs_x0_ID_top.Write();
  h_y0_ID_vs_x0_ID_bottom.Write();
  h_z0_ID_vs_phi_ID.Write();
  h_z0_ID_vs_phi_ID_upstream.Write();
  h_z0_ID_vs_phi_ID_downstream.Write();
  h_z0_ID_vs_y0_ID.Write();
  h_z0_ID_vs_x0_ID.Write();
  h_time.Write();
  h_deltatime.Write();
  //  h_z0_ID_vs_y0_ID_vs_x0_ID.Write();


  exit(0);


}


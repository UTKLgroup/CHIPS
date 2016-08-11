#include "Riostream.h"
#include "TString.h"
#include "iostream"
#include "sstream"
#include <stdio.h>
#include <string.h>

void count_file(string input_file_name) {

  using namespace std;

  size_t last_slash=input_file_name.rfind("/");
  if( last_slash == string::npos){
    cout << " zombie no directory " << endl;
    exit(0);
  }

  string directory=input_file_name.substr(0,last_slash);

  TFile *input = new TFile(input_file_name.c_str(),"READ");
  if( input->IsZombie() ){
    cout << " zombie " << endl;
    exit(0);
  }

  int nentries = RunTree->GetEntries();
  cout << " events " << nentries << endl;
  cout << " directory " << directory << endl;
  exit(0);


}


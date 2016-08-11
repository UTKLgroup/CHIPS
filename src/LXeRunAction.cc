//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file optical/LXe/src/LXeRunAction.cc
/// \brief Implementation of the LXeRunAction class
//
//
#include "LXeRunAction.hh"
#include "LXeRecorderBase.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeRunAction::LXeRunAction(LXeRecorderBase* r) : fRecorder(r) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeRunAction::~LXeRunAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeRunAction::BeginOfRunAction(const G4Run* aRun){
  if(fRecorder)fRecorder->RecordBeginOfRun(aRun);

  RunTree   = new TTree("RunTree","Event Statistics");
  RunBranch = RunTree->Branch("Stat",
			      &Stat.PhotonCount_Scint, 
			      "PhotonCount_Scint/I:PhotonCount_Ceren:HitCount:PMTsAboveThreshold:AbsorptionCount:BoundaryAbsorptionCount:Unacounted:VetoPhotonCount_Ceren:VetoHitCount:VetoPMTsAboveThreshold:VetoAbsorptionCount:VetoBoundaryAbsorptionCount:VetoUnacounted:primaryPDG:inX/D:inY:inZ:outX:outY:outZ:VetoinX:VetoinY:VetoinZ:VetooutX:VetooutY:VetooutZ:PX:PY:PZ:primaryX:primaryY:primaryZ");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeRunAction::EndOfRunAction(const G4Run* aRun){
  if(fRecorder)fRecorder->RecordEndOfRun(aRun);

  TFile *out=new TFile("chips.root","update");
  RunTree->Write();
  out->Close();
}

void LXeRunAction::UpdateStatistics(EvtStat stat)
{
  Stat = stat;
  RunTree->Fill();

  /*
  G4cout << Stat.PhotonCount_Scint << G4endl;

  G4cout << Stat.PhotonCount_Ceren << G4endl;
  G4cout << Stat.HitCount << G4endl;
  G4cout << Stat.PMTsAboveThreshold << G4endl;
  G4cout << Stat.AbsorptionCount << G4endl;
  G4cout << Stat.BoundaryAbsorptionCount << G4endl;
  G4cout << Stat.Unacounted << G4endl;

  G4cout << Stat.VetoPhotonCount_Ceren << G4endl;
  G4cout << Stat.VetoHitCount << G4endl;
  G4cout << Stat.VetoPMTsAboveThreshold << G4endl;
  G4cout << Stat.VetoAbsorptionCount << G4endl;
  G4cout << Stat.VetoBoundaryAbsorptionCount << G4endl;
  G4cout << Stat.VetoUnacounted << G4endl; 
  */
}  

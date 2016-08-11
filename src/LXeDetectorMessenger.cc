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
/// \file optical/LXe/src/LXeDetectorMessenger.cc
/// \brief Implementation of the LXeDetectorMessenger class
//
//
#include "LXeDetectorMessenger.hh"
#include "LXeDetectorConstruction.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4Scintillation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeDetectorMessenger::LXeDetectorMessenger(LXeDetectorConstruction* detector)
 : fLXeDetector(detector)
{
  //Setup a command directory for detector controls with guidance
  fDetectorDir = new G4UIdirectory("/LXe/detector/");
  fDetectorDir->SetGuidance("Detector geometry control");

  fVolumesDir = new G4UIdirectory("/LXe/detector/volumes/");
  fVolumesDir->SetGuidance("Enable/disable volumes");
 
  //Various commands for modifying detector geometry
  fDimensionsCmd =
    new G4UIcmdWith3VectorAndUnit("/LXe/detector/dimensions",this);
  fDimensionsCmd->SetGuidance("Set the dimensions of the detector volume.");
  fDimensionsCmd->SetParameterName("scint_x","scint_y","scint_z",false);
  fDimensionsCmd->SetDefaultUnit("cm");

  fHousingThicknessCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/housingThickness",this);
  fHousingThicknessCmd->SetGuidance("Set the thickness of the housing.");
  fHousingThicknessCmd->SetParameterName("d_mtl",false);
  fHousingThicknessCmd->SetDefaultUnit("cm");

  fPmtRadiusCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/pmtRadius",this);
  fPmtRadiusCmd->SetGuidance("Set the radius of the PMTs.");
  fPmtRadiusCmd->SetParameterName("radius",false);
  fPmtRadiusCmd->SetDefaultUnit("cm");

  fNxCmd = new G4UIcmdWithAnInteger("/LXe/detector/nx",this);
  fNxCmd->SetGuidance("Set the number of PMTs along the x-dimension.");
  fNxCmd->SetParameterName("nx",false);

  fNyCmd = new G4UIcmdWithAnInteger("/LXe/detector/ny",this);
  fNyCmd->SetGuidance("Set the number of PMTs along the y-dimension.");
  fNyCmd->SetParameterName("ny",false);

  fNzCmd = new G4UIcmdWithAnInteger("/LXe/detector/nz",this);
  fNzCmd->SetGuidance("Set the number of PMTs along the z-dimension.");
  fNzCmd->SetParameterName("nz",false);

  fSphereCmd = new G4UIcmdWithABool("/LXe/detector/volumes/sphere",this);
  fSphereCmd->SetGuidance("Enable/Disable the sphere.");

  fSaveHitCmd = new G4UIcmdWithABool("/LXe/detector/SaveHitOn",this);
  fSaveHitCmd->SetGuidance("Save/do not save hit information.");

  fSidePMTsRotationAngleXCmd = new G4UIcmdWithADouble("/LXe/detector/SidePMTsRotationAngleX",this);
  fSidePMTsRotationAngleXCmd->SetGuidance("side PMTs angle of rotation (X).");

  fSidePMTsRotationAngleYCmd = new G4UIcmdWithADouble("/LXe/detector/SidePMTsRotationAngleY",this);
  fSidePMTsRotationAngleYCmd->SetGuidance("side PMTs angle of rotation (Y).");

  fSidePMTsAimedAtAxisCmd = new G4UIcmdWithABool("/LXe/detector/SidePMTsAimedAtAxis",this);
  fSidePMTsAimedAtAxisCmd->SetGuidance("side PMTs aimed at axis.");

  fTopPMTsRotationAngleYCmd = new G4UIcmdWithADouble("/LXe/detector/TopPMTsRotationAngleY",this);
  fTopPMTsRotationAngleYCmd->SetGuidance("top PMTs angle of rotation (Y).");

  fBottomPMTsRotationAngleYCmd = new G4UIcmdWithADouble("/LXe/detector/BottomPMTsRotationAngleY",this);
  fBottomPMTsRotationAngleYCmd->SetGuidance("bottom PMTs angle of rotation (Y).");

  fScintReflectivityCmd = new G4UIcmdWithADouble("/LXe/detector/scintreflectivity",this);
  fScintReflectivityCmd->SetGuidance("Set the reflectivity of the inner detector (ID).");

  fHousingLobeReflectivityCmd = new G4UIcmdWithADouble("/LXe/detector/HousingLobeReflectivity",this);
  fHousingLobeReflectivityCmd->SetGuidance("Set the HousingLobeReflectivity");

  fPMTBaseReflectivityCmd = new G4UIcmdWithADouble("/LXe/detector/PMTBaseReflectivity",this);
  fPMTBaseReflectivityCmd->SetGuidance("Set the PMTBaseReflectivity");

  fAdHocPMTsCmd = new G4UIcmdWithABool("/LXe/detector/AdHocPMTs",this);
  fAdHocPMTsCmd->SetGuidance("Set ad hot PMTs");

  fPutMirrorCmd = new G4UIcmdWithABool("/LXe/detector/PutMirror",this);
  fPutMirrorCmd->SetGuidance("Set put mirror");
  fMirrorX0Cmd = new G4UIcmdWithADouble("/LXe/detector/MirrorX0",this);
  fMirrorX0Cmd->SetGuidance("Set mirror X0");
  fMirrorY0Cmd = new G4UIcmdWithADouble("/LXe/detector/MirrorY0",this);
  fMirrorY0Cmd->SetGuidance("Set mirror Y0");
  fMirrorZ0Cmd = new G4UIcmdWithADouble("/LXe/detector/MirrorZ0",this);
  fMirrorZ0Cmd->SetGuidance("Set mirror Z0");
  fMirrorLACmd = new G4UIcmdWithADouble("/LXe/detector/MirrorLA",this);
  fMirrorLACmd->SetGuidance("Set mirror LA");
  fMirrorLBCmd = new G4UIcmdWithADouble("/LXe/detector/MirrorLB",this);
  fMirrorLBCmd->SetGuidance("Set mirror LB");
  fMirrorLCCmd = new G4UIcmdWithADouble("/LXe/detector/MirrorLC",this);
  fMirrorLCCmd->SetGuidance("Set mirror LC");
  fMirrorAngleCmd = new G4UIcmdWithADouble("/LXe/detector/MirrorAngle",this);
  fMirrorAngleCmd->SetGuidance("Set mirror Angle");

  fPutMirror2Cmd = new G4UIcmdWithABool("/LXe/detector/PutMirror2",this);
  fPutMirror2Cmd->SetGuidance("Set put mirror2");
  fMirror2X0Cmd = new G4UIcmdWithADouble("/LXe/detector/Mirror2X0",this);
  fMirror2X0Cmd->SetGuidance("Set mirror2 X0");
  fMirror2Y0Cmd = new G4UIcmdWithADouble("/LXe/detector/Mirror2Y0",this);
  fMirror2Y0Cmd->SetGuidance("Set mirror2 Y0");
  fMirror2Z0Cmd = new G4UIcmdWithADouble("/LXe/detector/Mirror2Z0",this);
  fMirror2Z0Cmd->SetGuidance("Set mirror2 Z0");
  fMirror2LACmd = new G4UIcmdWithADouble("/LXe/detector/Mirror2LA",this);
  fMirror2LACmd->SetGuidance("Set mirror2 LA");
  fMirror2LBCmd = new G4UIcmdWithADouble("/LXe/detector/Mirror2LB",this);
  fMirror2LBCmd->SetGuidance("Set mirror2 LB");
  fMirror2LCCmd = new G4UIcmdWithADouble("/LXe/detector/Mirror2LC",this);
  fMirror2LCCmd->SetGuidance("Set mirror2 LC");
  fMirror2AngleCmd = new G4UIcmdWithADouble("/LXe/detector/Mirror2Angle",this);
  fMirror2AngleCmd->SetGuidance("Set mirror2 Angle");

  fAdHocPMT1xCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT1x",this);
  fAdHocPMT1xCmd->SetGuidance("ad hoc PMTs 1 x.");
  fAdHocPMT1yCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT1y",this);
  fAdHocPMT1yCmd->SetGuidance("ad hoc PMTs 1 y.");
  fAdHocPMT1zCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT1z",this);
  fAdHocPMT1zCmd->SetGuidance("ad hoc PMTs 1 z.");
  fAdHocPMT1angleXCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT1angleX",this);
  fAdHocPMT1angleXCmd->SetGuidance("ad hoc PMTs 1 angleX.");
  fAdHocPMT1angleYCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT1angleY",this);
  fAdHocPMT1angleYCmd->SetGuidance("ad hoc PMTs 1 angleY.");

  fAdHocPMT2xCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT2x",this);
  fAdHocPMT2xCmd->SetGuidance("ad hoc PMTs 2 x.");
  fAdHocPMT2yCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT2y",this);
  fAdHocPMT2yCmd->SetGuidance("ad hoc PMTs 2 y.");
  fAdHocPMT2zCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT2z",this);
  fAdHocPMT2zCmd->SetGuidance("ad hoc PMTs 2 z.");
  fAdHocPMT2angleXCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT2angleX",this);
  fAdHocPMT2angleXCmd->SetGuidance("ad hoc PMTs 2 angleX.");
  fAdHocPMT2angleYCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT2angleY",this);
  fAdHocPMT2angleYCmd->SetGuidance("ad hoc PMTs 2 angleY.");

   fAdHocPMT3xCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT3x",this);
  fAdHocPMT3xCmd->SetGuidance("ad hoc PMTs 3 x.");
  fAdHocPMT3yCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT3y",this);
  fAdHocPMT3yCmd->SetGuidance("ad hoc PMTs 3 y.");
  fAdHocPMT3zCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT3z",this);
  fAdHocPMT3zCmd->SetGuidance("ad hoc PMTs 3 z.");
  fAdHocPMT3angleXCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT3angleX",this);
  fAdHocPMT3angleXCmd->SetGuidance("ad hoc PMTs 3 angleX.");
  fAdHocPMT3angleYCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT3angleY",this);
  fAdHocPMT3angleYCmd->SetGuidance("ad hoc PMTs 3 angleY.");

   fAdHocPMT4xCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT4x",this);
  fAdHocPMT4xCmd->SetGuidance("ad hoc PMTs 4 x.");
  fAdHocPMT4yCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT4y",this);
  fAdHocPMT4yCmd->SetGuidance("ad hoc PMTs 4 y.");
  fAdHocPMT4zCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT4z",this);
  fAdHocPMT4zCmd->SetGuidance("ad hoc PMTs 4 z.");
  fAdHocPMT4angleXCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT4angleX",this);
  fAdHocPMT4angleXCmd->SetGuidance("ad hoc PMTs 4 angleX.");
  fAdHocPMT4angleYCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT4angleY",this);
  fAdHocPMT4angleYCmd->SetGuidance("ad hoc PMTs 4 angleY.");

   fAdHocPMT5xCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT5x",this);
  fAdHocPMT5xCmd->SetGuidance("ad hoc PMTs 5 x.");
  fAdHocPMT5yCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT5y",this);
  fAdHocPMT5yCmd->SetGuidance("ad hoc PMTs 5 y.");
  fAdHocPMT5zCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT5z",this);
  fAdHocPMT5zCmd->SetGuidance("ad hoc PMTs 5 z.");
  fAdHocPMT5angleXCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT5angleX",this);
  fAdHocPMT5angleXCmd->SetGuidance("ad hoc PMTs 5 angleX.");
  fAdHocPMT5angleYCmd = new G4UIcmdWithADouble("/LXe/detector/AdHocPMT5angleY",this);
  fAdHocPMT5angleYCmd->SetGuidance("ad hoc PMTs 5 angleY.");

   fPMTsOnTopCmd = new G4UIcmdWithABool("/LXe/detector/PMTsOnTop",this);
  fPMTsOnTopCmd->SetGuidance("Set the PMTsOnTop");

  fPMTsOnBottomCmd = new G4UIcmdWithABool("/LXe/detector/PMTsOnBottom",this);
  fPMTsOnBottomCmd->SetGuidance("Set the PMTsOnBottom");

  fPMTsOnPositiveXCmd = new G4UIcmdWithABool("/LXe/detector/PMTsOnPositiveX",this);
  fPMTsOnPositiveXCmd->SetGuidance("Set the PMTsOnPositiveX");

  fPMTsOnNegativeXCmd = new G4UIcmdWithABool("/LXe/detector/PMTsOnNegativeX",this);
  fPMTsOnNegativeXCmd->SetGuidance("Set the PMTsOnNegativeX");

  fVetoReflectivityCmd = new G4UIcmdWithADouble("/LXe/detector/vetoreflectivity",this);
  fVetoReflectivityCmd->SetGuidance("Set the reflectivity of the veto walls.");

  fpmtQECmd = new G4UIcmdWithADouble("/LXe/detector/pmtQE",this);
  fpmtQECmd->SetGuidance("Set the pmt QE.");

  fWlsCmd = new G4UIcmdWithABool("/LXe/detector/volumes/wls",this);
  fWlsCmd->SetGuidance("Enable/Disable the WLS slab");

  fLxeCmd = new G4UIcmdWithABool("/LXe/detector/volumes/lxe",this);
  fLxeCmd->SetGuidance("Enable/Disable the main detector volume.");

  fNFibersCmd = new G4UIcmdWithAnInteger("/LXe/detector/nfibers",this);
  fNFibersCmd->SetGuidance("Set the number of WLS fibers in the WLS slab.");

  fUpdateCmd = new G4UIcommand("/LXe/detector/update",this);
  fUpdateCmd->SetGuidance("Update the detector geometry with changed values.");
  fUpdateCmd->SetGuidance
    ("Must be run before beamOn if detector has been changed.");

  fDefaultsCmd = new G4UIcommand("/LXe/detector/defaults",this);
  fDefaultsCmd->SetGuidance("Set all detector geometry values to defaults.");
  fDefaultsCmd->SetGuidance("(Update still required)");

  fMainScintYield=new G4UIcmdWithADouble("/LXe/detector/MainScintYield",this);
  fMainScintYield->SetGuidance("Set scinitillation yield of main volume.");
  fMainScintYield->SetGuidance("Specified in photons/MeV");

  fWLSScintYield = new G4UIcmdWithADouble("/LXe/detector/WLSScintYield",this);
  fWLSScintYield->SetGuidance("Set scintillation yield of WLS Slab");
  fWLSScintYield->SetGuidance("Specified in photons/MeV");

  fPmtDRCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/PmtDR",this);
  fPmtDRCmd->SetGuidance("Set the inner detector PMT horizontal spaceing.");
  fPmtDRCmd->SetParameterName("pmtdr",false);
  fPmtDRCmd->SetDefaultUnit("cm");

  fPmtDZCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/PmtDZ",this);
  fPmtDZCmd->SetGuidance("Set the inner detector PMT vertical spaceing.");
  fPmtDZCmd->SetParameterName("pmtdz",false);
  fPmtDZCmd->SetDefaultUnit("cm");

  fVetoPmtDRCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/VetoPmtDR",this);
  fVetoPmtDRCmd->SetGuidance("Set the veto PMT horizontal spaceing.");
  fVetoPmtDRCmd->SetParameterName("vetopmtdr",false);
  fVetoPmtDRCmd->SetDefaultUnit("cm");

  fVetoPmtDZCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/VetoPmtDZ",this);
  fVetoPmtDZCmd->SetGuidance("Set the veto PMT vertical spaceing.");
  fVetoPmtDZCmd->SetParameterName("vetopmtdz",false);
  fVetoPmtDZCmd->SetDefaultUnit("cm");

  fScintRCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/ScintR",this);
  fScintRCmd->SetGuidance("Set the inner detector radius.");
  fScintRCmd->SetParameterName("scintr",false);
  fScintRCmd->SetDefaultUnit("cm");

  fDepthCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/Depth",this);
  fDepthCmd->SetGuidance("Set the detector depth.");
  fDepthCmd->SetParameterName("Depth",false);
  fDepthCmd->SetDefaultUnit("cm");

  fScintHCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/ScintH",this);
  fScintHCmd->SetGuidance("Set the inner detector half height.");
  fScintHCmd->SetParameterName("scinth",false);
  fScintHCmd->SetDefaultUnit("cm");

  fVetoDRCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/VetoDR",this);
  fVetoDRCmd->SetGuidance("Set the vertical veto wall thickness.");
  fVetoDRCmd->SetParameterName("vetodr",false);
  fVetoDRCmd->SetDefaultUnit("cm");

  fVetoDHCmd = new G4UIcmdWithADoubleAndUnit
    ("/LXe/detector/VetoDH",this);
  fVetoDHCmd->SetGuidance("Set the top veto thickness.");
  fVetoDHCmd->SetParameterName("vetodh",false);
  fVetoDHCmd->SetDefaultUnit("cm");

  fPrintSettings = new G4UIcmdWithoutParameter
    ("/LXe/detector/PrintSettings",this);
  fVetoDHCmd->SetGuidance("Print the full detector setting.");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeDetectorMessenger::~LXeDetectorMessenger()
{
  delete fDimensionsCmd;
  delete fHousingThicknessCmd;
  delete fPmtRadiusCmd;
  delete fNxCmd;
  delete fNyCmd;
  delete fNzCmd;
  delete fUpdateCmd;
  delete fDetectorDir;
  delete fVolumesDir;
  delete fDefaultsCmd;
  delete fSphereCmd;
  delete fSaveHitCmd;
  delete fSidePMTsRotationAngleXCmd;
  delete fSidePMTsRotationAngleYCmd;
  delete fSidePMTsAimedAtAxisCmd;
  delete fTopPMTsRotationAngleYCmd;
  delete fBottomPMTsRotationAngleYCmd;
  delete fWlsCmd;
  delete fLxeCmd;
  delete fNFibersCmd;
  delete fScintReflectivityCmd;
  delete fHousingLobeReflectivityCmd;
  delete fPMTBaseReflectivityCmd;
  delete fPMTsOnTopCmd;
  delete fPMTsOnBottomCmd;
  delete fPMTsOnPositiveXCmd;
  delete fPMTsOnNegativeXCmd;
  delete fAdHocPMTsCmd;
  delete fPutMirrorCmd;
  delete fMirrorX0Cmd;
  delete fMirrorY0Cmd;
  delete fMirrorZ0Cmd;
  delete fMirrorLACmd;
  delete fMirrorLBCmd;
  delete fMirrorLCCmd;
  delete fMirrorAngleCmd;
  delete fPutMirror2Cmd;
  delete fMirror2X0Cmd;
  delete fMirror2Y0Cmd;
  delete fMirror2Z0Cmd;
  delete fMirror2LACmd;
  delete fMirror2LBCmd;
  delete fMirror2LCCmd;
  delete fMirror2AngleCmd;

  delete fAdHocPMT1xCmd;
  delete fAdHocPMT1yCmd;
  delete fAdHocPMT1zCmd;
  delete fAdHocPMT1angleXCmd;
  delete fAdHocPMT1angleYCmd;

  delete fAdHocPMT2xCmd;
  delete fAdHocPMT2yCmd;
  delete fAdHocPMT2zCmd;
  delete fAdHocPMT2angleXCmd;
  delete fAdHocPMT2angleYCmd;

  delete fAdHocPMT3xCmd;
  delete fAdHocPMT3yCmd;
  delete fAdHocPMT3zCmd;
  delete fAdHocPMT3angleXCmd;
  delete fAdHocPMT3angleYCmd;

  delete fAdHocPMT4xCmd;
  delete fAdHocPMT4yCmd;
  delete fAdHocPMT4zCmd;
  delete fAdHocPMT4angleXCmd;
  delete fAdHocPMT4angleYCmd;

  delete fAdHocPMT5xCmd;
  delete fAdHocPMT5yCmd;
  delete fAdHocPMT5zCmd;
  delete fAdHocPMT5angleXCmd;
  delete fAdHocPMT5angleYCmd;

  delete fVetoReflectivityCmd;
  delete fpmtQECmd;
  delete fMainScintYield;
  delete fWLSScintYield;

  delete fPmtDRCmd;
  delete fPmtDZCmd;
  delete fVetoPmtDRCmd;
  delete fVetoPmtDZCmd;

  delete fDepthCmd;
  delete fScintRCmd;
  delete fScintHCmd;
  delete fVetoDRCmd;
  delete fVetoDHCmd;

  delete fPrintSettings;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if( command == fDimensionsCmd ){
    fLXeDetector->SetDimensions(fDimensionsCmd->GetNew3VectorValue(newValue));
  }
  else if (command == fHousingThicknessCmd){
    fLXeDetector->SetHousingThickness(fHousingThicknessCmd
                                     ->GetNewDoubleValue(newValue));
  }
  else if (command == fPmtRadiusCmd){
    fLXeDetector->SetPMTRadius(fPmtRadiusCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fNxCmd){
    fLXeDetector->SetNX(fNxCmd->GetNewIntValue(newValue));
  }
  else if (command == fNyCmd){
    fLXeDetector->SetNY(fNyCmd->GetNewIntValue(newValue));
  }
  else if (command == fNzCmd){
    fLXeDetector->SetNZ(fNzCmd->GetNewIntValue(newValue));
  }
  else if (command == fUpdateCmd){
    fLXeDetector->UpdateGeometry();
  }
  else if (command == fDefaultsCmd){
    fLXeDetector->SetDefaults();
  }
  else if (command == fSphereCmd){
    fLXeDetector->SetSphereOn(fSphereCmd->GetNewBoolValue(newValue));
  }
  else if (command == fSaveHitCmd){
    fLXeDetector->SaveHitOn(fSaveHitCmd->GetNewBoolValue(newValue));
  }
  else if (command == fSidePMTsRotationAngleXCmd){
    fLXeDetector->SetSidePMTsRotationAngleX(fSidePMTsRotationAngleXCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT1xCmd){
    fLXeDetector->SetAdHocPMT1x(fAdHocPMT1xCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT1yCmd){
    fLXeDetector->SetAdHocPMT1y(fAdHocPMT1yCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT1zCmd){
    fLXeDetector->SetAdHocPMT1z(fAdHocPMT1zCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT1angleXCmd){
    fLXeDetector->SetAdHocPMT1angleX(fAdHocPMT1angleXCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT1angleYCmd){
    fLXeDetector->SetAdHocPMT1angleY(fAdHocPMT1angleYCmd->GetNewDoubleValue(newValue));
  }

  else if (command == fAdHocPMT2xCmd){
    fLXeDetector->SetAdHocPMT2x(fAdHocPMT2xCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT2yCmd){
    fLXeDetector->SetAdHocPMT2y(fAdHocPMT2yCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT2zCmd){
    fLXeDetector->SetAdHocPMT2z(fAdHocPMT2zCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT2angleXCmd){
    fLXeDetector->SetAdHocPMT2angleX(fAdHocPMT2angleXCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT2angleYCmd){
    fLXeDetector->SetAdHocPMT2angleY(fAdHocPMT2angleYCmd->GetNewDoubleValue(newValue));
  }

  else if (command == fAdHocPMT3xCmd){
    fLXeDetector->SetAdHocPMT3x(fAdHocPMT3xCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT3yCmd){
    fLXeDetector->SetAdHocPMT3y(fAdHocPMT3yCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT3zCmd){
    fLXeDetector->SetAdHocPMT3z(fAdHocPMT3zCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT3angleXCmd){
    fLXeDetector->SetAdHocPMT3angleX(fAdHocPMT3angleXCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT3angleYCmd){
    fLXeDetector->SetAdHocPMT3angleY(fAdHocPMT3angleYCmd->GetNewDoubleValue(newValue));
  }

  else if (command == fAdHocPMT4xCmd){
    fLXeDetector->SetAdHocPMT4x(fAdHocPMT4xCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT4yCmd){
    fLXeDetector->SetAdHocPMT4y(fAdHocPMT4yCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT4zCmd){
    fLXeDetector->SetAdHocPMT4z(fAdHocPMT4zCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT4angleXCmd){
    fLXeDetector->SetAdHocPMT4angleX(fAdHocPMT4angleXCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT4angleYCmd){
    fLXeDetector->SetAdHocPMT4angleY(fAdHocPMT4angleYCmd->GetNewDoubleValue(newValue));
  }

  else if (command == fAdHocPMT5xCmd){
    fLXeDetector->SetAdHocPMT5x(fAdHocPMT5xCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT5yCmd){
    fLXeDetector->SetAdHocPMT5y(fAdHocPMT5yCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT5zCmd){
    fLXeDetector->SetAdHocPMT5z(fAdHocPMT5zCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT5angleXCmd){
    fLXeDetector->SetAdHocPMT5angleX(fAdHocPMT5angleXCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMT5angleYCmd){
    fLXeDetector->SetAdHocPMT5angleY(fAdHocPMT5angleYCmd->GetNewDoubleValue(newValue));
  }

  else if (command == fSidePMTsRotationAngleYCmd){
    fLXeDetector->SetSidePMTsRotationAngleY(fSidePMTsRotationAngleYCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fSidePMTsAimedAtAxisCmd){
    fLXeDetector->SetSidePMTsAimedAtAxis(fSidePMTsAimedAtAxisCmd->GetNewBoolValue(newValue));
  }
  else if (command == fTopPMTsRotationAngleYCmd){
    fLXeDetector->SetTopPMTsRotationAngleY(fTopPMTsRotationAngleYCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fBottomPMTsRotationAngleYCmd){
    fLXeDetector->SetBottomPMTsRotationAngleY(fBottomPMTsRotationAngleYCmd->GetNewDoubleValue(newValue));
  }
  //set reflectivities
  else if (command == fScintReflectivityCmd){
    fLXeDetector
      ->SetScintHousingReflectivity(fScintReflectivityCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fHousingLobeReflectivityCmd){
    fLXeDetector
      ->SetHousingLobeReflectivity(fHousingLobeReflectivityCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fPMTBaseReflectivityCmd){
    fLXeDetector
      ->SetPMTBaseReflectivity(fPMTBaseReflectivityCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fPMTsOnTopCmd){
    fLXeDetector
      ->SetPMTsOnTop(fPMTsOnTopCmd->GetNewBoolValue(newValue));
  }

  else if (command == fPutMirrorCmd){
    fLXeDetector
      ->SetPutMirror(fPutMirrorCmd->GetNewBoolValue(newValue));
  }
  else if (command == fMirrorX0Cmd){
    fLXeDetector
      ->SetMirrorX0(fMirrorX0Cmd->GetNewDoubleValue(newValue));
  }
  else if (command == fMirrorY0Cmd){
    fLXeDetector
      ->SetMirrorY0(fMirrorY0Cmd->GetNewDoubleValue(newValue));
  }
  else if (command == fMirrorZ0Cmd){
    fLXeDetector
      ->SetMirrorZ0(fMirrorZ0Cmd->GetNewDoubleValue(newValue));
  }
  else if (command == fMirrorLACmd){
    fLXeDetector
      ->SetMirrorLA(fMirrorLACmd->GetNewDoubleValue(newValue));
  }
  else if (command == fMirrorLBCmd){
    fLXeDetector
      ->SetMirrorLB(fMirrorLBCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fMirrorLCCmd){
    fLXeDetector
      ->SetMirrorLC(fMirrorLCCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fMirrorAngleCmd){
    fLXeDetector
      ->SetMirrorAngle(fMirrorAngleCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fPutMirror2Cmd){
    fLXeDetector
      ->SetPutMirror2(fPutMirror2Cmd->GetNewBoolValue(newValue));
  }
  else if (command == fMirror2X0Cmd){
    fLXeDetector
      ->SetMirror2X0(fMirror2X0Cmd->GetNewDoubleValue(newValue));
  }
  else if (command == fMirror2Y0Cmd){
    fLXeDetector
      ->SetMirror2Y0(fMirror2Y0Cmd->GetNewDoubleValue(newValue));
  }
  else if (command == fMirror2Z0Cmd){
    fLXeDetector
      ->SetMirror2Z0(fMirror2Z0Cmd->GetNewDoubleValue(newValue));
  }
  else if (command == fMirror2LACmd){
    fLXeDetector
      ->SetMirror2LA(fMirror2LACmd->GetNewDoubleValue(newValue));
  }
  else if (command == fMirror2LBCmd){
    fLXeDetector
      ->SetMirror2LB(fMirror2LBCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fMirror2LCCmd){
    fLXeDetector
      ->SetMirror2LC(fMirror2LCCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fMirror2AngleCmd){
    fLXeDetector
      ->SetMirror2Angle(fMirror2AngleCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fAdHocPMTsCmd){
    fLXeDetector
      ->SetAdHocPMTs(fAdHocPMTsCmd->GetNewBoolValue(newValue));
  }
  else if (command == fPMTsOnBottomCmd){
    fLXeDetector
      ->SetPMTsOnBottom(fPMTsOnBottomCmd->GetNewBoolValue(newValue));
  }
  else if (command == fPMTsOnPositiveXCmd){
    fLXeDetector
      ->SetPMTsOnPositiveX(fPMTsOnPositiveXCmd->GetNewBoolValue(newValue));
  }
  else if (command == fPMTsOnNegativeXCmd){
    fLXeDetector
      ->SetPMTsOnNegativeX(fPMTsOnNegativeXCmd->GetNewBoolValue(newValue));
  }
  else if (command == fVetoReflectivityCmd){
    fLXeDetector
      ->SetVetoHousingReflectivity(fVetoReflectivityCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fpmtQECmd){
    fLXeDetector
      ->SetpmtQE(fpmtQECmd->GetNewDoubleValue(newValue));
  }
  else if (command == fWlsCmd){
    fLXeDetector->SetWLSSlabOn(fWlsCmd->GetNewBoolValue(newValue));
  }
  else if (command == fLxeCmd){
    fLXeDetector->SetMainVolumeOn(fLxeCmd->GetNewBoolValue(newValue));
  }
  else if (command == fNFibersCmd){
    fLXeDetector->SetNFibers(fNFibersCmd->GetNewIntValue(newValue));
  }
  else if (command == fMainScintYield){
   fLXeDetector->SetMainScintYield(fMainScintYield->GetNewDoubleValue(newValue));
  }
  else if (command == fWLSScintYield){
    fLXeDetector->SetWLSScintYield(fWLSScintYield->GetNewDoubleValue(newValue));
  }
  //set pmt spacings
  else if (command == fPmtDRCmd){
    fLXeDetector->SetPmtDR(fPmtDRCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fPmtDZCmd){
    fLXeDetector->SetPmtDZ(fPmtDZCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fVetoPmtDRCmd){
    fLXeDetector->SetPmtDR_Veto(fVetoPmtDRCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fVetoPmtDZCmd){
    fLXeDetector->SetPmtDZ_Veto(fVetoPmtDZCmd->GetNewDoubleValue(newValue));
  }
  //set detector dimension
  else if (command == fDepthCmd){
    fLXeDetector->SetDepth(fDepthCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fScintRCmd){
    fLXeDetector->SetScintR(fScintRCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fScintHCmd){
    fLXeDetector->SetScintH(fScintHCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fVetoDRCmd){
    fLXeDetector->SetVetoDR(fVetoDRCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fVetoDHCmd){
    fLXeDetector->SetVetoDH(fVetoDHCmd->GetNewDoubleValue(newValue));
  }
  //print settings
  else if (command == fPrintSettings){
    fLXeDetector->PrintSettings();
  }
}

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
/// \file optical/LXe/include/LXeDetectorMessenger.hh
/// \brief Definition of the LXeDetectorMessenger class
//
//
#ifndef LXeDetectorMessenger_h
#define LXeDetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class LXeDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;

class LXeDetectorMessenger: public G4UImessenger
{
  public:

    LXeDetectorMessenger(LXeDetectorConstruction*);
    virtual ~LXeDetectorMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:

    LXeDetectorConstruction*     fLXeDetector;
    G4UIdirectory*               fDetectorDir;
    G4UIdirectory*               fVolumesDir;
    G4UIcmdWith3VectorAndUnit*   fDimensionsCmd;
    G4UIcmdWithADoubleAndUnit*   fHousingThicknessCmd;
    G4UIcmdWithADoubleAndUnit*   fPmtRadiusCmd;
    G4UIcmdWithAnInteger*        fNxCmd;
    G4UIcmdWithAnInteger*        fNyCmd;
    G4UIcmdWithAnInteger*        fNzCmd;
    G4UIcmdWithABool*            fSphereCmd;
    G4UIcmdWithABool*            fSaveHitCmd;
    G4UIcmdWithADouble*          fScintReflectivityCmd;
    G4UIcmdWithADouble*          fPMTBaseReflectivityCmd;
    G4UIcmdWithADouble*          fHousingLobeReflectivityCmd;
    G4UIcmdWithADouble*          fSidePMTsRotationAngleXCmd;
    G4UIcmdWithADouble*          fSidePMTsRotationAngleYCmd;
    G4UIcmdWithABool*          fSidePMTsAimedAtAxisCmd;
    G4UIcmdWithADouble*          fTopPMTsRotationAngleYCmd;
    G4UIcmdWithADouble*          fBottomPMTsRotationAngleYCmd;
    G4UIcmdWithABool*            fPMTsOnTopCmd;
    G4UIcmdWithABool*            fPMTsOnBottomCmd;
    G4UIcmdWithABool*            fPMTsOnPositiveXCmd;
    G4UIcmdWithABool*            fPMTsOnNegativeXCmd;
    G4UIcmdWithABool*            fAdHocPMTsCmd;
    G4UIcmdWithABool*            fPutMirrorCmd;
    G4UIcmdWithADouble*            fMirrorX0Cmd;
    G4UIcmdWithADouble*            fMirrorY0Cmd;
    G4UIcmdWithADouble*            fMirrorZ0Cmd;
    G4UIcmdWithADouble*            fMirrorLACmd;
    G4UIcmdWithADouble*            fMirrorLBCmd;
    G4UIcmdWithADouble*            fMirrorLCCmd;
    G4UIcmdWithADouble*            fMirrorAngleCmd;
    G4UIcmdWithABool*            fPutMirror2Cmd;
    G4UIcmdWithADouble*            fMirror2X0Cmd;
    G4UIcmdWithADouble*            fMirror2Y0Cmd;
    G4UIcmdWithADouble*            fMirror2Z0Cmd;
    G4UIcmdWithADouble*            fMirror2LACmd;
    G4UIcmdWithADouble*            fMirror2LBCmd;
    G4UIcmdWithADouble*            fMirror2LCCmd;
    G4UIcmdWithADouble*            fMirror2AngleCmd;
    G4UIcmdWithADouble*          fAdHocPMT1xCmd;
    G4UIcmdWithADouble*          fAdHocPMT1yCmd;
    G4UIcmdWithADouble*          fAdHocPMT1zCmd;
    G4UIcmdWithADouble*          fAdHocPMT1angleXCmd;
    G4UIcmdWithADouble*          fAdHocPMT1angleYCmd;
    G4UIcmdWithADouble*          fAdHocPMT2xCmd;
    G4UIcmdWithADouble*          fAdHocPMT2yCmd;
    G4UIcmdWithADouble*          fAdHocPMT2zCmd;
    G4UIcmdWithADouble*          fAdHocPMT2angleXCmd;
    G4UIcmdWithADouble*          fAdHocPMT2angleYCmd;

    G4UIcmdWithADouble*          fAdHocPMT3xCmd;
    G4UIcmdWithADouble*          fAdHocPMT3yCmd;
    G4UIcmdWithADouble*          fAdHocPMT3zCmd;
    G4UIcmdWithADouble*          fAdHocPMT3angleXCmd;
    G4UIcmdWithADouble*          fAdHocPMT3angleYCmd;

    G4UIcmdWithADouble*          fAdHocPMT4xCmd;
    G4UIcmdWithADouble*          fAdHocPMT4yCmd;
    G4UIcmdWithADouble*          fAdHocPMT4zCmd;
    G4UIcmdWithADouble*          fAdHocPMT4angleXCmd;
    G4UIcmdWithADouble*          fAdHocPMT4angleYCmd;

    G4UIcmdWithADouble*          fAdHocPMT5xCmd;
    G4UIcmdWithADouble*          fAdHocPMT5yCmd;
    G4UIcmdWithADouble*          fAdHocPMT5zCmd;
    G4UIcmdWithADouble*          fAdHocPMT5angleXCmd;
    G4UIcmdWithADouble*          fAdHocPMT5angleYCmd;

    G4UIcmdWithADouble*          fVetoReflectivityCmd;
    G4UIcmdWithADouble*          fpmtQECmd;
    G4UIcmdWithABool*            fWlsCmd;
    G4UIcmdWithABool*            fLxeCmd;
    G4UIcmdWithAnInteger*        fNFibersCmd;
    G4UIcommand*                 fUpdateCmd;
    G4UIcommand*                 fDefaultsCmd;
    G4UIcmdWithADouble*        fMainScintYield;
    G4UIcmdWithADouble*        fWLSScintYield;

    G4UIcmdWithADoubleAndUnit*   fPmtDRCmd;
    G4UIcmdWithADoubleAndUnit*   fPmtDZCmd;
    G4UIcmdWithADoubleAndUnit*   fVetoPmtDRCmd;
    G4UIcmdWithADoubleAndUnit*   fVetoPmtDZCmd;

    G4UIcmdWithADoubleAndUnit*   fDepthCmd;
    G4UIcmdWithADoubleAndUnit*   fScintRCmd;
    G4UIcmdWithADoubleAndUnit*   fScintHCmd;
    G4UIcmdWithADoubleAndUnit*   fVetoDRCmd;
    G4UIcmdWithADoubleAndUnit*   fVetoDHCmd;

    G4UIcmdWithoutParameter*     fPrintSettings;
};

#endif

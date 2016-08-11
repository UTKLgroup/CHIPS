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
/// \file optical/LXe/include/LXeDetectorConstruction.hh
/// \brief Definition of the LXeDetectorConstruction class
//
//
#ifndef LXeDetectorConstruction_H
#define LXeDetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Box;
class G4Tubs;
class LXePMTSD;
class LXeScintSD;
class G4Sphere;

#include "G4Material.hh"
#include "LXeDetectorMessenger.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4VUserDetectorConstruction.hh"

class LXeDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    LXeDetectorConstruction();
    virtual ~LXeDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    //Functions to modify the geometry
    void SetDimensions(G4ThreeVector );
    void SetHousingThickness(G4double );
    void SetNX(G4int );
    void SetNY(G4int );
    void SetNZ(G4int );
    void SetPMTRadius(G4double );
    void SetDefaults();

    void SetPmtDR(G4double ); 
    void SetPmtDZ(G4double ); 
    void SetPmtDR_Veto(G4double ); 
    void SetPmtDZ_Veto(G4double ); 

    void SetDepth(G4double );
    void SetScintR(G4double );
    void SetScintH(G4double );
    void SetVetoDR(G4double );
    void SetVetoDH(G4double );

    //Get values
    G4double GetScintX(){return fScint_x;}
    G4double GetScintY(){return fScint_y;}
    G4double GetScintZ(){return fScint_z;}

    G4double GetHousingThickness(){return fD_mtl;}
    G4int GetNX(){return fNx;}
    G4int GetNY(){return fNy;}
    G4int GetNZ(){return fNz;}
    G4int GetNR(){return fNr;}
    G4int GetNR_Veto(){return fNr_veto;}

    G4double GetPMTRadius(){return fOuterRadius_pmt;}
    G4double GetSlabZ(){return fSlab_z;}
 
    G4double GetPmtDR(){return fpmtdr;}
    G4double GetPmtDZ(){return fpmtdz;}
    G4double GetPmtDR_Veto(){return fpmtdr_veto;}
    G4double GetPmtDZ_Veto(){return fpmtdz_veto;}

    G4double GetDepth(){return fDepth;}
    G4double GetScintR(){return fScint_r;}
    G4double GetScintH(){return fScint_h;}
    G4double GetVetoDR(){return fVeto_dr;}
    G4double GetVetoDH(){return fVeto_dh;}

    //rebuild the geometry based on changes. must be called
    void UpdateGeometry();
    G4bool GetUpdated(){return fUpdated;}

    void SetSphereOn(G4bool b){fSphereOn=b; fUpdated=true;}
    static G4bool GetSphereOn(){return fSphereOn;}

    void SaveHitOn(G4bool b){fSaveHitOn=b; fUpdated=true;}
    static G4bool GetSaveHitOn(){return fSaveHitOn;}

    void SetScintHousingReflectivity(G4double scintr){fScintRefl = scintr; fUpdated=true;}
    G4double GetScintHousingReflectivity(){return fScintRefl;}

    void SetHousingLobeReflectivity(G4double scintr){fHousingLobeReflectivity = scintr; fUpdated=true;}
    G4double GetHousingLobeReflectivity(){return fHousingLobeReflectivity;}

    void SetPMTBaseReflectivity(G4double scintr){fPMTBaseReflectivity = scintr; fUpdated=true;}
    G4double GetPMTBaseReflectivity(){return fPMTBaseReflectivity;}

    void SetSidePMTsRotationAngleX(G4double scintr){fSidePMTsRotationAngleX = scintr; fUpdated=true;}
    G4double GetSidePMTsRotationAngleX(){return fSidePMTsRotationAngleX;}

    void SetSidePMTsRotationAngleY(G4double scintr){fSidePMTsRotationAngleY = scintr; fUpdated=true;}
    G4double GetSidePMTsRotationAngleY(){return fSidePMTsRotationAngleY;}

    void SetSidePMTsAimedAtAxis(G4bool scintr){fSidePMTsAimedAtAxis = scintr; fUpdated=true;}
    G4bool GetSidePMTsAimedAtAxis(){return fSidePMTsAimedAtAxis;}

    void SetTopPMTsRotationAngleY(G4double scintr){fTopPMTsRotationAngleY = scintr; fUpdated=true;}
    G4double GetTopPMTsRotationAngleY(){return fTopPMTsRotationAngleY;}

    void SetBottomPMTsRotationAngleY(G4double scintr){fBottomPMTsRotationAngleY = scintr; fUpdated=true;}
    G4double GetBottomPMTsRotationAngleY(){return fBottomPMTsRotationAngleY;}

    void SetPMTsOnTop(G4bool scintr){fPMTsOnTop = scintr; fUpdated=true;}
    G4bool GetPMTsOnTop(){return fPMTsOnTop;}

    void SetPMTsOnBottom(G4bool scintr){fPMTsOnBottom = scintr; fUpdated=true;}
    G4bool GetPMTsOnBottom(){return fPMTsOnBottom;}

    void SetPMTsOnPositiveX(G4bool scintr){fPMTsOnPositiveX = scintr; fUpdated=true;}
    G4bool GetPMTsOnPositiveX(){return fPMTsOnPositiveX;}

    void SetPMTsOnNegativeX(G4bool scintr){fPMTsOnNegativeX = scintr; fUpdated=true;}
    G4bool GetPMTsOnNegativeX(){return fPMTsOnNegativeX;}

    void SetAdHocPMTs(G4bool scintr){fAdHocPMTs = scintr; fUpdated=true;}
    G4bool GetAdHocPMTs(){return fAdHocPMTs;}

    void SetPutMirror(G4bool scintr){fPutMirror = scintr; fUpdated=true;}
    G4bool GetPutMirror(){return fPutMirror;}
    void SetMirrorX0(G4double scintr){fMirrorX0 = scintr; fUpdated=true;}
    G4double GetMirrorX0(){return fMirrorX0;}
    void SetMirrorY0(G4double scintr){fMirrorY0 = scintr; fUpdated=true;}
    G4double GetMirrorY0(){return fMirrorY0;}
    void SetMirrorZ0(G4double scintr){fMirrorZ0 = scintr; fUpdated=true;}
    G4double GetMirrorZ0(){return fMirrorZ0;}
    void SetMirrorLA(G4double scintr){fMirrorLA = scintr; fUpdated=true;}
    G4double GetMirrorLA(){return fMirrorLA;}
    void SetMirrorLB(G4double scintr){fMirrorLB = scintr; fUpdated=true;}
    G4double GetMirrorLB(){return fMirrorLB;}
    void SetMirrorLC(G4double scintr){fMirrorLC = scintr; fUpdated=true;}
    G4double GetMirrorLC(){return fMirrorLC;}
    void SetMirrorAngle(G4double scintr){fMirrorAngle = scintr; fUpdated=true;}
    G4double GetMirrorAngle(){return fMirrorAngle;}

    void SetPutMirror2(G4bool scintr){fPutMirror2 = scintr; fUpdated=true;}
    G4bool GetPutMirror2(){return fPutMirror2;}
    void SetMirror2X0(G4double scintr){fMirror2X0 = scintr; fUpdated=true;}
    G4double GetMirror2X0(){return fMirror2X0;}
    void SetMirror2Y0(G4double scintr){fMirror2Y0 = scintr; fUpdated=true;}
    G4double GetMirror2Y0(){return fMirror2Y0;}
    void SetMirror2Z0(G4double scintr){fMirror2Z0 = scintr; fUpdated=true;}
    G4double GetMirror2Z0(){return fMirror2Z0;}
    void SetMirror2LA(G4double scintr){fMirror2LA = scintr; fUpdated=true;}
    G4double GetMirror2LA(){return fMirror2LA;}
    void SetMirror2LB(G4double scintr){fMirror2LB = scintr; fUpdated=true;}
    G4double GetMirror2LB(){return fMirror2LB;}
    void SetMirror2LC(G4double scintr){fMirror2LC = scintr; fUpdated=true;}
    G4double GetMirror2LC(){return fMirror2LC;}
    void SetMirror2Angle(G4double scintr){fMirror2Angle = scintr; fUpdated=true;}
    G4double GetMirror2Angle(){return fMirror2Angle;}

    void SetAdHocPMT1x(G4double scintr){fAdHocPMT1x = scintr; fUpdated=true;}
    G4double GetAdHocPMT1x(){return fAdHocPMT1x;}
    void SetAdHocPMT1y(G4double scintr){fAdHocPMT1y = scintr; fUpdated=true;}
    G4double GetAdHocPMT1y(){return fAdHocPMT1y;}
    void SetAdHocPMT1z(G4double scintr){fAdHocPMT1z = scintr; fUpdated=true;}
    G4double GetAdHocPMT1z(){return fAdHocPMT1z;}
    void SetAdHocPMT1angleX(G4double scintr){fAdHocPMT1angleX = scintr; fUpdated=true;}
    G4double GetAdHocPMT1angleX(){return fAdHocPMT1angleX;}
    void SetAdHocPMT1angleY(G4double scintr){fAdHocPMT1angleY = scintr; fUpdated=true;}
    G4double GetAdHocPMT1angleY(){return fAdHocPMT1angleY;}

    void SetAdHocPMT2x(G4double scintr){fAdHocPMT2x = scintr; fUpdated=true;}
    G4double GetAdHocPMT2x(){return fAdHocPMT2x;}
    void SetAdHocPMT2y(G4double scintr){fAdHocPMT2y = scintr; fUpdated=true;}
    G4double GetAdHocPMT2y(){return fAdHocPMT2y;}
    void SetAdHocPMT2z(G4double scintr){fAdHocPMT2z = scintr; fUpdated=true;}
    G4double GetAdHocPMT2z(){return fAdHocPMT2z;}
    void SetAdHocPMT2angleX(G4double scintr){fAdHocPMT2angleX = scintr; fUpdated=true;}
    G4double GetAdHocPMT2angleX(){return fAdHocPMT2angleX;}
    void SetAdHocPMT2angleY(G4double scintr){fAdHocPMT2angleY = scintr; fUpdated=true;}
    G4double GetAdHocPMT2angleY(){return fAdHocPMT2angleY;}

    void SetAdHocPMT3x(G4double scintr){fAdHocPMT3x = scintr; fUpdated=true;}
    G4double GetAdHocPMT3x(){return fAdHocPMT3x;}
    void SetAdHocPMT3y(G4double scintr){fAdHocPMT3y = scintr; fUpdated=true;}
    G4double GetAdHocPMT3y(){return fAdHocPMT3y;}
    void SetAdHocPMT3z(G4double scintr){fAdHocPMT3z = scintr; fUpdated=true;}
    G4double GetAdHocPMT3z(){return fAdHocPMT3z;}
    void SetAdHocPMT3angleX(G4double scintr){fAdHocPMT3angleX = scintr; fUpdated=true;}
    G4double GetAdHocPMT3angleX(){return fAdHocPMT3angleX;}
    void SetAdHocPMT3angleY(G4double scintr){fAdHocPMT3angleY = scintr; fUpdated=true;}
    G4double GetAdHocPMT3angleY(){return fAdHocPMT3angleY;}

    void SetAdHocPMT4x(G4double scintr){fAdHocPMT4x = scintr; fUpdated=true;}
    G4double GetAdHocPMT4x(){return fAdHocPMT4x;}
    void SetAdHocPMT4y(G4double scintr){fAdHocPMT4y = scintr; fUpdated=true;}
    G4double GetAdHocPMT4y(){return fAdHocPMT4y;}
    void SetAdHocPMT4z(G4double scintr){fAdHocPMT4z = scintr; fUpdated=true;}
    G4double GetAdHocPMT4z(){return fAdHocPMT4z;}
    void SetAdHocPMT4angleX(G4double scintr){fAdHocPMT4angleX = scintr; fUpdated=true;}
    G4double GetAdHocPMT4angleX(){return fAdHocPMT4angleX;}
    void SetAdHocPMT4angleY(G4double scintr){fAdHocPMT4angleY = scintr; fUpdated=true;}
    G4double GetAdHocPMT4angleY(){return fAdHocPMT4angleY;}

    void SetAdHocPMT5x(G4double scintr){fAdHocPMT5x = scintr; fUpdated=true;}
    G4double GetAdHocPMT5x(){return fAdHocPMT5x;}
    void SetAdHocPMT5y(G4double scintr){fAdHocPMT5y = scintr; fUpdated=true;}
    G4double GetAdHocPMT5y(){return fAdHocPMT5y;}
    void SetAdHocPMT5z(G4double scintr){fAdHocPMT5z = scintr; fUpdated=true;}
    G4double GetAdHocPMT5z(){return fAdHocPMT5z;}
    void SetAdHocPMT5angleX(G4double scintr){fAdHocPMT5angleX = scintr; fUpdated=true;}
    G4double GetAdHocPMT5angleX(){return fAdHocPMT5angleX;}
    void SetAdHocPMT5angleY(G4double scintr){fAdHocPMT5angleY = scintr; fUpdated=true;}
    G4double GetAdHocPMT5angleY(){return fAdHocPMT5angleY;}

    void SetVetoHousingReflectivity(G4double vetor){fVetoRefl = vetor; fUpdated=true;}
    G4double GetVetoHousingReflectivity(){return fVetoRefl;}

    void SetpmtQE(G4double qe){fpmtQE = qe; fUpdated=true;}
    G4double GetpmtQE(){return fpmtQE;}

    void SetWLSSlabOn(G4bool b){fWLSslab=b; fUpdated=true;}
    G4bool GetWLSSlabOn(){return fWLSslab;}

    void SetMainVolumeOn(G4bool b){fMainVolume=b; fUpdated=true;}
    G4bool GetMainVolumeOn(){return fMainVolume;}

    void SetNFibers(G4int n){fNfibers=n; fUpdated=true;}
    G4int GetNFibers(){return fNfibers;}

    void SetMainScintYield(G4double );
    void SetWLSScintYield(G4double );

    //print settings
    void PrintSettings();

  private:

    void DefineMaterials();
    G4VPhysicalVolume* ConstructDetector();

    LXeDetectorMessenger* fDetectorMessenger;

    G4bool fUpdated;
 
    G4Box* fExperimentalHall_box;
    G4LogicalVolume* fExperimentalHall_log;
    G4VPhysicalVolume* fExperimentalHall_phys;

    //Materials & Elements
    G4Material* fLXe;
    G4Material* fAl;
    G4Element* fN;
    G4Element* fO;
    G4Material* fAir;
    G4Material* fVacuum;
    G4Element* fC;
    G4Element* fH;
    G4Material* fGlass;
    G4Material* fPstyrene;
    G4Material* fPMMA;
    G4Material* fPethylene1;
    G4Material* fPethylene2;
    G4Material* fWater;

    //Geometry
    G4double fDepth; 

    G4double fScint_x;
    G4double fScint_y;
    G4double fScint_z;

    G4double fScint_r;
    G4double fScint_h;

    G4double fVeto_dr;
    G4double fVeto_dh;

    G4double fD_mtl;
    G4int fNx;
    G4int fNy;
    G4int fNz;
    G4int fNr;
    G4int fNr_veto;

    G4double fpmtdr;
    G4double fpmtdz;
    G4double fpmtdr_veto;
    G4double fpmtdz_veto;

    G4double fOuterRadius_pmt;
    G4int fNfibers;
    static G4bool fSphereOn;
    static G4bool fSaveHitOn;

    G4double fScintRefl;
    G4double fHousingLobeReflectivity;
    G4double fPMTBaseReflectivity;
    G4double fSidePMTsRotationAngleX;
    G4double fSidePMTsRotationAngleY;
    G4bool fSidePMTsAimedAtAxis;
    G4double fTopPMTsRotationAngleY;
    G4double fBottomPMTsRotationAngleY;
    G4bool fPMTsOnTop;
    G4bool fPMTsOnBottom;
    G4bool fPMTsOnPositiveX;
    G4bool fPMTsOnNegativeX;
    G4bool fAdHocPMTs;
    G4bool fPutMirror;
    G4double fMirrorX0;
    G4double fMirrorY0;
    G4double fMirrorZ0;
    G4double fMirrorLA;
    G4double fMirrorLB;
    G4double fMirrorLC;
    G4double fMirrorAngle;
    G4bool fPutMirror2;
    G4double fMirror2X0;
    G4double fMirror2Y0;
    G4double fMirror2Z0;
    G4double fMirror2LA;
    G4double fMirror2LB;
    G4double fMirror2LC;
    G4double fMirror2Angle;

    G4double fAdHocPMT1x;
    G4double fAdHocPMT1y;
    G4double fAdHocPMT1z;
    G4double fAdHocPMT1angleX;
    G4double fAdHocPMT1angleY;
    G4double fAdHocPMT2x;
    G4double fAdHocPMT2y;
    G4double fAdHocPMT2z;
    G4double fAdHocPMT2angleX;
    G4double fAdHocPMT2angleY;
    G4double fAdHocPMT3x;
    G4double fAdHocPMT3y;
    G4double fAdHocPMT3z;
    G4double fAdHocPMT3angleX;
    G4double fAdHocPMT3angleY;
    G4double fAdHocPMT4x;
    G4double fAdHocPMT4y;
    G4double fAdHocPMT4z;
    G4double fAdHocPMT4angleX;
    G4double fAdHocPMT4angleY;
    G4double fAdHocPMT5x;
    G4double fAdHocPMT5y;
    G4double fAdHocPMT5z;
    G4double fAdHocPMT5angleX;
    G4double fAdHocPMT5angleY;
    G4double fVetoRefl;
    G4double fpmtQE;

    G4bool fWLSslab;
    G4bool fMainVolume;
    G4double fSlab_z;

    G4MaterialPropertiesTable* fLXe_mt;
    G4MaterialPropertiesTable* fWater_mt;
    G4MaterialPropertiesTable* fMPTPStyrene;

};

#endif

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
/// \file optical/LXe/include/LXeMainVolume.hh
/// \brief Definition of the LXeMainVolume class
//
#ifndef LXeMainVolume_H
#define LXeMainVolume_H 1

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"

#include "LXeDetectorConstruction.hh"

class LXeMainVolume : public G4PVPlacement
{
  public:

    LXeMainVolume(G4RotationMatrix *pRot,
                 const G4ThreeVector &tlate,
                 G4LogicalVolume *pMotherLogical,
                 G4bool pMany,
                 G4int pCopyNo,
                 LXeDetectorConstruction* c);

  private:

    void VisAttributes();
    void SurfaceProperties();

    void PlacePMTs(G4LogicalVolume* pmt_Log,
                   G4RotationMatrix* rot, G4double &a, G4double &b, G4double da,
                   G4double db, G4double amin, G4double bmin, G4int na, G4int nb,
                   G4double &x, G4double &y, G4double &z, G4int &k,LXePMTSD* sd);

    void PlaceVetoPMTs(G4LogicalVolume* pmt_log,
		       G4int nr, G4double dz,
		       G4double r, G4double halfh, 
		       G4double mtl, 
		       G4double veto_dr, G4double veto_dh,
		       G4double pmtr,
		       G4int &k,      LXePMTSD* sd);
  
    G4int NumofPMTonDisk(G4double r, G4double pmtr, G4double d);

    void PlacePMTonDisk(G4LogicalVolume* pmt_log, G4LogicalVolume* mothervol_log,
			G4double r, G4double z, 
			G4double pmtr, G4double d, 
			G4double angleY,
			//			G4bool up,
			G4int &k, LXePMTSD* sd,
			G4String phyvolname);

  void PlaceAdHocPMT(G4double x, G4double y, G4double z, G4double angleX, G4double angleY, 
		     G4LogicalVolume* pmt_log, G4LogicalVolume* mothervol_log,
		     G4int &k, LXePMTSD* sd, G4String phyvolname);


    void CopyValues();

    G4bool fUpdated;
  
    LXeDetectorConstruction* fConstructor;

    G4double fScint_x;
    G4double fScint_y;
    G4double fScint_z;

    G4double fDepth;
    G4double fScint_h;
    G4double fScint_r;
    G4double fVeto_dr;
    G4double fVeto_dh;

    G4double fD_mtl;
    G4int fNx;
    G4int fNy;
    G4int fNz;
    G4int fNz_veto;
    G4int fNr;
    G4int fNr_veto;
    G4int fNtop;
    G4int fNtop_veto;

    G4double fpmtdr;
    G4double fpmtdz;
    G4double fpmtdr_veto;
    G4double fpmtdz_veto;

    G4double fOuterRadius_pmt;
    G4bool fSphereOn;
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

    //Basic Volumes
    //
    G4Box* fScint_box;
    G4Box* fHousing_box;
    G4Sphere* fSphere;

    G4Tubs* fHousing_tub;
    G4Tubs* fVeto_tub;
    G4Tubs* fIDout_tub;
    G4Tubs* fIDin_tub;
    G4Tubs* fScint_tub;

    G4Tubs* fPmt;
    G4Tubs* fPhotocath;
    G4Tubs* fPmtbase;

    G4Box* fMirror_box;
    G4LogicalVolume* fMirror_log;
    G4Box* fMirror2_box;
    G4LogicalVolume* fMirror2_log;

    // Logical volumes
    //
    G4LogicalVolume* fSphere_log;

    static G4LogicalVolume* fHousing_log;
    G4LogicalVolume* fVeto_log;
    G4LogicalVolume* fIDout_log;
    G4LogicalVolume* fIDin_log;
    G4LogicalVolume* fScint_log;

    G4LogicalVolume* fPmt_log;
    G4LogicalVolume* fPhotocath_log;
    G4LogicalVolume* fPmtbase_log;

    //Sensitive Detectors
    static LXeScintSD* fScint_SD;
    static LXePMTSD* fPmt_SD;
};

#endif

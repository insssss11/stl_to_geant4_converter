#include "STLParser.hh"

#include "G01DetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4TessellatedSolid.hh"
#include <fstream>
#include "G4TriangularFacet.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"

G01DetectorConstruction::G01DetectorConstruction(G4String file, G4String foramt, G4String unit)
{
  fSolid = STLParser::Instance()->ParseStlAscii("test_solid", file, unit);
  // else if(format == "-b")
  //   fSolid = STLParser::Instance()->ParseStlBinary("test_solid", file, unit);
}

G4VPhysicalVolume* G01DetectorConstruction::Construct()
{
  G4Element* elN = new G4Element("Nitrogen", "N", 7, 14.01*g/mole);
  G4Element* elO = new G4Element("Oxygen", "O", 8, 16.00*g/mole);
  G4Material* air = new G4Material("L2Air", 1.290*mg/cm3, 2);
  air->AddElement(elN, 70*perCent);air->AddElement(elO, 30*perCent);
  G4double worldHalfSizeX = 3000.*mm;
  G4double worldHalfSizeY = 3000.*mm;
  G4double worldHalfSizeZ = 3000.*mm;
  fSolidWorld   = new G4Box("World", worldHalfSizeX, worldHalfSizeY, worldHalfSizeZ);
  fLogicWorld   = new G4LogicalVolume(fSolidWorld, air, "World");
  fPhysWorld    = new G4PVPlacement(0,    G4ThreeVector(),  fLogicWorld,  "World",  0, false, 0, true);    
  auto logicTest = new G4LogicalVolume(fSolid, air, "Test");
  new G4PVPlacement(0,    G4ThreeVector(),  logicTest,  "Test",  fLogicWorld, false, 0, true);  
  return fPhysWorld;
}
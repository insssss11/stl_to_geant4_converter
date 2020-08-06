// written by Hyunmin Yang, HANUL, KOREA university
// Implement of STLParser class
// This singleton class to convert directly stl file to G4Tessellated instance

#ifndef STLPARSER_HH
#define STLPARSER_HH 1

#include "G4ThreeVector.hh"
#include "G4TessellatedSolid.hh"
#include <fstream>
#include "G4TriangularFacet.hh"
#include "G4SystemOfUnits.hh"
#include "G4String.hh"

class STLParser{
  public:
    static STLParser* Instance();
    G4TessellatedSolid* ParseStlAscii(G4String name, G4String file, G4String unit = "mm");
    G4TessellatedSolid* ParseStlBinary(G4String name, G4String file, G4String unit = "mm");
    ~STLParser()
    {
      fInstance = nullptr;
    }
  private:
    STLParser(){}
    void ArrangeCCW(G4ThreeVector& nor, G4ThreeVector& vec1, G4ThreeVector& vec2, G4ThreeVector& vec3);

  private:
    static STLParser* fInstance;
};

#endif

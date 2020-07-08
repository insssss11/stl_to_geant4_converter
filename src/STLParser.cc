#include "STLParser.hh"

#include "G4TessellatedSolid.hh"
#include "G4TriangularFacet.hh"
#include <iostream>
#include <fstream>

STLParser *STLParser::fInstance = nullptr;

STLParser *STLParser::Instance()
{
  if (fInstance == nullptr)
  {
    fInstance = new STLParser();
  }
  return fInstance;
}

G4TessellatedSolid *STLParser::ParseStlAscii(G4String name, G4String file, G4String unit)
{
  G4double u = 0;
  std::ifstream fileIn;
  fileIn.open(file.data(), std::ios::in);
  char buffer[256];
  if (unit == "mm")
    u = mm;
  else if (unit == "cm")
    u = cm;
  else if (unit == "m")
    u = m;
  else if (!fileIn.is_open())
  {
    std::ostringstream message;
    message << "File Name : " << file.data();
    G4Exception("STLParser::ParseStlAscii(G4String, G4String)", "STLPARSER0001",
                FatalErrorInArgument, message);
  }
  else
  {
    std::ostringstream message;
    message << "Input unit : " << unit.data();
    G4Exception("STLParser::ParseStlAscii(G4String, G4String)", "STLPARSER0000",
                FatalErrorInArgument, message);
  }
  G4TessellatedSolid *ts = new G4TessellatedSolid(name);
  G4TriangularFacet *fc;
  G4ThreeVector normal, vertex1, vertex2, vertex3;
  G4double x, y, z;
  G4int vertex = 0, nFacet = 0;

  // parsing STL file
  fileIn.getline(buffer, 256);
  // G4cout << buffer << G4endl;
  while (true)
  {
    if (vertex == 0)
    {
      fileIn >> buffer;
      G4String checkEndSolid(buffer);
      if (checkEndSolid.contains("endsolid"))
        break;
      // G4cout << "  " << buffer << " ";
      fileIn >> buffer;
      // G4cout << buffer << " ";
      fileIn >> x >> y >> z;
      // G4cout << x << " " << y << " " << z;
      normal = G4ThreeVector(x * u, y * u, z * u);
      fileIn.getline(buffer, 256);
      // G4cout << buffer << G4endl;
      fileIn.getline(buffer, 256);
      // G4cout << buffer << G4endl;
      vertex++;
    }
    else if (vertex == 1)
    {
      fileIn >> buffer;
      // G4cout << "      " << buffer << " ";
      fileIn >> x >> y >> z;
      // G4cout << x << " " << y << " " << z;
      vertex1 = G4ThreeVector(x * u, y * u, z * u);
      fileIn.getline(buffer, 256);
      // G4cout << buffer << G4endl;
      vertex++;
    }
    else if (vertex == 2)
    {
      fileIn >> buffer;
      // G4cout << "      " << buffer << " ";
      fileIn >> x >> y >> z;
      // G4cout << x << " " << y << " " << z;
      vertex2 = G4ThreeVector(x * u, y * u, z * u);
      fileIn.getline(buffer, 256);
      // G4cout << buffer << G4endl;
      vertex++;
    }
    else if (vertex == 3)
    {
      fileIn >> buffer;
      // G4cout << "      " << buffer << " ";
      fileIn >> x >> y >> z;
      // G4cout << x << " " << y << " " << z;
      vertex3 = G4ThreeVector(x * u, y * u, z * u);
      fileIn.getline(buffer, 256);
      // G4cout << buffer << G4endl;
      fileIn.getline(buffer, 256);
      // G4cout << buffer << G4endl;
      fileIn.getline(buffer, 256);
      // G4cout << buffer << G4endl;
      ArrangeCCW(normal, vertex1, vertex2, vertex3);
      fc = new G4TriangularFacet(vertex1, vertex2, vertex3, ABSOLUTE);
      ts->AddFacet(fc);
      nFacet++;
      vertex = 0;
    }
    if (fileIn.eof())
      break;
  }
  ts->SetSolidClosed(true);
  fileIn.close();
  G4cout << "-------------------------------------------------" << G4endl;
  G4cout << "G4TessellatedSolid Name : " << ts->GetName() << G4endl;
  G4cout << "# of Facets             : " << ts->GetNumberOfFacets() << G4endl;
  G4cout << "Allocated Memory        : " << ts->AllocatedMemory() / 1024 << " kB" << G4endl;
  G4cout << "-------------------------------------------------" << G4endl;
  return ts;
}

void STLParser::ArrangeCCW(G4ThreeVector &nor, G4ThreeVector &vec1, G4ThreeVector &vec2, G4ThreeVector &vec3)
{
  G4ThreeVector center(
      (vec1.x() + vec2.x() + vec3.x()) / 3,
      (vec1.y() + vec2.y() + vec3.y()) / 3,
      (vec1.z() + vec2.z() + vec3.z()) / 3);
  G4ThreeVector diff[3];
  diff[0] = G4ThreeVector(vec1 - center), diff[1] = G4ThreeVector(vec2 - center), diff[2] = G4ThreeVector(vec3 - center);
  if (nor.dot(diff[0].cross(diff[1])) < 0)
    std::swap(vec1, vec2);
  if (nor.dot(diff[0].cross(diff[2])) < 0)
    std::swap(vec1, vec3);
  if (nor.dot(diff[1].cross(diff[2])) < 0)
    std::swap(vec2, vec3);
}

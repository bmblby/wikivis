#ifndef H_GLOOST_ISOSURFACEGENERATION
#define H_GLOOST_ISOSURFACEGENERATION



/* This Code was taken from http://paulbourke.net/geometry/polygonise */

// File Name: IsoSurfaceGeneration.h
// Last Modified: 29.07.2015
// Author: Raghavendra Chandrashekara (basesd on source code
// provided by Paul Bourke and Cory Gene Bloyd)
// Email: rc99@doc.ic.ac.uk, rchandrashekara@hotmail.com
//
// Description: This is the interface file for the IsoSurfaceGeneration class.
// IsoSurfaceGeneration can be used to construct an isosurface from a scalar
// field.

/* adapted for the use with the gloost framework July 2015 by Felix Weißig, thesleeper@gmx.net */

// gloost includes
#include <gloost/Point3.h>
#include <gloost/Vector3.h>
#include <gloost/Mesh.h>
#include <gloost/VolumeData.h>

// cpp includes
#include <map>
#include <vector>

namespace gloost
{

struct POINT3DID {
	unsigned newID;
	float x, y, z;
};

typedef std::map<unsigned, POINT3DID> ID2POINT3DID;
typedef gloost::Mesh::TriangleIndices TRIANGLE;
typedef std::vector<TRIANGLE>         TRIANGLEVECTOR;
typedef gloost::Point3                POINT3D;
typedef gloost::Vector3               VECTOR3D;

template <class FieldType>
class IsoSurfaceGeneration {
public:
	// Constructor and destructor.
	IsoSurfaceGeneration();
	~IsoSurfaceGeneration();

	// Generates the isosurface from the scalar field contained in the
	// buffer ptScalarField[].
	bool generateSurface(const std::shared_ptr<const VolumeData<FieldType> >& volume,
                       FieldType isoLevel);

	// Returns true if a valid surface has been generated.
	bool isSurfaceValid();

	// Deletes the isosurface.
	void deleteSurface();

	// returns the mesh of the generated iso surface
	const std::shared_ptr<gloost::Mesh>& getMesh() const;

protected:

	std::shared_ptr<gloost::Mesh> _mesh;

	// List of POINT3Ds which form the isosurface.
	ID2POINT3DID m_i2pt3idVertices;

	// List of TRIANGLES which form the triangulation of the isosurface.
	TRIANGLEVECTOR m_trivecTriangles;

	// Returns the edge ID.
	unsigned GetEdgeID(unsigned nX, unsigned nY, unsigned nZ, unsigned nEdgeNo);

	// Returns the vertex ID.
	unsigned getVertexID(unsigned nX, unsigned nY, unsigned nZ);

	// Calculates the intersection point of the isosurface with an
	// edge.
	POINT3DID calculateIntersection(unsigned nX, unsigned nY, unsigned nZ, unsigned nEdgeNo);

	// Interpolates between two grid points to produce the point at which
	// the isosurface intersects an edge.
	POINT3DID Interpolate(float fX1, float fY1, float fZ1,
                        float fX2, float fY2, float fZ2,
                        FieldType tVal1, FieldType tVal2);

	// Renames vertices and triangles so that they can be accessed more
	// efficiently.
	void RenameVerticesAndTriangles();

	// No. of cells in x, y, and z directions.
	unsigned m_nCellsX, m_nCellsY, m_nCellsZ;

	// The buffer holding the scalar field.
	std::shared_ptr<const gloost::VolumeData<FieldType> > _volume;

	// The isosurface value.
	FieldType m_tIsoLevel;

	// Indicates whether a valid surface is present.
	bool m_bValidSurface;

	// Lookup tables used in the construction of the isosurface.
	static const int m_edgeTable[256];
	static const int m_triTable[256][16];
};
#endif // H_GLOOST_ISOSURFACEGENERATION


} // namespace gloost

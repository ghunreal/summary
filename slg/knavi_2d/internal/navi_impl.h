/*
 * Copyright (c) 2014-2015, dennis wang
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#ifndef NAVI_IMPL_H
#define NAVI_IMPL_H

#include <string>

#include "Recast.h"
#include "DetourCrowd.h"
#include "DetourCommon.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"
#include "DetourNavMeshBuilder.h"

#include "navi.h"

namespace knavi {

struct NaviPoint3D;
struct rcChunkyTriMeshNode;
struct rcChunkyTriMesh;
struct ConvexVolume;
class rcMeshLoaderObj;
class InputGeom;
class Navi;

enum KNaviPolyFlags
{
	KNAVI_POLYFLAGS_WALK	 = 0x01,	// Ability to walk (ground, grass, road)
	KNAVI_POLYFLAGS_SWIM	 = 0x02,	// Ability to swim (water).
	KNAVI_POLYFLAGS_DOOR	 = 0x04,	// Ability to move through doors.
	KNAVI_POLYFLAGS_JUMP	 = 0x08,	// Ability to jump.
	KNAVI_POLYFLAGS_DISABLED = 0x10,	// Disabled polygon
	KNAVI_POLYFLAGS_ALL		 = 0xffff	// All abilities.
};

enum KNaviPolyAreas
{
	KNAVI_POLYAREA_GROUND,
	KNAVI_POLYAREA_WATER,
	KNAVI_POLYAREA_ROAD,
	KNAVI_POLYAREA_DOOR,
	KNAVI_POLYAREA_GRASS,
	KNAVI_POLYAREA_JUMP,
};

struct rcChunkyTriMeshNode
{
	float bmin[2], bmax[2];
	int i, n;
};

struct rcChunkyTriMesh
{
	inline rcChunkyTriMesh() : nodes(0), tris(0) {};
	inline ~rcChunkyTriMesh() { delete [] nodes; delete [] tris; }

	rcChunkyTriMeshNode* nodes;
	int nnodes;
	int* tris;
	int ntris;
	int maxTrisPerChunk;
};

/// Creates partitioned triangle mesh (AABB tree),
/// where each node contains at max trisPerChunk triangles.
bool rcCreateChunkyTriMesh(const float* verts, const int* tris, int ntris,
						   int trisPerChunk, rcChunkyTriMesh* cm);

/// Returns the chunk indices which overlap the input rectable.
int rcGetChunksOverlappingRect(const rcChunkyTriMesh* cm, float bmin[2], float bmax[2], int* ids, const int maxIds);

/// Returns the chunk indices which overlap the input segment.
int rcGetChunksOverlappingSegment(const rcChunkyTriMesh* cm, float p[2], float q[2], int* ids, const int maxIds);

class rcMeshLoaderObj
{
public:
	rcMeshLoaderObj();
	~rcMeshLoaderObj();
	
	bool load(const char* fileName);

	inline const float* getVerts() const { return m_verts; }
	inline const float* getNormals() const { return m_normals; }
	inline const int* getTris() const { return m_tris; }
	inline int getVertCount() const { return m_vertCount; }
	inline int getTriCount() const { return m_triCount; }
	inline const char* getFileName() const { return m_filename; }

private:
	
	void addVertex(float x, float y, float z, int& cap);
	void addTriangle(int a, int b, int c, int& cap);
	
	char m_filename[260];
	float m_scale;	
	float* m_verts;
	int* m_tris;
	float* m_normals;
	int m_vertCount;
	int m_triCount;
};

static const int MAX_CONVEXVOL_PTS = 12;
struct ConvexVolume
{
	float verts[MAX_CONVEXVOL_PTS*3];
	float hmin, hmax;
	int nverts;
	int area;
};

class InputGeom
{
	rcChunkyTriMesh* m_chunkyMesh;
	rcMeshLoaderObj* m_mesh;
	float m_meshBMin[3], m_meshBMax[3];
	
	/// @name Off-Mesh connections.
	///@{
	static const int MAX_OFFMESH_CONNECTIONS = 256;
	float m_offMeshConVerts[MAX_OFFMESH_CONNECTIONS*3*2];
	float m_offMeshConRads[MAX_OFFMESH_CONNECTIONS];
	unsigned char m_offMeshConDirs[MAX_OFFMESH_CONNECTIONS];
	unsigned char m_offMeshConAreas[MAX_OFFMESH_CONNECTIONS];
	unsigned short m_offMeshConFlags[MAX_OFFMESH_CONNECTIONS];
	unsigned int m_offMeshConId[MAX_OFFMESH_CONNECTIONS];
	int m_offMeshConCount;
	///@}

	/// @name Convex Volumes.
	///@{
	static const int MAX_VOLUMES = 256;
	ConvexVolume m_volumes[MAX_VOLUMES];
	int m_volumeCount;
	///@}
	
public:
	InputGeom();
	~InputGeom();
	
	bool loadMesh(class rcContext* ctx, const char* filepath);	
	bool load(class rcContext* ctx, const char* filepath);
	
	/// Method to return static mesh data.
	inline const rcMeshLoaderObj* getMesh() const { return m_mesh; }
	inline const float* getMeshBoundsMin() const { return m_meshBMin; }
	inline const float* getMeshBoundsMax() const { return m_meshBMax; }
	inline const rcChunkyTriMesh* getChunkyMesh() const { return m_chunkyMesh; }

	/// @name Off-Mesh connections.
	///@{
	int getOffMeshConnectionCount() const { return m_offMeshConCount; }
	const float* getOffMeshConnectionVerts() const { return m_offMeshConVerts; }
	const float* getOffMeshConnectionRads() const { return m_offMeshConRads; }
	const unsigned char* getOffMeshConnectionDirs() const { return m_offMeshConDirs; }
	const unsigned char* getOffMeshConnectionAreas() const { return m_offMeshConAreas; }
	const unsigned short* getOffMeshConnectionFlags() const { return m_offMeshConFlags; }
	const unsigned int* getOffMeshConnectionId() const { return m_offMeshConId; }
	///@}

	/// @name Box Volumes.
	///@{
	int getConvexVolumeCount() const { return m_volumeCount; }
	const ConvexVolume* getConvexVolumes() const { return m_volumes; }
	void addConvexVolume(const float* verts, const int nverts,
						 const float minh, const float maxh, unsigned char area);
	void deleteConvexVolume(int i);
	void drawConvexVolumes(struct duDebugDraw* dd, bool hilight = false);
	///@}
};

class NaviImpl : public Navi {
public:
    NaviImpl(int maxAgent = 50);
    virtual ~NaviImpl();

    virtual bool load(const char* filePath);
    virtual void update(uint32_t ms);
    virtual NaviAgent* addAgent(float radius, const NaviPoint3D& position);
    virtual void removeAgent(NaviAgent* agent);
    virtual bool find(const NaviPoint3D& start, const NaviPoint3D& end, NaviPoint3D* path, int* size);
    virtual bool raycast(const NaviPoint3D& start, const NaviPoint3D& end, NaviPoint3D& hitPoint);
    virtual const char* getError();

private:
    void find();
    bool handleBuild();
	void resetCommonSettings();
    void cleanup();
    void reset();
    inline dtCrowd* getCrowd() {
        return m_crowd;
    }
    inline dtPolyRef getPolyRefByPos(const NaviPoint3D& pos) {
        dtPolyRef polyRef;
        float posArray[3] = {.0f};
        posArray[0] = pos.x;
        posArray[1] = pos.y;
        posArray[2] = pos.z;
        m_navQuery->findNearestPoly(posArray, m_polyPickExt, &m_filter, &polyRef, 0);
        return polyRef;
    }

    friend class NaviAgentImpl;

private:
    InputGeom* m_geom;
	dtNavMesh* m_navMesh;
	dtNavMeshQuery* m_navQuery;
	dtCrowd* m_crowd;
    rcContext m_ctx;
    dtQueryFilter m_filter;

	unsigned char m_navMeshDrawFlags;

	float m_cellSize;
	float m_cellHeight;
	float m_agentHeight;
	float m_agentRadius;
	float m_agentMaxClimb;
	float m_agentMaxSlope;
	float m_regionMinSize;
	float m_regionMergeSize;
	float m_edgeMaxLen;
	float m_edgeMaxError;
	float m_vertsPerPoly;
	float m_detailSampleDist;
	float m_detailSampleMaxError;

    bool m_keepInterResults;

	unsigned char* m_triareas;
	rcHeightfield* m_solid;
	rcCompactHeightfield* m_chf;
	rcContourSet* m_cset;
	rcPolyMesh* m_pmesh;
	rcConfig m_cfg;	
	rcPolyMeshDetail* m_dmesh;

    static const int MAX_POLYS = 256;
	static const int MAX_SMOOTH = 2048;
	
	dtPolyRef m_startRef;
	dtPolyRef m_endRef;
	dtPolyRef m_polys[MAX_POLYS];
	int m_npolys;
	float m_polyPickExt[3];
	float m_smoothPath[MAX_SMOOTH*3];
	int m_nsmoothPath;

	float m_spos[3];
	float m_epos[3];

    char m_error[512];
    int m_maxAgent;
};

#define NAVI_TRY try {
#define NAVI_CATCH \
        } catch(std::exception& e) { \
            int size = strlen(e.what()); \
            memcpy(m_error, e.what(), size); \
            m_error[size] = 0; \
        }
}

#define NAVI_ERROR(e) \
    memcpy(m_error, e, strlen(e) + 1); \
    m_error[strlen(e)] = 0;

#endif /* NAVI_IMPL_H */

//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <math/Forward.h>
#include <util/Forward.h>

struct rcHeightfield;
struct rcCompactHeightfield;
struct rcContourSet;
struct rcPolyMesh;
struct rcPolyMeshDetail;
struct rcConfig;

class rcContext;

typedef unsigned int dtStatus;
typedef unsigned int dtPolyRef;
typedef unsigned int dtTileRef;

class dtNavMeshQuery;
class dtQueryFilter;
class dtNavMesh;

class btCollisionWorld;
class btDynamicsWorld;
class btDiscreteDynamicsWorld;
class btBroadphaseInterface;
class btConstraintSolver;

class btCollisionShape;
class btStridingMeshInterface;
class btCollisionObject;
class btRigidBody;
class btPersistentManifold;

class btCollisionConfiguration;
class btCollisionDispatcher;


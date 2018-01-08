#include "DemoBruteForceTex.h"

#include <node3/Camera.h>

namespace eterrain3d
{

DemoBruteForceTex::DemoBruteForceTex(n3::Camera& cam)
	: m_cam(cam)
{
}

void DemoBruteForceTex::Load()
{
	m_cam.Pitch(-60);
	m_cam.MoveToward(8);

	//load the height map in
//	// 1. load from image
//	m_bf.LoadHeightMap("demo02/height128.raw", 128);
	// 2. gen 1
	m_bf.MakeTerrainPlasma(128, 1.0f);
//	// 3. gen 2
//	m_bf.MakeTerrainFault(128, 8, 0, 1, 0.4);

	m_bf.SetHeightScale(0.025f);

	//load the terrain3d texture in
// 	// 1. load one texture
// 	m_bf.LoadTexture("demo03/grass_1.tga");
	// 2. load multi textures
	m_bf.LoadTile( Terrain::LOWEST_TILE,  "demo03/lowestTile.tga" );
	m_bf.LoadTile( Terrain::LOW_TILE,     "demo03/lowTile.tga" );
	m_bf.LoadTile( Terrain::HIGH_TILE,    "demo03/highTile.tga" );
	m_bf.LoadTile( Terrain::HIGHEST_TILE, "demo03/highestTile.tga" );
	// 3. load detail map
	//load the terrain's detail map
	m_bf.LoadDetailMap("demo03/detailMap.tga");

// 	g_bruteForce.LoadDetailMap( "../Data/detailMap.tga" );
// 	g_bruteForce.DoDetailMapping( g_bDetail, 8 );

	m_bf.GenerateTextureMap( 256 );
}

void DemoBruteForceTex::Draw() const
{
	m_bf.Draw();
}

}
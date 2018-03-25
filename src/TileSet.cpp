#include "TileSet.h"


namespace ME
{
	TileSet::TileSet(Nz::MaterialRef material, TileMapParams params) : TileMapBase(params),
		m_tileSet(m_size, m_tileSize),
		m_material(material)
	{
		m_tileSet.SetMaterial(0, m_material);
	
		m_entityTileSet = m_world.CreateEntity();
		Ndk::NodeComponent& tileSetNodeComponent = m_entityTileSet->AddComponent<Ndk::NodeComponent>();
		Ndk::GraphicsComponent& tileSetGraphicsComponent = m_entityTileSet->AddComponent<Ndk::GraphicsComponent>();
		tileSetGraphicsComponent.Attach(&m_tileSet);
		Display();
	}
	
	void TileSet::Display()
	{
		for (unsigned int y = 0; y < m_size.y; ++y)
		{
			for (unsigned int x = 0; x < m_size.x; ++x)
			{
				m_posTiles.emplace_back(Nz::Vector2ui(x, y));
				m_textureRectTiles.emplace_back(Nz::Rectui(x * 32, y * 32, 32, 32));
				m_tileSet.EnableTile(m_posTiles.back(), m_textureRectTiles.back());
			}
		}
	}
}
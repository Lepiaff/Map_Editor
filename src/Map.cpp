#include "Map.h"

namespace ME
{
	Map::Map(Nz::MaterialRef material, TileMapParams params) : ME::TileMapBase(params),
		m_map(m_size, m_tileSize)
	{
		//m_map.SetMaterial(0, m_material);
	}

	void Map::DeleteSelection()
	{
		Nz::Rectui selectionIds = m_selection.GetSelectionIds();

		for (std::size_t y = selectionIds.y; y < selectionIds.height; ++y)
		{
			for (std::size_t x = selectionIds.x; x < selectionIds.width; ++x)
			{
				m_map.DisableTile({ x, y });
			}
		}
		m_selection.Clear();
	}

	void Map::Display()
	{

	}
}
#pragma once
#ifndef MAP_H
#define MAP_H

#include <Nazara/Graphics/TileMap.hpp>

#include "TileMapBase.h"

namespace ME
{
	class Map : public ME::TileMapBase
	{
	private:
		//Attributs
		Nz::TileMap m_map;


		virtual void Display() override;

	public:
		Map(Nz::MaterialRef material, TileMapParams params);
		~Map() = default;

		void DeleteSelection();
		
	};
}
#endif // !MAP_H



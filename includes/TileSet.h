#pragma once
#ifndef TILESET_H
#define TILESET_H

#include <vector>

#include <Nazara/Core/String.hpp>
#include <Nazara/Graphics/TileMap.hpp>
#include <Nazara/Graphics/Material.hpp>
#include <Nazara/Renderer.hpp>

#include <NDK/Application.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/World.hpp>

#include "TileMapBase.h"

namespace ME
{
	class TileSet : public ME::TileMapBase
	{
	private:
		//Attributs
		///Attributs nécéssaire au TileMap
		Nz::MaterialRef m_material;
		std::vector<Nz::Vector2ui> m_posTiles;
		std::vector<Nz::Rectui> m_textureRectTiles;
		Ndk::EntityHandle m_entityTileSet;
		Nz::TileMap m_tileSet;


		//Méthode privée
		virtual void Display() override;
	public:
		TileSet(Nz::MaterialRef material, TileMapParams params);
		~TileSet() = default;
	};

	

}
#endif // !TILESET_H
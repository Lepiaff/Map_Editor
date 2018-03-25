#pragma once
#ifndef TILEMAPBASE_H
#define TILEMAPBASE_H

#include <iostream>
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

#include "Selection.h"

namespace ME
{
	class TileMapBase
	{
		struct TileMapParams;
	protected:
		Ndk::World& m_world;
		Nz::RenderWindow* m_window;
		Nz::Vector2f m_tileSize;
		Ndk::EntityHandle m_entityCamera;
		Nz::Vector2ui m_size;
		Nz::Rectf m_displayZone;
		ME::Selection m_selection;

	public:
		TileMapBase(TileMapParams params);
		~TileMapBase() = default;

		virtual void UpdateSelection(Nz::Rectui selection);
		virtual void UpdateCamera(const float delta);
		virtual void SetTargetRegion(Nz::Rectf region);

		virtual void Display() = 0;

		inline Ndk::World& GetWorld() const { return m_world; }
		inline virtual void ClearSelection() { m_selection.Clear(); }
		inline Ndk::CameraComponent& GetCamera() const { return m_entityCamera->GetComponent<Ndk::CameraComponent>(); }
		inline bool Contains(const Nz::Vector2i pos) const {
			return m_entityCamera->GetComponent<Ndk::CameraComponent>().GetTargetRegion().Contains(float(pos.x) / float(m_window->GetSize().x), float(pos.y) / float(m_window->GetSize().y));
		}
		inline bool Contains(const int x, const int y) const {
			return m_entityCamera->GetComponent<Ndk::CameraComponent>().GetTargetRegion().Contains(float(x)/float(m_window->GetSize().x), float(y) / float(m_window->GetSize().y));
		}

		Nz::Rectui GetSelectedTiles(Nz::Rectui selection, bool ordonned = true);

		struct TileMapParams
		{
			Nz::RenderWindow* window;
			Nz::Vector2f tileSize;
			Nz::Vector2ui size;
			Nz::Rectf displayZone;
		};
	};
}
#endif // !TILEMAPBASE_H
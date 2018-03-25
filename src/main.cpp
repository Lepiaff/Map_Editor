#include <Nazara/Graphics.hpp>
#include <Nazara/Renderer.hpp>
#include <Nazara/Utility.hpp>
#include <NDK/Application.hpp>
#include <NDK/Components.hpp>
#include <NDK/Systems.hpp>
#include <NDK/World.hpp>
#include <iostream>

#include <Nazara/Graphics/TileMap.hpp>
#include <Nazara/Platform/EventHandler.hpp>
#include <NDK/Canvas.hpp>

#include "Selection.h"
#include "TileSet.h"
#include "Map.h"

int main()
{
	//Création de la fenêtre
	Ndk::Application application;

	Nz::RenderWindow& mainWindow = application.AddWindow<Nz::RenderWindow>();
	mainWindow.Create(Nz::VideoMode(800, 600, 32)/*::GetDesktopMode()*/, "Test"/*,Nz::WindowStyle_Fullscreen*/);

	//Variables de fonctionnement
	Nz::EventHandler& eventHandler = mainWindow.GetEventHandler();
	Nz::Vector2ui posOnClick(0, 0);
	Nz::Vector2ui posOnRelease(0, 0);

	enum MOUSE_POSITION
	{
		NOWHERE = 0,
		ON_TILESET = 1,
		ON_MAP = 2
	};

	MOUSE_POSITION MOUSE_ZONE = NOWHERE;
	
	//Création des TileMap
	///TileSet
	Nz::MaterialRef material = Nz::Material::New();
	material->LoadFromFile("D:/Programmation_2018/NazaraProject/NazaraProject/Ressources/Tilesets/32x32/Village.png");
	
	ME::TileMapBase::TileMapParams tileSetParams;
	tileSetParams.tileSize = Nz::Vector2f(32.f, 32.f);
	tileSetParams.window = &mainWindow;
	tileSetParams.size.x = material->GetDiffuseMap()->GetSize().x / tileSetParams.tileSize.x;
	tileSetParams.size.y = material->GetDiffuseMap()->GetSize().y / tileSetParams.tileSize.y;

	float tileSetRatioX = float(tileSetParams.size.x) * tileSetParams.tileSize.x / float(mainWindow.GetSize().x);
	tileSetParams.displayZone = { 0.f, 0.f, tileSetRatioX, 1.f };
	
	ME::TileSet tileSet(material, tileSetParams);
	
	///Map
	ME::TileSet::TileMapParams mapParams;
	mapParams.tileSize = Nz::Vector2f(32.f, 32.f);
	mapParams.window = &mainWindow;
	mapParams.size.x = 25;
	mapParams.size.y = 20;

	mapParams.displayZone = { tileSetRatioX + float(0.0005), 0, 1.f, 1.f };

	ME::Map map(material, mapParams);

	//Gestion des évenements
	///MOLETTE
	eventHandler.OnMouseWheelMoved.Connect([&](const Nz::EventHandler*, Nz::WindowEvent::MouseWheelEvent e)
	{
		std::cout << Nz::Mouse::GetPosition().x << " : " << Nz::Mouse::GetPosition().y << std::endl;
		if (tileSet.Contains(Nz::Mouse::GetPosition() - mainWindow.GetPosition())) //PAS TERRIBLE A CAUSE DES BORDURE DE LA FENETRE
		{///Evenement dans le tileSet
			tileSet.UpdateCamera(e.delta);
		}
		else
		{
			//Evenement dans la MAP
		}
	});

	///CLIC GAUCHE
	eventHandler.OnMouseButtonPressed.Connect([&](const Nz::EventHandler*, Nz::WindowEvent::MouseButtonEvent e)
	{
		posOnClick.x = e.x;
		posOnClick.y = e.y;

		/*if (tileSet.Contains(Nz::Mouse::GetPosition()))
		{
			
		}*/
	});
	eventHandler.OnMouseButtonReleased.Connect([&](const Nz::EventHandler*, Nz::WindowEvent::MouseButtonEvent e)
	{
		posOnRelease.x = e.x;
		posOnRelease.y = e.y;

		switch (MOUSE_ZONE)
		{
		case ON_TILESET:

			if (e.button == Nz::Mouse::Left)
			{
				Nz::Rectui SelectedTiles(posOnClick.x, posOnClick.y, e.x, e.y);
				tileSet.UpdateSelection(SelectedTiles);
			}
			break;

		case ON_MAP:

			if (e.button == Nz::Mouse::Left)
			{
				
			}
			break;

		default:

			break;
		}
	});
	eventHandler.OnMouseMoved.Connect([&](const Nz::EventHandler*, Nz::WindowEvent::MouseMoveEvent e)
	{
		if (tileSet.Contains(e.x, e.y))
		{
			MOUSE_ZONE = ON_TILESET;
			std::cout << "Mouse zone : TileSet" << std::endl;
		}
		else if (map.Contains(e.x, e.y))
		{
			MOUSE_ZONE = ON_MAP;
			std::cout << "Mouse zone : Map" << std::endl;
		}
		else
		{
			MOUSE_ZONE = NOWHERE;
			std::cout << "Mouse zone : Nowhere" << std::endl;
		}
			
		switch (MOUSE_ZONE)
		{
		case ON_TILESET :

			if (Nz::Mouse::IsButtonPressed(Nz::Mouse::Left))
			{
				Nz::Rectui SelectedTiles(posOnClick.x, posOnClick.y, e.x, e.y);
				tileSet.UpdateSelection(std::move(SelectedTiles));
			}
			break;

		case ON_MAP:

			if (Nz::Mouse::IsButtonPressed(Nz::Mouse::Left))
			{
				
			}
			break;

		default:

			break;
		}
	});
	///Redimensionnement de l'écran
	eventHandler.OnResized.Connect([&](const Nz::EventHandler*, Nz::WindowEvent::SizeEvent e)
	{
		//Redéfinition du ratio des éléments
		float tileSetRatioX = float(material->GetDiffuseMap()->GetSize().x) / float(mainWindow.GetSize().x);
		///Redéfinition de la target région de la caméra du tileSet
		Nz::Rectf displayZoneTileSet = { 0.f, 0.f, tileSetRatioX, 1.f };
		tileSet.SetTargetRegion(std::move(displayZoneTileSet));
		///Redéfinition de la target région de la caméra de la map
		Nz::Rectf displayZoneTileMap = { tileSetRatioX + float(0.0005), 0.f, 1.f, 1.f };
		map.SetTargetRegion(std::move(displayZoneTileMap));
	});

	///Clavier
	eventHandler.OnKeyPressed.Connect([&](const Nz::EventHandler*, Nz::WindowEvent::KeyEvent e)
	{
		switch(e.code)
		{
			case Nz::Keyboard::Escape :
				switch (MOUSE_ZONE)
				{
				case ON_TILESET :
					tileSet.ClearSelection();
					break;

				case ON_MAP:
					map.ClearSelection();
					break;

				default :
					///Nothing to do...
					break;
				}

			break;

			case Nz::Keyboard::Delete : 
				if (MOUSE_ZONE == ON_MAP)
					map.DeleteSelection();
				break;

			default :
				///Nothing to do...
				break;
		}
	});

	while (application.Run())
	{
		mainWindow.Display();
	}

	return EXIT_SUCCESS;
}



#include "pch.h"
#include "world.h"
#include "menu/util/control.h"
#include "world/blackhole.h"
#include "world/weather.h"
#include "world/time.h"
#include "world/water.h"
#include "world/rendering.h"
#include "world/world_glow.h"
#include "world/entities.h"
#include "world/clearing.h"
using namespace base::gui;
using namespace menu::world::vars;

namespace menu::world::vars {
	variables m_vars;



}

namespace menu {

	void world_menu::render() {
		renderer::addSubmenu("World", "World", [](core* core) {

			core->addOption(submenuOption("Weather")
				.setTarget("weather"));

			core->addOption(submenuOption("Time")
				.setTarget("time"));;

			core->addOption(submenuOption("Water")
				.setTarget("water/ocean"));

			core->addOption(submenuOption("Aesthtic Lights")
				.setTarget("world_glow"));

			core->addOption(submenuOption("Rendering")
				.setTarget("game_rendering"));

			core->addOption(submenuOption("Entities")
				.setTarget("local_entities"));

			core->addOption(submenuOption("Black Hole")
				.setTarget("black_hole"));

			core->addOption(submenuOption("Clearing")
				.setTarget("clearing"));

			core->addOption(toggleOption("Enable Sky")
				.addToggle(&patterns::m_disable_sky));

			core->addOption(toggleOption("Blackout")
				.addToggle(&m_vars.blackout));

			core->addOption(toggleOption("Decreased Ped Population")
				.addToggle(&m_vars.decrease_ped_population));

			core->addOption(toggleOption("Decreased Vehicle Population")
				.addToggle(&m_vars.decrease_vehicle_population));

			core->addOption(toggleOption("Domino Trail")
				.addToggle(&m_vars.dominos));

			core->addOption(numberOption<float>("Time Scale")
				.addNumber(&m_vars.time_scale).addMin(0.f).addMax(1.f).addStep(0.25f).setPrecision(2.f));

			core->addOption(buttonOption("Refresh Interior")
				.addClick([] { INTERIOR::REFRESH_INTERIOR(INTERIOR::GET_INTERIOR_FROM_ENTITY(PLAYER::PLAYER_PED_ID())); }));
		});
	}

	void world_menu::update() {
		render();
		getBlackholeMenu()->update();
		getWeatherMenu()->update();
		getTimeMenu()->update();
		getWaterMenu()->update();
		getWorldGlowMenu()->update();
		getRenderingMenu()->update();
		getEntitiesMenu()->update();
		getClearingMenu()->update();

		MISC::SET_TIME_SCALE(m_vars.time_scale);

		if (m_vars.blackout) {
			GRAPHICS::SET_ARTIFICIAL_LIGHTS_STATE(true);
		}
		else {
			GRAPHICS::SET_ARTIFICIAL_LIGHTS_STATE(false);
		}

		if (m_vars.decrease_ped_population) {
			PED::SET_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
		}
		if (m_vars.decrease_vehicle_population) {
			VEHICLE::SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
			VEHICLE::SET_PARKED_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0F);
		}
		if (m_vars.dominos) {
			Vector3 me = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0);
			Hash board = rage::joaat("prop_boogieboard_05");
			static int timer = 0;
			if (GetTickCount() - timer >= 200) {
				*(unsigned short*)patterns::set_this_thread_networked = 0x9090;
				Object domino = OBJECT::CREATE_OBJECT(board, { me.x, me.y - 1, me.z - 1 }, 1, 1, 1);
				*(unsigned short*)patterns::set_this_thread_networked = 0x0574;
				ENTITY::SET_ENTITY_HEADING(domino, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
				timer = GetTickCount();
			}
		}
	}
}
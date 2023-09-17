#include "pch.h"
#include "weapon.h"
#include "menu/util/globals.h"
#include "gui/options/option.h"
#include "gui/util/panels.h"
#include "rage/classes/CPed.h"
#include "rage/classes/CPlayerInfo.h"
#include "menu/util/control.h"
#include "menu/util/raycast.h"
#include "rage/invoker/natives.h"
#include "menu/util/lists.h"
#include "rage/engine.h"
#include "util/util.h"

#include "../main/weapon/paint_gun.h"
#include "../main/weapon/aimbot.h"
#include "../main/weapon/weapon_disables.h"
#include "../main/weapon/weapon_multipliers.h"
#include "../main/weapon/explosions.h"
#include "../main/weapon/entity_gun.h"
#include "../main/weapon/particle_gun.h"
#include "../main/weapon/gravity_gun.h"
#include "../main/weapon/weapon_tracers.h"

using namespace base::gui;
using namespace menu::weapon::mods::vars;

float DegreeToRadian(float deg)
{
	double radian = (3.14159265359 / 180) * deg;
	return (float)radian;
}

Vector3 Multiply(Vector3 vector, float incline)
{
	vector.x *= incline;
	vector.y *= incline;
	vector.z *= incline;
	return vector;
}

Vector3 RotationToDirection(Vector3 rotation)
{
	float x = DegreeToRadian(rotation.x);
	float z = DegreeToRadian(rotation.z);

	float num = abs(cos(x));

	return Vector3
	{
		-sin(z) * num,
		cos(z) * num,
		sin(x)
	};
}

Vector3 Add(Vector3 vector, Vector3 vector2)
{
	vector.x += vector2.x;
	vector.y += vector2.y;
	vector.z += vector2.z;
	return vector;
}

Vector3 CoordsInFrontOfCam(std::float_t distance)
{
	Vector3 pos = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(NULL);
	Vector3 dir = RotationToDirection(rot);
	Vector3 dist = Multiply(dir, distance);
	return Add(pos, dist);
}

void RequestControlOfid(Entity id)
{
	int tick = 0;
	while (!NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(id) && tick <= 25)
	{
		NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(id);
		tick++;
	}
}

void RequestControlOfEnt(Entity entity)
{
	int tick = 0;
	while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity) && tick <= 25)
	{
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);
		tick++;
	}
	if (NETWORK::NETWORK_IS_SESSION_STARTED())
	{
		int id = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(entity);
		RequestControlOfid(id);
		NETWORK::SET_NETWORK_ID_CAN_MIGRATE(id, 1);
	}
}

void ClearTasks(Ped ped)
{
	TASK::CLEAR_PED_TASKS(ped);
	TASK::CLEAR_PED_TASKS_IMMEDIATELY(ped);
	TASK::CLEAR_PED_SECONDARY_TASK(ped);
}

void DeleteEntity(Entity ent)
{
	ENTITY::DETACH_ENTITY(ent, true, true);
	ENTITY::SET_ENTITY_VISIBLE(ent, false, false);
	NETWORK::NETWORK_SET_ENTITY_ONLY_EXISTS_FOR_PARTICIPANTS(ent, true);
	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ent, { 0.f, 0.f, 0.f }, false, false, false);
	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(ent, true, true);
	ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&ent);
	ENTITY::DELETE_ENTITY(&ent);
	OBJECT::DELETE_OBJECT(&ent);
}

inline bool castray(Entity* ent)
{
	BOOL hit;
	Vector3 endCoords;
	Vector3 surfaceNormal;

	Vector3 camCoords = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(2);
	Vector3 dir = RotationToDirection(rot);
	Vector3 farCoords;

	farCoords.x = camCoords.x + dir.x * 1000;
	farCoords.y = camCoords.y + dir.y * 1000;
	farCoords.z = camCoords.z + dir.z * 1000;


	int ray = SHAPETEST::START_EXPENSIVE_SYNCHRONOUS_SHAPE_TEST_LOS_PROBE(
		{ camCoords.x,
		camCoords.y,
		camCoords.z },
		{ farCoords.x,
		farCoords.y,
		farCoords.z },
		-1,
		0,
		7);
	SHAPETEST::GET_SHAPE_TEST_RESULT(ray, &hit, &endCoords, &surfaceNormal, ent);

	return (bool)hit;
}

void upgrade_weapon_components(Ped ped, uint32_t weapon_hash) {
	for (const auto components : menu::lists::weapon_components) {
		if (WEAPON::DOES_WEAPON_TAKE_WEAPON_COMPONENT(weapon_hash, components.hash)) {
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(ped, weapon_hash, components.hash);
		}
	}
}

void remove_weapon_upgrades(Ped ped, uint32_t weapon_hash) {
	for (const auto components : menu::lists::weapon_components) {
		if (WEAPON::DOES_WEAPON_TAKE_WEAPON_COMPONENT(weapon_hash, components.hash)) {
			WEAPON::REMOVE_WEAPON_COMPONENT_FROM_PED(ped, weapon_hash, components.hash);
		}
	}
}

void give_weapon(Ped ped, uint32_t weapon_hash, bool equip_now, bool fully_upgraded) {
	int ammo = -1;
	WEAPON::GET_MAX_AMMO(ped, weapon_hash, &ammo);
	WEAPON::GIVE_WEAPON_TO_PED(ped, weapon_hash, ammo, false, equip_now);

	if (fully_upgraded) {
		upgrade_weapon_components(ped, weapon_hash);
	}
}

namespace menu::weapon::mods::vars {
	variables m_vars;
	int m_selected_crosshair;

	const char* crosshairs[11] = { "Cross",  "Target", "Arrow", "Holy Cross", "Reticle" };

	Vector3 get_direction(Vector3 rotation) {
		Vector3 tmp;
		tmp.y = rotation.z * 0.0174532924f;
		tmp.x = rotation.x * 0.0174532924f;
		tmp.z = abs(cos(tmp.x));

		return Vector3((-sin(tmp.y)) * tmp.z, (cos(tmp.y)) * tmp.z, sin(tmp.x));
	}

	bool isPedShooting(Ped ped) {
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(ped, 1);
		return PED::IS_PED_SHOOTING_IN_AREA(ped, coords, coords, true, true);
	}
}



namespace menu {

	void weapon_mods_menu::render() {
		renderer::addSubmenu("Weapons", "Weapons", [](core* core) {
			core->addOption(submenuOption("Aimbot")
				.setTarget("Aimbot"));

			core->addOption(submenuOption("Disables")
				.setTarget("Weapon Disables"));

			core->addOption(submenuOption("Multipliers")
				.setTarget("Weapon Multipliers"));

			core->addOption(submenuOption("Tracers")
				.setTarget("Weapon Tracers"));

			core->addOption(submenuOption("Explosion Gun")
				.setTarget("Weapon Explosions"));

			core->addOption(submenuOption("Entity Gun")
				.setTarget("Weapon Entity Gun"));

			core->addOption(submenuOption("Particle Gun")
				.setTarget("Weapon Particle Gun"));

			core->addOption(submenuOption("Light Gun")
				.setTarget("Light Gun"));

			core->addOption(submenuOption("Gravity Gun")
				.setTarget("Weapon Gravity Gun"));

			core->addOption(submenuOption("Crosshairs")
				.setTarget("Weapon Crosshairs"));

			core->addOption(toggleOption("Unlimited Ammo")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_unlimited_ammo));

			core->addOption(toggleOption("Rapid Fire")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_rapid_fire));

			core->addOption(toggleOption("Explosive Bullets")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_explosive_bullets));

			core->addOption(toggleOption("Incendiary Bullets")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_incendiary_bullets));

			core->addOption(toggleOption("Teleport")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_teleport));

			core->addOption(toggleOption("Airstrike")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_airstrike));

			core->addOption(toggleOption("TriggerBot")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_triggerbot));

			core->addOption(toggleOption("Delete Entity")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_delete_entity));

			core->addOption(toggleOption("Shrink Entity")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_shrink_entity));

			core->addOption(toggleOption("Spawn Money")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_spawn_money));

			core->addOption(toggleOption("Vehicle Hijack")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_vehicle_hijack));

			core->addOption(toggleOption("Clown VFX")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_clown_vfx));

			core->addOption(buttonOption("Give All Weapons")
				.addTranslate()
				.addClick([] {
					for (const auto& weapon : lists::g_weapons) {
						give_weapon(PLAYER::PLAYER_PED_ID(), weapon.id, true, false);
					}
					}));

			core->addOption(buttonOption("Remove Weapons")
				.addTranslate()
				.addClick([] {
					WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::PLAYER_PED_ID(), 0);
					}));

			core->addOption(buttonOption("Give Ammo")
				.addTranslate()
				.addClick([] {
					for (const auto& weapon : lists::g_weapons)
						WEAPON::ADD_AMMO_TO_PED(PLAYER::PLAYER_PED_ID(), weapon.id, 999999);
					}));

			core->addOption(buttonOption("Remove Ammo")
				.addTranslate()
				.addClick([] {
					for (const auto& weapon : lists::g_weapons)
						WEAPON::ADD_AMMO_TO_PED(PLAYER::PLAYER_PED_ID(), weapon.id, 0);
					}));

			core->addOption(buttonOption("Upgrade Weapons")
				.addTranslate()
				.addClick([] {
					for (const auto& weapon : lists::g_weapons)
						upgrade_weapon_components(PLAYER::PLAYER_PED_ID(), weapon.id);
					}));

			core->addOption(buttonOption("Downgrade Weapons")
				.addTranslate()
				.addClick([] {
					for (const auto& weapon : lists::g_weapons)
						remove_weapon_upgrades(PLAYER::PLAYER_PED_ID(), weapon.id);
					}));
			});

		renderer::addSubmenu("Crosshairs", "Weapon Crosshairs", [](core* core) {
			core->addOption(toggleOption("Toggle")
				.addToggle(&m_vars.m_toggle_crosshair));

			core->addOption(scrollOption<const char*, int>("Crosshair")
				.addScroll(&crosshairs).setPosition(&m_selected_crosshair));
			});


	}

	void weapon_mods_menu::update() {
		render();
		raycast::raycastUpdate();

		getPaintGunMenu()->update();
		getAimBotMenu()->update();
		getWeaponDisablesMenu()->update();
		getWeaponMultipliersMenu()->update();
		getWeaponExplosionsMenu()->update();
		getEntityGunMenu()->update();
		getParticleGunMenu()->update();
		getGravityGunMenu()->update();
		getWeaponTracersMenu()->update();

		uint32_t m_weapon_hash = 0;
		WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &m_weapon_hash, false);

		if (m_vars.m_unlimited_ammo) {
			WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), true);
		}

		if (m_vars.m_rapid_fire) {
			if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false)) {
				if (PAD::IS_CONTROL_PRESSED(0, ControlAttack)) {
					Vector3 start = CAM::GET_GAMEPLAY_CAM_COORD() + (get_direction(CAM::GET_GAMEPLAY_CAM_ROT(0)));
					Vector3 end = CAM::GET_GAMEPLAY_CAM_COORD() + (get_direction(CAM::GET_GAMEPLAY_CAM_ROT(0)) * 200.f);
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY(start, end, WEAPON::GET_WEAPON_DAMAGE_TYPE(m_weapon_hash), true, m_weapon_hash, PLAYER::PLAYER_PED_ID(), true, false, 500.0f, PLAYER::PLAYER_PED_ID(), 0);
				}
			}
		}

		if (m_vars.m_explosive_bullets) {
			if (patterns::ped_factory) {
				if ((*patterns::ped_factory)->m_local_ped) {
					if ((*patterns::ped_factory)->m_local_ped->m_player_info) {
						(*patterns::ped_factory)->m_local_ped->m_player_info->m_frame_flags |= 0x800;
					}
				}
			}
		}

		if (m_vars.m_incendiary_bullets) {
			if (patterns::ped_factory) {
				if ((*patterns::ped_factory)->m_local_ped) {
					if ((*patterns::ped_factory)->m_local_ped->m_player_info) {
						(*patterns::ped_factory)->m_local_ped->m_player_info->m_frame_flags |= 0x1000;
					}
				}
			}
		}


		if (m_vars.m_teleport) {
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			if (isPedShooting(playerPed)) {
				auto BulletCoord = raycast::m_ShapeTestDestination;
				if (!raycast::IsEmpty(BulletCoord))
					ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), BulletCoord, FALSE, TRUE, TRUE, FALSE);
			}
		}

		if (m_vars.m_airstrike) {
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			if (isPedShooting(playerPed)) {
				auto BulletCoord = raycast::m_ShapeTestDestination;
				if (!raycast::IsEmpty(BulletCoord)) {
					control::requestWeapon(0x47757124);
					control::requestWeapon(0xf8a3939f);

					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY(BulletCoord, BulletCoord, WEAPON::GET_WEAPON_DAMAGE_TYPE(0x47757124), true, 0x47757124, PLAYER::PLAYER_PED_ID(), true, false, 500.0f, PLAYER::PLAYER_PED_ID(), 0);
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY({ BulletCoord.x, BulletCoord.y, BulletCoord.z + 60.0f }, BulletCoord, WEAPON::GET_WEAPON_DAMAGE_TYPE(0xf8a3939f), true, 0xf8a3939f, PLAYER::PLAYER_PED_ID(), true, false, 500.0f, PLAYER::PLAYER_PED_ID(), 0);
				}
			}
		}

		if (m_vars.m_apply_force) {
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			if (isPedShooting(playerPed)) {
				auto BulletCoord = raycast::m_ShapeTestDestination;
				if (!raycast::IsEmpty(BulletCoord)) {
					if (control::request_control(raycast::m_ShapeTestEntity)) {
						Vector3 velocity = ((ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false) + (get_direction(CAM::GET_GAMEPLAY_CAM_ROT(0)) * 9999.f)) - ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false)) * 4.f;
						ENTITY::SET_ENTITY_VELOCITY(raycast::m_ShapeTestEntity, velocity);
					}

				}
			}
		}

		/*castray stand for ray cast mb*/
		if (m_vars.m_delete_entity) {
			Entity entity;
			if (isPedShooting(PLAYER::PLAYER_PED_ID())) {
				Entity AimedAtEntity;
			if (castray(&AimedAtEntity))
				{
						RequestControlOfEnt(AimedAtEntity);
						DeleteEntity(AimedAtEntity);
				}
			}
		}

		if (m_vars.m_spawn_money) {
			if (isPedShooting(PLAYER::PLAYER_PED_ID())) {
				auto bulletCoords = raycast::m_ShapeTestDestination;
				if (!raycast::IsEmpty(bulletCoords)) {
					control::requestModel(0x113fd533);
					OBJECT::CREATE_AMBIENT_PICKUP(0x1e9a99f8, bulletCoords, 1, 2500, 0x113fd533, 1, 1);
				}
			}
		}

		if (m_vars.m_vehicle_hijack) {
			Entity AimedAtEntity;
			if (isPedShooting(PLAYER::PLAYER_PED_ID()))
			{
				if (castray(&AimedAtEntity))
				{
					NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(AimedAtEntity);
					if (ENTITY::IS_ENTITY_A_VEHICLE(AimedAtEntity))
					{
						if (!VEHICLE::IS_VEHICLE_SEAT_FREE(AimedAtEntity, -1, false))
						{
							auto Ped = VEHICLE::GET_PED_IN_VEHICLE_SEAT(AimedAtEntity, -1, false);
							const auto ped = VEHICLE::GET_PED_IN_VEHICLE_SEAT(AimedAtEntity, -1, 0);
							TASK::CLEAR_PED_TASKS_IMMEDIATELY(ped);
						}
						PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), AimedAtEntity, -1);
					}
				}
			}
		}

		if (m_vars.m_clown_vfx) {
			if (isPedShooting(PLAYER::PLAYER_PED_ID())) {
				STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
				GRAPHICS::USE_PARTICLE_FX_ASSET("scr_rcbarry2");
				auto weaponIndexed = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(PLAYER::PLAYER_PED_ID(), 1);

				Vector3 v0, v1;
				MISC::GET_MODEL_DIMENSIONS(WEAPON::GET_SELECTED_PED_WEAPON(PLAYER::PLAYER_PED_ID()), &v0, &v1);
				GRAPHICS::START_NETWORKED_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_clown", weaponIndexed, { (v0.x - v1.x) / 2.0f + 0.7f, 0.f, 0.f }, { 0.f, 180.f, 0.f }, 1.f, 1, 1, 1);
			}
		}

		if (m_vars.m_shrink_entity) {
			Entity aimed;
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &aimed)) {
				if (ENTITY::IS_ENTITY_A_PED(aimed)) {
					PED::SET_PED_CONFIG_FLAG(aimed, 223, true);
				}
			}
		}

		if (m_vars.m_triggerbot) {
			Entity PlayerTarget;
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &PlayerTarget)) {
				if (ENTITY::IS_ENTITY_A_PED(PlayerTarget) && !ENTITY::IS_ENTITY_DEAD(PlayerTarget, 1) && ENTITY::GET_ENTITY_ALPHA(PlayerTarget) == 255) {
					Vector3 Head = PED::GET_PED_BONE_COORDS(PlayerTarget, 0x796E, { 0.1f, 0.0f, 0.0f });
					PED::SET_PED_SHOOTS_AT_COORD(PLAYER::PLAYER_PED_ID(), Head, true);
				}
			}
		}

		if (m_vars.m_toggle_crosshair) {
			switch (m_selected_crosshair) {
			case 0:
				render::draw_sprite({ "srange_gen", "hit_cross" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, color(0, 255, 0, 180), 0.0f);
				break;
			case 1:
				render::draw_sprite({ "helicopterhud", "hud_target" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, color(0, 255, 0, 180), 0.0f);
				break;
			case 2:
				render::draw_sprite({ "helicopterhud", "hudarrow" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, color(0, 255, 0, 180), 0.0f);
				break;
			case 3:
				render::draw_sprite({ "mptattoos3", "tattoo_reach_rank_r_10" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, color(0, 255, 0, 180), 0.0f);
				break;
			case 4:
				render::draw_sprite({ "darts", "dart_reticules" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, color(0, 255, 0, 180), 0.0f);
				break;
			case 5:
				render::draw_sprite({ "darts", "dart_reticules_zoomed" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, color(0, 255, 0, 180), 0.0f);
				break;
			case 6:
				render::draw_sprite({ "shared", "emptydot_32" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, color(0, 255, 0, 180), 0.0f);
				break;
			case 7:
				render::draw_sprite({ "shared", "menuplus_32" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, color(0, 255, 0, 180), 0.0f);
				break;
			case 8:
				render::draw_sprite({ "mp_freemode_mc", "mouse" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, color(0, 255, 0, 180), 0.0f);
				break;
			case 9:
				render::draw_sprite({ "visualflow", "crosshair" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, color(0, 255, 0, 180), 0.0f);
				break;
			case 10:
				render::draw_sprite({ "shared", "newstar_32" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, color(0, 255, 0, 180), 0.0f);
				break;
			}
		}
	}
}
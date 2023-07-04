#include "pch.h"
#include "misc.h"
#include "menu/util/control.h"
#include "gui/util/timer.h"

using namespace base::gui;
using namespace menu::misc::vars;

namespace menu::misc::vars {
	variables m_vars;

	const char* hud_color_lables_names[] = {
		   "Pure White",
		"White",
		"Black",
		"Grey",
		"Grey Light",
		"Grey Dark",
		"Red",
		"Red Light",
		"Red Dark",
		"Blue",
		"Blue Light",
		"Blue Dark",
		"Yellow",
		"Yellow Light",
		"Yellow Dark",
		"Orange",
		"Orange Light",
		"Orange Dark",
		"Green",
		"Green Light",
		"Green Dark",
		"Purple",
		"Purple Light",
		"Purple Dark",
		"Pink",
		"Radar Health",
		"Radar Armour",
		"Radar Damage",
		"Player 1",
		"Player 2",
		"Player 3",
		"Player 4",
		"Player 5",
		"Player 6",
		"Player 7",
		"Player 8",
		"Player 9",
		"Player 10",
		"Player 11",
		"Player 12",
		"Player 13",
		"Player 14",
		"Player 15",
		"Player 16",
		"Player 17",
		"Player 18",
		"Player 19",
		"Player 20",
		"Player 21",
		"Player 22",
		"Player 23",
		"Player 24",
		"Player 25",
		"Player 26",
		"Player 27",
		"Player 28",
		"Player 29",
		"Player 30",
		"Player 31",
		"Player 32",
		"Simple Blip Default",
		"Menu Blue",
		"Menu Grey Light",
		"Menu Blue Extra Dark",
		"Menu Yellow",
		"Menu Yellow Dark",
		"Menu Green",
		"Menu Grey",
		"Menu Grey Dark",
		"Menu Highlight",
		"Menu Standard",
		"Menu Dimmed",
		"Menu Extra Dimmed",
		"Brief Title",
		"Mid Grey MP",
		"Player 1 Dark",
		"Player 2 Dark",
		"Player 3 Dark",
		"Player 4 Dark",
		"Player 5 Dark",
		"Player 6 Dark",
		"Player 7 Dark",
		"Player 8 Dark",
		"Player 9 Dark",
		"Player 10 Dark",
		"Player 11 Dark",
		"Player 12 Dark",
		"Player 13 Dark",
		"Player 14 Dark",
		"Player 15 Dark",
		"Player 16 Dark",
		"Player 17 Dark",
		"Player 18 Dark",
		"Player 19 Dark",
		"Player 20 Dark",
		"Player 21 Dark",
		"Player 22 Dark",
		"Player 23 Dark",
		"Player 24 Dark",
		"Player 25 Dark",
		"Player 26 Dark",
		"Player 27 Dark",
		"Player 28 Dark",
		"Player 29 Dark",
		"Player 30 Dark",
		"Player 31 Dark",
		"Player 32 Dark",
		"Bronze",
		"Silver",
		"Gold",
		"Platinum",
		"Gang 1",
		"Gang 2",
		"Gang 3",
		"Gang 4",
		"Same Crew",
		"Freemode",
		"Pause BG",
		"Friendly",
		"Enemy",
		"Location",
		"Pickup",
		"Pause Singleplayer",
		"Freemode Dark",
		"Inactive Mission",
		"Damage",
		"Pink Light",
		"PM Item Highlight",
		"Script Variable",
		"Yoga",
		"Tennis",
		"Golf",
		"Shooting Range",
		"Flight School",
		"North Blue",
		"Social Club",
		"Platform Blue",
		"Platform Green",
		"Platform Grey",
		"Facebook Blue",
		"In-Game BG",
		"Darts",
		"Waypoint",
		"Michael",
		"Franklin",
		"Trevor",
		"Golf P1",
		"Golf P2",
		"Golf P3",
		"Golf P4",
		"Waypoint Light",
		"Waypoint Dark",
		"panel Light",
		"Michael Dark",
		"Franklin Dark",
		"Trevor Dark",
		"Objective Route",
		"Pause Map Tint",
		"Pause Deselect",
		"PM Weapons Purchasable",
		"PM Weapons Locked",
		"End Screen BG",
		"Chop",
		"Pausemap Tint Half",
		"North Blue Official",
		"Script Variable 2",
		"H",
		"H Dark",
		"T",
		"T Dark",
		"HS Hard",
		"Controller Michael",
		"Controller Franklin",
		"Controller Trevor",
		"Controller Chop",
		"Video Editor Video",
		"Video Editor Audio",
		"Video Editor Text",
		"HB Blue",
		"HB Yellow",
		"Video Editor Score",
		"Video Editor Audio Fadeout",
		"Video Editor Text Fadeout",
		"Video Editor Score Fadeout",
		"Heist Background",
		"Video Editor Ambient",
		"Video Editor Ambient Fadeout",
		"GB",
		"G",
		"B",
		"Low Flow",
		"Low Flow Dark",
		"G1",
		"G2",
		"G3",
		"G4",
		"G5",
		"G6",
		"G7",
		"G8",
		"G9",
		"G10",
		"G11",
		"G12",
		"G13",
		"G14",
		"G15",
		"Adversary",
		"Degen Red",
		"Degen Yellow",
		"Degen Green",
		"Degen Cyan",
		"Degen Blue",
		"Degen Magenta",
		"Stunt 1",
		"Stunt 2",
		"Special Race Series",
		"Special Race Series Dark",
		"CS",
		"CS Dark",
		"Tech Green",
		"Tech Green Dark",
		"Tech Red",
		"Tech Green Very Dark"
	};


	bool isToggled(eDispatchType service) {
		if (patterns::dispatch_table) {
			auto Table = *(uint64_t*)(patterns::dispatch_table + (service * 8));
			if (Table) {
				return *(bool*)(Table + 0x20);
			}
		}

		return false;
	}

	void set_HUD_color() {
		HUD::REPLACE_HUD_COLOUR_WITH_RGBA(m_vars.m_hud, m_vars.hud_r, m_vars.hud_g, m_vars.hud_b, m_vars.hud_a);
	}

	void setToggle(eDispatchType service, bool toggle) {
		if (patterns::dispatch_table) {
			uint64_t table = *(uint64_t*)(patterns::dispatch_table + (service * 8));
			if (table) {
				*(bool*)(table + 0x20) = !toggle;
			}
		}
	}

	Vector3 get_cam_direction(Vector3 rotation) {
		Vector3 tmp;
		tmp.y = rotation.z * 0.0174532924f;
		tmp.x = rotation.x * 0.0174532924f;
		tmp.z = abs(cos(tmp.x));

		return Vector3((-sin(tmp.y)) * tmp.z, (cos(tmp.y)) * tmp.z, sin(tmp.x));
	}
	auto get_all_vehicles() {
		std::vector<Vehicle> result;
		rage::replay::CReplayInterface* CReplayInterface_var = *patterns::replay_interface;
		for (int i = 0; i < 300; i++) {
			auto vehicle_ptr = CReplayInterface_var->m_vehicle_interface->get_vehicle(i);
			if (vehicle_ptr) {
				Vehicle vehicle_handle = patterns::ptr_to_handle(vehicle_ptr);
				result.push_back(vehicle_handle);
			}
		}
		return result;
	};


	int get_closest_train() {
		auto allVehicles = get_all_vehicles();
		for (int i = 0; i < allVehicles.size(); i++) {
			if (ENTITY::GET_ENTITY_MODEL(allVehicles[i]) == 1030400667)
				return allVehicles[i];
		}
		return 0;
	}

	inline void hijack_train() {
		auto train = get_closest_train();
		if (train != 0) {
			control::request_control(train);
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), train, -1);
		}
	}

	void spawn_train() {
		Hash cablecar = rage::joaat("cablecar");
		Hash metrotrain = rage::joaat("metrotrain");
		Hash freight = rage::joaat("freight");
		Hash freightcar = rage::joaat("freightcar");
		Hash freightgrain = rage::joaat("freightgrain");
		Hash freightcont1 = rage::joaat("freightcont1");
		Hash freightcont2 = rage::joaat("freightcont2");
		Hash freighttrailer = rage::joaat("freighttrailer");
		STREAMING::REQUEST_MODEL(cablecar);
		STREAMING::REQUEST_MODEL(metrotrain);
		STREAMING::REQUEST_MODEL(freight);
		STREAMING::REQUEST_MODEL(freightcar);
		STREAMING::REQUEST_MODEL(freightgrain);
		STREAMING::REQUEST_MODEL(freightcont1);
		STREAMING::REQUEST_MODEL(freightcont2);
		STREAMING::REQUEST_MODEL(freighttrailer);
		while (!STREAMING::HAS_MODEL_LOADED(cablecar)) util::fiber::go_to_main();
		while (!STREAMING::HAS_MODEL_LOADED(metrotrain)) util::fiber::go_to_main();
		while (!STREAMING::HAS_MODEL_LOADED(freight)) util::fiber::go_to_main();
		while (!STREAMING::HAS_MODEL_LOADED(freightcar)) util::fiber::go_to_main();
		while (!STREAMING::HAS_MODEL_LOADED(freightgrain)) util::fiber::go_to_main();
		while (!STREAMING::HAS_MODEL_LOADED(freightcont1)) util::fiber::go_to_main();
		while (!STREAMING::HAS_MODEL_LOADED(freightcont2)) util::fiber::go_to_main();
		while (!STREAMING::HAS_MODEL_LOADED(freighttrailer)) util::fiber::go_to_main();
		Vector3 c = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
		*(unsigned short*)patterns::set_this_thread_networked = 0x9090;
		Vehicle train = VEHICLE::CREATE_MISSION_TRAIN(0, c, 1, 1, 1);
		*(unsigned short*)patterns::set_this_thread_networked = 0x0574;
		VEHICLE::SET_VEHICLE_UNDRIVEABLE(train, false);
		TASK::TASK_WARP_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), train, -1);
	}


	inline void delete_train() {
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) && get_closest_train() != 0) {
			VEHICLE::DELETE_ALL_TRAINS();
		}
	}

	inline void exit_train() {
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
			TASK::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
	}

	inline void set_train_speed(float value) {
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
			VEHICLE::SET_TRAIN_CRUISE_SPEED(get_closest_train(), value);
	}

	Vector3 get_direction() {
		Vector3 Cam = CAM::GET_GAMEPLAY_CAM_ROT(0);
		float radiansZ = Cam.z * 0.0174532924f;
		float radiansX = Cam.x * 0.0174532924f;
		float num = abs((float)cos((double)radiansX));
		Vector3 dir;
		dir.x = (float)((double)((float)(-(float)sin((double)radiansZ))) * (double)num);
		dir.y = (float)((double)((float)cos((double)radiansZ)) * (double)num);
		dir.z = (float)sin((double)radiansX);
		return dir;
	}
	static bool active = false;
	inline void set_old_model() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.model = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}

	inline void set_new_model() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.new_model = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}



	inline void HUD_MPREENTER() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.HUD_MPREENTER = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}

	inline void HUD_JOINING() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.HUD_JOINING = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}

	inline void PM_QUIT_MP() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.PM_QUIT_MP = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}

	inline void PM_ENTER_MP() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.PM_ENTER_MP = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}

	inline void PM_EXIT_GAME() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.PM_EXIT_GAME = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}

	inline void PM_GO() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.PM_GO = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}

	inline void PM_FRIEND_FM() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.PM_FRIEND_FM = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}

	inline void PM_FIND_SESS() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			active = true;
			renderer::getRenderer()->reset_keys();
			util::fiber::go_to_main();
		}
		active = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return;
		m_vars.PM_FIND_SESS = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}

}
	
namespace menu {

	void misc_menu::render() {
		renderer::addSubmenu("Miscellaneous", "Miscellaneous", [](core* core) {

			core->addOption(submenuOption("Disables")
				.setTarget("misc_disables"));

			core->addOption(submenuOption("Camera")
				.setTarget("misc_camera"));

			core->addOption(submenuOption("Radio")
				.setTarget("radio"));

			/*core->addOption(submenuOption("Train")
				.setTarget("train"));*/

			core->addOption(submenuOption("HUD")
				.setTarget("hud_color"));

			core->addOption(submenuOption("Dispatch")
				.setTarget("Misc Dispatch"));

			core->addOption(submenuOption("Label Editor")
				.setTarget("label_editing"));

			core->addOption(submenuOption("Model Swapping")
				.setTarget("model_swapping"));

			core->addOption(submenuOption("View Report Stats")
				.setTarget("report_stats"));

			core->addOption(toggleOption("Snow Trails")
				.addToggle(&m_vars.mobile_radio));

			core->addOption(toggleOption("No Orbital Cannon Cooldown")
				.addToggle(&m_vars.no_orbital_cannon_cooldown));

			core->addOption(toggleOption("Rockstar Developer Perks")
				.addToggle(&m_vars.dev_dlc));

			core->addOption(toggleOption("Instant Alt-F4")
				.addToggle(&m_vars.instant_alt_f4));

			core->addOption(buttonOption("Skip Cutscene")
				.addClick([] { CUTSCENE::STOP_CUTSCENE_IMMEDIATELY(); }));

			core->addOption(buttonOption("Network Bail")
				.addClick([] { NETWORK::NETWORK_BAIL(16, 0, 0); }));
		});

		renderer::addSubmenu("View Report Stats", "report_stats", [](core* core) {
			int intexploits, intoffensive, REPORTSTRENGTH, OFFENSIVETAGPLATE, OFFENSIVEUGC,
			EXPLOITS, GRIEFING, COMMENDSTRENGTH, FRIENDLY, HELPFUL, VCANNOYINGME, VCHATE,
			BADCREWNAME, BADCREWMOTTO, BADCREWSTATUS, BADCREWEMBLEM, ISPUNISHED;
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_REPORT_STRENGTH"), &REPORTSTRENGTH, -1);
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_GAME_EXPLOITS"), &intexploits, -1);
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_OFFENSIVE_LANGUAGE"), &intoffensive, -1);
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_OFFENSIVE_TAGPLATE"), &OFFENSIVETAGPLATE, -1);
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_OFFENSIVE_UGC"), &OFFENSIVEUGC, -1);
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_EXPLOITS"), &EXPLOITS, -1);
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_GRIEFING"), &GRIEFING, -1);
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_COMMEND_STRENGTH"), &COMMENDSTRENGTH, -1);
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_FRIENDLY"), &FRIENDLY, -1);
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_HELPFUL"), &HELPFUL, -1);
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_VC_ANNOYINGME"), &VCANNOYINGME, -1);
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_VC_HATE"), &VCHATE, -1);
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_BAD_CREW_NAME"), &BADCREWNAME, -1);
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_BAD_CREW_MOTTO"), &BADCREWMOTTO, -1);
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_BAD_CREW_STATUS"), &BADCREWSTATUS, -1);
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_BAD_CREW_EMBLEM"), &BADCREWEMBLEM, -1);
		STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_ISPUNISHED"), &ISPUNISHED, -1);

		core->addOption(buttonOption("Report Strength")
			.addRightText(std::to_string(REPORTSTRENGTH).c_str()));

		core->addOption(buttonOption("Game Exploits")
			.addRightText(std::to_string(intexploits).c_str()));

		core->addOption(buttonOption("Offensive Language")
			.addRightText(std::to_string(intoffensive).c_str()));

		core->addOption(buttonOption("Offensive Tagplate")
			.addRightText(std::to_string(OFFENSIVETAGPLATE).c_str()));

		core->addOption(buttonOption("Offensive UGC")
			.addRightText(std::to_string(OFFENSIVEUGC).c_str()));

		core->addOption(buttonOption("Exploits")
			.addRightText(std::to_string(EXPLOITS).c_str()));

		core->addOption(buttonOption("Griefing")
			.addRightText(std::to_string(GRIEFING).c_str()));

		core->addOption(buttonOption("Friendly")
			.addRightText(std::to_string(FRIENDLY).c_str()));

		core->addOption(buttonOption("Helpful")
			.addRightText(std::to_string(HELPFUL).c_str()));

		core->addOption(buttonOption("VC Annoying Me")
			.addRightText(std::to_string(VCANNOYINGME).c_str()));

		core->addOption(buttonOption("VC Hate")
			.addRightText(std::to_string(VCHATE).c_str()));

		core->addOption(buttonOption("Bad Crew Name")
			.addRightText(std::to_string(BADCREWNAME).c_str()));

		core->addOption(buttonOption("Bad Crew Motto")
			.addRightText(std::to_string(BADCREWMOTTO).c_str()));

		core->addOption(buttonOption("Bad Crew Status")
			.addRightText(std::to_string(BADCREWSTATUS).c_str()));

		core->addOption(buttonOption("Bad Crew Emblem")
			.addRightText(std::to_string(BADCREWEMBLEM).c_str()));

		core->addOption(buttonOption("Is Punished")
			.addRightText(std::to_string(ISPUNISHED).c_str()));
			});
		renderer::addSubmenu("Label Editor", "label_editing", [](core* core) {
			core->addOption(keyboardOption("HUD_MPREENTER")
				.addClick([] { HUD_MPREENTER(); }));

			core->addOption(keyboardOption("HUD_JOINING")
				.addClick([] { HUD_JOINING(); }));

			core->addOption(keyboardOption("PM_QUIT_MP")
				.addClick([] { PM_QUIT_MP(); }));

			core->addOption(keyboardOption("PM_ENTER_MP")
				.addClick([] { PM_ENTER_MP(); }));

			core->addOption(keyboardOption("PM_EXIT_GAME")
				.addClick([] { PM_EXIT_GAME(); }));

			core->addOption(keyboardOption("PM_GO")
				.addClick([] { PM_GO(); }));

			core->addOption(keyboardOption("PM_FRIEND_FM")
				.addClick([] { PM_FRIEND_FM(); }));

			core->addOption(keyboardOption("PM_FIND_SESS")
				.addClick([] { PM_FIND_SESS(); }));
			});
		renderer::addSubmenu("Radio", "radio", [](core* core) {
			core->addOption(toggleOption("Mobile Radio")
				.addToggle(&m_vars.mobile_radio));

			core->addOption(buttonOption("Skip Track")
				.addClick([] { AUDIO::SKIP_RADIO_FORWARD(); }));
			});
		renderer::addSubmenu("Model Swapping", "model_swapping", [](core* core) {
			core->addOption(keyboardOption("Model to Swap")
				.addClick([] { set_old_model(); }).addRightText(m_vars.model.c_str()));

			core->addOption(keyboardOption("New Model")
				.addClick([] { set_new_model(); }).addRightText(m_vars.new_model.c_str()));
			static size_t selected_index = -1;
			core->addOption(buttonOption("Create Swap")
				.addClick([] {
					std::lock_guard lock(m_vars.m);
					std::string str = m_vars.model;
					transform(str.begin(), str.end(), str.begin(), ::tolower);
					size_t i = 0;
					for (; i < m_vars.models.size(); i++) {
						std::string tmp = m_vars.models[i].first;
						transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
						if (str == tmp) {
							m_vars.models[i].first = m_vars.model;
							m_vars.models[i].second = m_vars.new_model;
							break;
						}
					}
					if (i == m_vars.models.size())
						m_vars.models.push_back(std::make_pair(m_vars.model, m_vars.new_model));
					m_vars.update = true;
					}));

			core->addOption(buttonOption("Delete Swap")
				.addClick([=] {
					std::lock_guard lock(m_vars.m);
					if (!m_vars.models.size() || selected_index < 0
						|| selected_index >= m_vars.models.size())
					{
						return;
					}
					m_vars.models.erase(std::begin(m_vars.models) + selected_index);
					m_vars.models.clear();
					m_vars.update = true;
					}));
			});
		renderer::addSubmenu("Disables", "misc_disables", [](core* core) {

			core->addOption(toggleOption("Disable Idle Kick")
				.addToggle(&m_vars.disable_idle_kick));

			core->addOption(toggleOption("Disable Phone")
				.addToggle(&m_vars.disable_phone));

			core->addOption(toggleOption("Disable Cutscenes")
				.addToggle(&m_vars.disable_cutscene));

			core->addOption(toggleOption("Disable Profanity Filter")
				.addToggle(&m_vars.disable_filter));

			core->addOption(toggleOption("Disable Chat Messages")
				.addToggle(&m_vars.disable_chat_messages));

			core->addOption(toggleOption("Disable Text Messages")
				.addToggle(&m_vars.disable_text_messages));

			core->addOption(toggleOption("Disable Notifications")
				.addToggle(&m_vars.disable_notifications));

			core->addOption(toggleOption("Disable HUD")
				.addToggle(&m_vars.disable_hud));

			core->addOption(toggleOption("Disable Minimap")
				.addToggle(&m_vars.disable_map));

			core->addOption(toggleOption("Disable Map Restrictions")
				.addToggle(&m_vars.disable_restrictions));

			core->addOption(toggleOption("Disable World Limit")
				.addToggle(&m_vars.disable_world_limit));
			});
		renderer::addSubmenu("Camera", "misc_camera", [](core* core) {
			core->addOption(toggleNumberOption<float>("Field of View")
				.addToggle(&m_vars.m_toggle_fov).addNumber(&m_vars.m_fov).addMin(0).addMax(0.05).addStep(0.01).setPrecision(4));

			core->addOption(toggleNumberOption<int>("Original Gta Camera")
				.addToggle(&m_vars.gta_cam).addNumber(&m_vars.gta_cam_distance).addMin(0).addMax(100)
				.addClick([] {
					if (!CAM::DOES_CAM_EXIST(m_vars.m_gta_cam_handle)) {
						m_vars.m_gta_cam_handle = CAM::CREATE_CAMERA(26379945, true);
						CAM::SET_CAM_ACTIVE(m_vars.m_gta_cam_handle, true);
						CAM::RENDER_SCRIPT_CAMS(true, true, 1000, true, false, 0);
					}
					else {
						CAM::DESTROY_CAM(m_vars.m_gta_cam_handle, true);
						CAM::RENDER_SCRIPT_CAMS(false, true, 1000, true, false, 0);
					}
					}));

			core->addOption(toggleNumberOption<float>("Camera Zoom Modifier")
				.addToggle(&m_vars.toggle_zoom).addNumber(&m_vars.cam_zoom).addMin(20).addMax(100).setPrecision(0));

			core->addOption(toggleOption("Free Cam")
				.addToggle(&m_vars.free_cam).addClick([] {
					if (!m_vars.free_cam) {
						if (CAM::DOES_CAM_EXIST(m_vars.m_freecam_handle)) {
							CAM::SET_CAM_ACTIVE(m_vars.m_freecam_handle, false);
							CAM::RENDER_SCRIPT_CAMS(false, true, 1000, true, false, 1);
							CAM::DESTROY_CAM(m_vars.m_freecam_handle, false);
							STREAMING::SET_FOCUS_ENTITY(PLAYER::PLAYER_PED_ID());
						}
					}
					}));

			core->addOption(toggleOption("Anti Crash Cam")
				.addToggle(&m_vars.anti_crash_cam).addClick([] {
					if (!m_vars.anti_crash_cam) {
						if (CAM::DOES_CAM_EXIST(m_vars.m_anti_crash_handle)) {
							CAM::SET_CAM_ACTIVE(m_vars.m_anti_crash_handle, false);
							CAM::RENDER_SCRIPT_CAMS(false, true, 1000, true, false, 1);
							CAM::DESTROY_CAM(m_vars.m_anti_crash_handle, false);
							PLAYER::SET_PLAYER_CONTROL(PLAYER::PLAYER_ID(), true, 0);
						}
					}
					}));
			});

		renderer::addSubmenu("Dispatch", "Misc Dispatch", [](core* core) {
			core->addOption(toggleOption("Disable Police Automobile")
				.addToggle(&m_vars.m_disable_police_automobile)
				.addClick([] { setToggle(DT_PoliceAutomobile, m_vars.m_disable_police_automobile); }));

			core->addOption(toggleOption("Disable Police Helicopter")
				.addToggle(&m_vars.m_disable_police_helicopter)
				.addClick([] { setToggle(DT_PoliceHelicopter, m_vars.m_disable_police_helicopter); }));

			core->addOption(toggleOption("Disable Police Riders")
				.addToggle(&m_vars.m_disable_police_riders)
				.addClick([] { setToggle(DT_PoliceRiders, m_vars.m_disable_police_riders); }));

			core->addOption(toggleOption("Disable Police Vehicle Request")
				.addToggle(&m_vars.m_disable_police_vehicle_request)
				.addClick([] { setToggle(DT_PoliceVehicleRequest, m_vars.m_disable_police_vehicle_request); }));

			core->addOption(toggleOption("Disable Police Road Block")
				.addToggle(&m_vars.m_disable_police_roadblock)
				.addClick([] { setToggle(DT_PoliceRoadBlock, m_vars.m_disable_police_roadblock); }));

			core->addOption(toggleOption("Disable Police Boat")
				.addToggle(&m_vars.m_disable_police_boat)
				.addClick([] { setToggle(DT_PoliceBoat, m_vars.m_disable_police_boat); }));

			core->addOption(toggleOption("Disable Swat Automobile")
				.addToggle(&m_vars.m_disable_swat_automobile)
				.addClick([] { setToggle(DT_SwatAutomobile, m_vars.m_disable_swat_automobile); }));

			core->addOption(toggleOption("Disable Swat Helicopter")
				.addToggle(&m_vars.m_disable_swat_helicopter)
				.addClick([] { setToggle(DT_SwatHelicopter, m_vars.m_disable_swat_helicopter); }));

			core->addOption(toggleOption("Disable Fire Department")
				.addToggle(&m_vars.m_disable_fire_department)
				.addClick([] { setToggle(DT_FireDepartment, m_vars.m_disable_fire_department); }));

			core->addOption(toggleOption("Disable Ambulance Department")
				.addToggle(&m_vars.m_disable_ambulance_department)
				.addClick([] { setToggle(DT_AmbulanceDepartment, m_vars.m_disable_ambulance_department); }));

			core->addOption(toggleOption("Disable Gangs")
				.addToggle(&m_vars.m_disable_gangs)
				.addClick([] { setToggle(DT_Gangs, m_vars.m_disable_gangs); }));

			core->addOption(toggleOption("Disable Army Vehicle")
				.addToggle(&m_vars.m_disable_army_vehicle)
				.addClick([] { setToggle(DT_ArmyVehicle, m_vars.m_disable_army_vehicle); }));

			core->addOption(toggleOption("Disable Bikers")
				.addToggle(&m_vars.m_disable_bikers)
				.addClick([] { setToggle(DT_BikerBackup, m_vars.m_disable_bikers); }));
		});

		renderer::addSubmenu("HUD", "hud_color", [](core* core) {

			HUD::GET_HUD_COLOUR(m_vars.m_hud, &m_vars.hud_r, &m_vars.hud_g, &m_vars.hud_b, &m_vars.hud_a);

			core->addOption(scrollOption<const char*, int>("HUD")
				.addScroll(&hud_color_lables_names).setPosition(&m_vars.m_hud).addClick([] {
					HUD::GET_HUD_COLOUR(m_vars.m_hud, &m_vars.hud_r, &m_vars.hud_g, &m_vars.hud_b, &m_vars.hud_a);
					}));

			core->addOption(numberOption<int>("[R]ed")
				.addNumber(&m_vars.hud_r).addMin(0).addMax(255).addClick([] { set_HUD_color(); }));

			core->addOption(numberOption<int>("[G]reen")
				.addNumber(&m_vars.hud_g).addMin(0).addMax(255).addClick([] { set_HUD_color(); }));

			core->addOption(numberOption<int>("[B]lue")
				.addNumber(&m_vars.hud_b).addMin(0).addMax(255).addClick([] { set_HUD_color(); }));
			});

		renderer::addSubmenu("Train", "train", [](core* core) {
			core->addOption(buttonOption("Spawn Train")
				.addClick([] { spawn_train(); }));

			core->addOption(buttonOption("Find Train")
				.addClick([] { hijack_train(); }));

			static float speed = 0;

			core->addOption(numberOption<float>("Speed")
				.addNumber(&speed).addMin(-500).addMax(500).addStep(1).setPrecision(0).addClick([=] { set_train_speed(speed); }));

			core->addOption(toggleOption("Drive Train")
				.addToggle(&m_vars.drive_train));

			core->addOption(toggleOption("Render as Derailed")
				.addToggle(&m_vars.render_as_derailed));

			core->addOption(buttonOption("Exit Train")
				.addClick([] { exit_train(); }));

			core->addOption(buttonOption("Delete Train")
				.addClick([] { delete_train(); }));
			});
	}

	void misc_menu::update() {
		render();

		int station = AUDIO::GET_PLAYER_RADIO_STATION_INDEX();
		if (m_vars.mobile_radio) {
			caller::call<bool>(patterns::mobile_radio, true);
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
			AUDIO::SET_MOBILE_PHONE_RADIO_STATE(1);
			AUDIO::SET_RADIO_TO_STATION_INDEX(station);
		}
		else {
			caller::call<bool>(patterns::mobile_radio, false);
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
			AUDIO::SET_MOBILE_PHONE_RADIO_STATE(0);
		}

		if (m_vars.disable_cutscene) {
			CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
		}
		if (m_vars.disable_cinematics) {
			CAM::SET_CINEMATIC_BUTTON_ACTIVE(!m_vars.disable_cinematics);
		}
		if (m_vars.disable_map) {
			HUD::SET_MINIMAP_HIDE_FOW(m_vars.disable_map);
		}
		if (m_vars.disable_hud) {
			HUD::HIDE_HUD_AND_RADAR_THIS_FRAME();
		}
		if (m_vars.disable_restrictions) {
			MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("am_armybase");
			MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("restrictedareas");
			MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_armybase");
			MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_lossantosintl");
			MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_prison");
			MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_prisonvanbreak");
			MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("am_doors");
		}
		if (m_vars.disable_world_limit) {
			PLAYER::EXTEND_WORLD_BOUNDARY_FOR_PLAYER({ -9000.0, -11000.0, 30.0 });
			PLAYER::EXTEND_WORLD_BOUNDARY_FOR_PLAYER({ 10000.0, 12000.0, 30.0 });
		}
		if (m_vars.snow_trails) {
			GRAPHICS::USE_SNOW_FOOT_VFX_WHEN_UNSHELTERED(true);
		}
		else {
			GRAPHICS::USE_SNOW_FOOT_VFX_WHEN_UNSHELTERED(false);
		}

		if (m_vars.toggle_zoom) {
			CAM::SET_THIRD_PERSON_CAM_ORBIT_DISTANCE_LIMITS_THIS_UPDATE(0, m_vars.cam_zoom);
		}

		if (m_vars.anti_crash_cam) {
			if (!CAM::DOES_CAM_EXIST(m_vars.m_anti_crash_handle)) {
				m_vars.m_anti_crash_handle = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_CAMERA", { 9999.f, 9999.f, 9999.f }, { 9999.f, 9999.f, 9999.f }, 9999.f, true, 1);
				CAM::RENDER_SCRIPT_CAMS(true, true, 1, false, false, false);
				CAM::SET_CAM_ACTIVE(m_vars.m_anti_crash_handle, true);
				PLAYER::SET_PLAYER_CONTROL(PLAYER::PLAYER_ID(), false, 0);
			}
		}

		if (m_vars.gta_cam) {
			if (CAM::DOES_CAM_EXIST(m_vars.m_gta_cam_handle)) {
				Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), { 0, (float)(m_vars.gta_cam_distance / 2) * -1, (float)m_vars.gta_cam_distance });
				coords.z = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1).z + m_vars.gta_cam_distance;
				CAM::SET_CAM_COORD(m_vars.m_gta_cam_handle,coords);
				CAM::POINT_CAM_AT_ENTITY(m_vars.m_gta_cam_handle, PLAYER::PLAYER_PED_ID(), 0, 0, 0, true);
			}
		}

		if (m_vars.free_cam) {
			if (!CAM::DOES_CAM_EXIST(m_vars.m_freecam_handle)) {
				Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
				m_vars.m_freecam_handle = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 0);
				CAM::SET_CAM_ACTIVE(m_vars.m_freecam_handle, true);
				CAM::RENDER_SCRIPT_CAMS(true, true, 1000, true, false, 1);
				CAM::SET_CAM_COORD(m_vars.m_freecam_handle, { coords.x, coords.y, coords.z + 7.0f });
			}
			else {
				//notify::display_help_text("W = Forward S = Backward A = Left D = Right Move Around With Mouse");
				Vector3 cam_coords = CAM::GET_CAM_COORD(m_vars.m_freecam_handle);
				Vector3 gameplay_cam_rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
				CAM::SET_CAM_ROT(m_vars.m_freecam_handle, { gameplay_cam_rot.x, gameplay_cam_rot.y, gameplay_cam_rot.z, }, 2);

				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false)) {
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), 0.0f);
				}
				else {
					TASK::TASK_STAND_STILL(PLAYER::PLAYER_PED_ID(), 10);
				}

				Vector3 infront = get_direction();

				int left_right = PAD::GET_CONTROL_VALUE(2, 188);
				int up_down = PAD::GET_CONTROL_VALUE(2, 189);

				auto w_down = GetKeyState('W') & 0x8000 || up_down == 0;
				auto s_down = GetKeyState('S') & 0x8000 || up_down == 254;
				auto d_down = GetKeyState('D') & 0x8000 || left_right == 254;
				auto a_down = GetKeyState('A') & 0x8000 || left_right == 0;

				if (w_down) {
					CAM::SET_CAM_COORD(m_vars.m_freecam_handle, { cam_coords.x + infront.x * 2.f, cam_coords.y + infront.y * 2.f, cam_coords.z + infront.z * 2.f });
				}
				else if (s_down) {
					CAM::SET_CAM_COORD(m_vars.m_freecam_handle, { cam_coords.x - infront.x * 2.f, cam_coords.y - infront.y * 2.f, cam_coords.z - infront.z * 2.f });
				}

				if (a_down) {
					Vector3 gameplay_cam_rot_new = gameplay_cam_rot;

					gameplay_cam_rot_new.x = 0.f;
					gameplay_cam_rot_new.y = 0.f;
					gameplay_cam_rot_new.z += 90.f;

					Vector3 new_coords = cam_coords + (get_cam_direction(gameplay_cam_rot_new) * 2.f);
					CAM::SET_CAM_COORD(m_vars.m_freecam_handle, new_coords);
				}

				if (d_down) {
					Vector3 gameplay_cam_rot_new = gameplay_cam_rot;

					gameplay_cam_rot_new.x = 0.f;
					gameplay_cam_rot_new.y = 0.f;
					gameplay_cam_rot_new.z -= 90.f;

					Vector3 new_coords = cam_coords + (get_cam_direction(gameplay_cam_rot_new) * 2.f);
					CAM::SET_CAM_COORD(m_vars.m_freecam_handle, new_coords);
				}

				cam_coords = CAM::GET_CAM_COORD(m_vars.m_freecam_handle);

				static int timer = 0;
				run_timed(&timer, 1000, [&] {
					STREAMING::SET_FOCUS_POS_AND_VEL(cam_coords, { 0, 0, 0 });
				});
			}
		}

		static Cam FieldOfViewCameraHandle = NULL;
		static float defualt_fov = patterns::game_visuals->m_fov;
		if (m_vars.m_toggle_fov) {
			patterns::game_visuals->m_fov = m_vars.m_fov;
			/*if (CAM::DOES_CAM_EXIST(FieldOfViewCameraHandle)) {
				const auto Coords = CAM::GET_GAMEPLAY_CAM_COORD();
				const auto Rotation = CAM::GET_GAMEPLAY_CAM_ROT(2);
				CAM::SET_CAM_COORD(FieldOfViewCameraHandle, Coords.x, Coords.y, Coords.z);
				CAM::SET_CAM_ROT(FieldOfViewCameraHandle, Rotation.x, Rotation.y, Rotation.z, 2);
				CAM::SET_CAM_FOV(FieldOfViewCameraHandle, m_fov);
			}
			else {
				FieldOfViewCameraHandle = CAM::CREATE_CAMERA(26379945, TRUE);
				CAM::SET_CAM_ACTIVE(FieldOfViewCameraHandle, TRUE);
				CAM::RENDER_SCRIPT_CAMS(TRUE, TRUE, 1000, TRUE, FALSE, NULL);
			}*/
		}
		else {
			m_vars.m_fov = defualt_fov;
			/*CAM::DESTROY_CAM(FieldOfViewCameraHandle, TRUE);
			CAM::RENDER_SCRIPT_CAMS(FALSE, TRUE, 1000, TRUE, FALSE, NULL);*/
		}

		/*int train = get_closest_train();

		if (train != 0)
			VEHICLE::SET_RENDER_TRAIN_AS_DERAILED(train, m_vars.render_as_derailed);

		int trainSpeed = ENTITY::GET_ENTITY_SPEED(get_closest_train());
		if (m_vars.drive_train) {
			if (PAD::IS_CONTROL_PRESSED(0, 71))
				trainSpeed++;
			if (PAD::IS_CONTROL_PRESSED(0, 72))
				trainSpeed--;
			set_train_speed(trainSpeed);
		}*/


		bool init = false;
		if (!init) {
			m_vars.m_disable_police_automobile = !isToggled(DT_PoliceAutomobile);
			m_vars.m_disable_police_helicopter = !isToggled(DT_PoliceHelicopter);
			m_vars.m_disable_police_riders = !isToggled(DT_PoliceRiders);
			m_vars.m_disable_police_vehicle_request = !isToggled(DT_PoliceVehicleRequest);
			m_vars.m_disable_police_roadblock = !isToggled(DT_PoliceRoadBlock);
			m_vars.m_disable_police_boat = !isToggled(DT_PoliceBoat);
			m_vars.m_disable_swat_automobile = !isToggled(DT_SwatAutomobile);
			m_vars.m_disable_swat_helicopter = !isToggled(DT_SwatHelicopter);
			m_vars.m_disable_fire_department = !isToggled(DT_FireDepartment);
			m_vars.m_disable_ambulance_department = !isToggled(DT_AmbulanceDepartment);
			m_vars.m_disable_gangs = !isToggled(DT_Gangs);
			m_vars.m_disable_army_vehicle = !isToggled(DT_ArmyVehicle);
			m_vars.m_disable_bikers = !isToggled(DT_BikerBackup);
			init = true;
		}

	}
}
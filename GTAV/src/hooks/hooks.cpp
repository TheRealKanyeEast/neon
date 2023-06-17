#include "pch.h"
#include "hooks.h"
#include "memory/pattern.h"
#include "util/hooking/hooking.h"
#include "patterns.h"
#include "util/caller.h"
#include "gui/util/fonts.h"
#include "rage/classes/enums.h"
#include "gui/util/notify.h"
#include "gui/renderer.h"

#include "rage/classes/scrNativeCallContext.h"
#include "rage/invoker/natives.h"
#include "menu/submenus/main.h"
#include "menu/submenus/main/settings.h"
#include "menu/submenus/main/player.h"
#include "menu/submenus/main/spawner.h"
#include "menu/submenus/main/network.h"
#include "menu/submenus/main/weapon.h"
#include "menu/submenus/main/vehicle.h"
#include "util/fiber.h"
#include "features/features.h"
#include "gui/util/fonts.h"
#include "gui/util/texture.h"
#include "gui/util/panels.h"
#include "util/fiber_pool.h"
#include "gui/renderer.h"
#include "gui/util/notify.h"
#include "menu/util/players.h"
#include "rage/classes/scrThread.h"
#include "menu/submenus/main/weapon/weapon_disables.h"
using namespace memory;

namespace base::hooks {
	bool patterns() {
		batch batch;
		bool out = true;

		batch.Add("GS", "83 3D ? ? ? ? ? 75 17 8B 43 20", [](Ptr ptr) {
			patterns::game_state = ptr.lea().as<decltype(patterns::game_state)>();
		}, out);

		batch.Add("NRT", "48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A", [](Ptr ptr) {
			patterns::native_registration_table = ptr.from_instruction().as<decltype(patterns::native_registration_table)>();
			patterns::get_native_handler = ptr.add(12).rip().as<decltype(patterns::get_native_handler)>();
		}, out);

		batch.Add("JRR", "23 C3 C1 E0 0C", [](Ptr ptr) {
			patterns::jmp_rbx_register = ptr.sub(1).as<decltype(patterns::jmp_rbx_register)>();
		}, out);

		batch.Add("SP", "48 8B 1D ? ? ? ? 41 83 F8 FF", [](Ptr ptr) {
			patterns::script_program = ptr.from_instruction().as<decltype(patterns::script_program)>();
		}, out);

		batch.Add(("FT"), "48 8B 05 ? ? ? ? 48 8B CB 48 69 C9 ? ? ? ? 40 38 AC 01 ? ? ? ? 74 0F E8 ? ? ? ? 0F 28 CE 8B CB E8 ? ? ? ? F3 0F 10 05 ? ? ? ? B2 01 E8", [](Ptr ptr) {
			patterns::font_table = ptr.from_instruction().as<uint64_t>();
		}, out);

		batch.Add(("FMH"), "48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B 0D ? ? ? ? 33 DB 48 85 C9", [](Ptr ptr) {
			patterns::font_memory_helper = ptr.from_instruction().as<uint64_t>();
		}, out);

		batch.Add(("D"), "48 83 EC 28 F0 FF 49 08 75 0F 48 85 C9 74 0A 48 8B 01 BA ? ? ? ? FF 10 48 83 C4 28 C3", [](Ptr ptr) {
			patterns::destruct = ptr.as<uint64_t>();
		}, out);

		batch.Add(("CGF"), "40 53 48 83 EC 20 C7 41 ? ? ? ? ? 48 8D 05 ? ? ? ? 48 8B D9 48 89 01 C7 41 ? ? ? ? ? 48 8D 05 ? ? ? ? 48 89 01", [](Ptr ptr) {
			patterns::construct_gfx_font = ptr.as<uint64_t>();//gfx :thumbs_up:
		}, out);

		batch.Add(("CIFF"), "40 55 48 8B EC 48 81 EC ? ? ? ? 49 8B 00 F3 0F 10 02 F3 0F 10 4A ? 83 65 B8 00 83 65 C0 00 83 65 C4 00 F3", [](Ptr ptr) {
			patterns::create_id_for_font = ptr.as<uint64_t>();
		}, out);

		batch.Add(("AFL"), "40 53 48 83 EC 20 48 8B 41 40 4D 63 C8 4C 8B D9 46 0F B6 14 08 48 8B DA 41 81 E2 ? ? ? ? 41 8B C2 45 8B CA 49 F7 D9 4C 0B", [](Ptr ptr) {
			patterns::add_font_lib = ptr.as<uint64_t>();
		}, out);

		batch.Add(("GFIFT"), "48 85 D2 74 10 49 83 C8 FF 49 FF C0 42 80 3C 02 ? 75 F6 EB 03 45 33 C0 E9", [](Ptr ptr) {
			patterns::get_font_id_table = ptr.as<uint64_t>();
		}, out);

		batch.Add(("SM"), "48 8D 0D ? ? ? ? 03 D3 E8 ? ? ? ? 66 44 39 7D ? 74 09 48 8B 4D E8 E8", [](Ptr ptr) {
			patterns::store_manager = ptr.from_instruction().as<rage::types::store_manager*>();
		}, out);

		batch.Add(("GSME"), "40 53 48 83 EC 20 48 8B C2 48 8B D9 33 D2 48 8B C8 E8 ? ? ? ? 33 D2 44 8B D0 F7 35 ? ? ? ? 44 8B C2 48 8B 15 ? ? ? ? 46 8B 0C 82 41", [](Ptr ptr) {
			patterns::get_store_module_extension = ptr.as<uint64_t>();
		}, out);

		batch.Add("RSF", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 50 48 8B EA 4C 8B FA 48 8B D9 4D 85 C9 B2", [](Ptr ptr) {
			patterns::register_streaming_file = ptr.as<uint64_t>();
		}, out);

		batch.Add("RSF2", "E8 ? ? ? ? 83 4F 48 10", [](Ptr ptr) {
			patterns::request_streaming_file = ptr.call().as<uint64_t>();
		}, out);

		batch.Add("VHL", "4C 03 05 ? ? ? ? EB 09 49 83 C2 04 EB 8B", [](Ptr ptr) {
			patterns::hash_list = ptr.from_instruction().as<decltype(patterns::hash_list)>();
		}, out);

		batch.Add("STTN", "48 8B C8 FF 52 30 84 C0 74 05 48", [](Ptr ptr) {
			patterns::set_this_thread_networked = ptr.add(8).as<decltype(patterns::set_this_thread_networked)>();
		}, out);

		batch.Add("SG", "48 8D 15 ? ? ? ? 4C 8B C0 E8 ? ? ? ? 48 85 FF 48 89 1D", [](Ptr ptr) {
			patterns::script_globals = ptr.from_instruction().as<std::int64_t**>();
		}, out);

		batch.Add("ICF", "48 8D 0D ? ? ? ? 88 05 ? ? ? ? 48 8D 05", [](Ptr ptr) {
			patterns::interval_check_func = ptr.from_instruction().as<PVOID>();
		}, out);

		batch.Add("SD", "E8 ? ? ? ? EB 03 48 8B C3 48 89 47 18", [](Ptr ptr) {
			patterns::setup_dictionary = ptr.call().as<decltype(patterns::setup_dictionary)>();
		}, out);

		batch.Add("ATTD", "E8 ? ? ? ? 84 C0 74 1F 48 8D 4F 30", [](Ptr ptr) {
			patterns::add_texture_to_dictionary = ptr.call().as<decltype(patterns::add_texture_to_dictionary)>();
		}, out);

		batch.Add("SSI", "E8 ? ? ? ? 8B 18 48 8B 45 07", [](Ptr ptr) {
			patterns::setup_store_item = ptr.call().as<decltype(patterns::setup_store_item)>();
		}, out);

		batch.Add("ADTP", "89 54 24 10 48 83 EC 28 48 8B 41 40 4C 63 CA 46 0F B6 14 08 8B 41 4C 41 81 E2 ? ? ? ? 45 8B CA 0F AF C2", [](Ptr ptr) {
			patterns::add_dictionary_to_pool = ptr.as<decltype(patterns::add_dictionary_to_pool)>();
		}, out);

		batch.Add("TS", "48 8D 0D ? ? ? ? E8 ? ? ? ? 8B 45 EC 4C 8D 45 F0 48 8D 55 EC 48 8D 0D ? ? ? ? 89 45 F0 E8", [](Ptr ptr) {
			patterns::texture_store = ptr.from_instruction().as<decltype(patterns::texture_store)>();
		}, out);

		batch.Add("TF", "48 8B 0D ? ? ? ? 45 33 C0 48 8B 01 33 D2 FF 90 ? ? ? ? 48 8B 0D ? ? ? ? 83 64 24", [](Ptr ptr) {
			patterns::texture_factory = ptr.from_instruction().as<decltype(patterns::texture_factory)>();
		}, out);

		batch.Add("IDSC", "48 8B 0D ? ? ? ? 48 8B 01 44 8D 43 01 33 D2 FF 50 40 8B C8", [](Ptr ptr) {
			patterns::swapchain = ptr.from_instruction().as<decltype(patterns::swapchain)>();
		}, out);

		batch.Add("PF", "48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52 8B 81", [](Ptr ptr) {
			patterns::ped_factory = ptr.from_instruction().as<decltype(patterns::ped_factory)>();
		}, out);

		batch.Add("GNGP", "48 83 EC ? 33 C0 38 05 ? ? ? ? 74 ? 83 F9", [](Ptr ptr) {
			patterns::get_net_player = ptr.as<decltype(patterns::get_net_player)>();
		}, out);

		batch.Add("WCHL", "4C 8D 35 ?? ?? ?? ?? 4C 8B F8", [](Ptr ptr) {
			patterns::weapon_components = ptr.from_instruction().as<decltype(patterns::weapon_components)>();
		}, out);

		batch.Add("WIHL", "4C 8D 05 ?? ?? ?? ?? 49 8B 18", [](Ptr ptr) {
			patterns::weapon_info = ptr.from_instruction().as<decltype(patterns::weapon_info)>();
		}, out);

		batch.Add("HTP", "83 F9 FF 74 31 4C 8B 0D", [](Ptr ptr) {
			patterns::handle_to_ptr = ptr.as<decltype(patterns::handle_to_ptr)>();
		}, out);

		batch.Add("RNC", "E8 ? ? ? ? EB 3E 48 8B D3", [](Ptr ptr) {
			patterns::request_control = ptr.call().as<decltype(patterns::request_control)>();
		}, out);

		batch.Add("IST", "40 38 35 ? ? ? ? 75 0E 4C 8B C3 49 8B D7 49 8B CE", [](Ptr ptr) {
			patterns::is_session_started = ptr.from_instruction().as<decltype(patterns::is_session_started)>();
		}, out);

		batch.Add("FV", "83 79 18 00 48 8B D1 74 4A FF 4A 18 48 63 4A 18 48 8D 41 04 48 8B 4C CA", [](Ptr ptr) {
			patterns::fix_vectors = ptr.as<decltype(patterns::fix_vectors)>();
		}, out);

		batch.Add("PEP", "4C 8B 35 ? ? ? ? B8 ? ? ? ? 0F 57 F6 89 05 ? ? ? ? 49 63 76 10 4C 8B FE 85 F6 0F 84 ? ? ? ? 49 8B 46 08 49 FF CF FF CE 42 0F B6 0C 38", [](Ptr ptr) {
			patterns::ped_pool = ptr.from_instruction().as<decltype(patterns::ped_pool)>();
		}, out);

		batch.Add("PRP", "48 8B 0D ? ? ? ? 49 8B D0 E8 ? ? ? ? 39 03 EB 19 41 80 78 ? ? 75 15 48 8B 0D ? ? ? ? 49 8B D0 E8 ? ? ? ? 39 43 04", [](Ptr ptr) {
			patterns::prop_pool = ptr.from_instruction().as<decltype(patterns::prop_pool)>();
		}, out);

		batch.Add("VEP", "4C 8B 25 ? ? ? ? 8B 29 33 F6 49 8B 04 24 33 DB 4C 8D 71 08 44 8B 78 08 45 85 FF 0F 8E ? ? ? ? 4D 8B 0C 24 41 3B 59 08 7D 29 49 8B 51 30 44 8B C3 8B CB 49 C1 E8 05 83 E1 1F 44 8B D3 42 8B 04 82", [](Ptr ptr) {
			patterns::vehicle_pool = ptr.from_instruction().as<decltype(patterns::vehicle_pool)>();
		}, out);

		batch.Add("PTH", "48 8B F9 48 83 C1 10 33 DB", [](Ptr ptr) {
			patterns::ptr_to_handle = ptr.sub(0x15).as<decltype(patterns::ptr_to_handle)>();
		}, out);

		batch.Add("SR", "66 0F 6E 0D ? ? ? ? 0F B7 3D", [](Ptr ptr) {
			patterns::resolution_x = ptr.add(4).rip().as<decltype(patterns::resolution_x)>();
			patterns::resolution_y = ptr.add(4).rip().as<decltype(patterns::resolution_y)>();
		}, out);

		batch.Add("GEA", "E8 ? ? ? ? 48 8B D8 48 85 C0 0F 84 ? ? ? ? 48 8B 0D ? ? ? ? 8B D7 E8 ? ? ? ? 48 8B F8 48 85 C0", [](Ptr ptr) {
			patterns::get_entity_address = ptr.call().as<uint64_t>();
		}, out);

		batch.Add("SVG", "8B 91 ? ? ? ? F3 0F 10 05 ? ? ? ? 8D 42 FD A9", [](Ptr ptr) {
			patterns::set_vehicle_gravity = ptr.as<decltype(patterns::set_vehicle_gravity)>();
		}, out);

		batch.Add("WVFX", "48 8D 05 ? ? ? ? 48 6B FF 45 F3 0F 59 0D ? ? ? ? F3 41 0F 59 9E ? ? ? ? F3 0F 10 BD ? ? ? ? 48 03 FE 48 69 FF ? ? ? ? F3", [](Ptr ptr) {
			patterns::vfx_wheel.first = ptr.from_instruction().as<decltype(patterns::vfx_wheel.first)>();
			patterns::vfx_wheel.second = ptr.add(0xA).as<decltype(*(uint8_t*)patterns::vfx_wheel.second)>();
		}, out);

		batch.Add("BL", "4C 8D 05 ? ? ? ? 0F B7 C1", [](Ptr ptr) {
			patterns::blip_list = ptr.from_instruction().as<decltype(patterns::blip_list)>();
		}, out);

		batch.Add("GT", "45 33 F6 8B E9 85 C9 B8", [](Ptr ptr) {
			patterns::script_threads = ptr.sub(4).rip().sub(8).as<decltype(patterns::script_threads)>();
		}, out);

		batch.Add("NPM", "48 8B 0D ? ? ? ? 8A D3 48 8B 01 FF 50 ? 4C 8B 07 48 8B CF", [](Ptr ptr) {
			patterns::network_player_mgr = ptr.from_instruction().as<decltype(patterns::network_player_mgr)>();
		}, out);

		batch.Add("SGSBGH", "E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 8B 05 ? ? ? ? 48 8D 4C 24", [](Ptr ptr) {
			patterns::start_get_session_by_gamer_handle = ptr.call().as<decltype(patterns::start_get_session_by_gamer_handle)>();
		}, out);

		batch.Add("N", "48 8B 0D ? ? ? ? 8A D3 48 8B 01 FF 50 ? 4C 8B 07 48 8B CF", [](Ptr ptr) {
			patterns::network = ptr.from_instruction().as<decltype(patterns::network)>();
		}, out);

		batch.Add("ESI", "E8 ? ? ? ? C6 83 94 01 00 00 01", [](Ptr ptr) {
			patterns::encode_session_info = ptr.call().as<decltype(patterns::encode_session_info)>();
		}, out);

		LOG_SUCCESS("Loaded patterns");

		auto mod = memory::module("GTA5.exe");	
		batch.run(mod);

		return true;
	}

	bool bypass() {
		batch batch;
		bool out = true;


		batch.Add("IRC", "80 3D ?? ?? ?? ?? ?? 0F 85 A7 00 00 00 48 8B 05", [](Ptr ptr) {
			patterns::invalid_resource_check = ptr.as<uint64_t>();
			memory::patchAddress(patterns::invalid_resource_check, 0xC3);
		}, out);

		batch.Add("ICP-1", "48 83 EC 48 48 83 64 24 ? ? 83 4C 24", [](Ptr ptr) {
			patterns::integ_check_1 = ptr.as<uint64_t>();
			memory::patchAddress(patterns::integ_check_1, 0xC3);
		}, out);

		batch.Add("C-CRC", "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 83 EC 30 44 8B F1 48 8B 0D", [](Ptr ptr) {
			patterns::check_crc = ptr.add(0x64).as<uint64_t>();
		}, out);

		batch.Add("C-RMS", "40 53 48 83 EC 40 83 B9 ? ? ? ? ? 48 8B D9 75 67 48 83 64 24 ? ? 48 83 64 24 ? ? 48 8D 54 24 ? 48 81 C1", [](Ptr ptr) {
			patterns::report_myself = ptr.add(0x5E).call().add(0x22).as<uint64_t>();
		}, out);

		batch.Add("C-CS", "48 89 5C 24 08 57 48 83 EC 20 33 FF 48 8B D9 48 85 C9 74 0B", [](Ptr ptr) {
			patterns::cash_spawn = ptr.sub(0x3F).as<uint64_t>();
		}, out);

		batch.Add("QD", "48 89 5C 24 ? 57 48 83 EC ? 0F B6 99", [](Ptr ptr) {
			patterns::queue_dependency = ptr.as<uint64_t>();
			return hooking::detour("QD", patterns::queue_dependency, &queue_dependency, &og_queue_dependency);
		}, out);

		batch.Add("SNE", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 54 41 55 41 56 41 57 48 83 EC 30 48 8D 71 28", [](Ptr ptr) {
			patterns::send_network_event = ptr.as<uint64_t>();
			return hooking::detour("SNE", patterns::send_network_event, &send_network_event, &og_send_network_event);
		}, out);

		batch.Add("AETL", "4C 8B D1 48 63 49 18", [](Ptr ptr) {
			patterns::add_event_to_list = ptr.as<uint64_t>();
			return hooking::detour("AETL", patterns::add_event_to_list, &add_event_to_list, &og_add_event_to_list);
		}, out);

		batch.Add("SHTTPR", "40 55 57 41 54 41 55", [](Ptr ptr) {
			patterns::send_http_request = ptr.as<uint64_t>();
			return hooking::detour("SHTTPR", patterns::send_http_request, &send_http_request, &og_send_http_request);
			}, out);

		batch.Add("NTQVMC", "66 0F 6F 0D ? ? ? ? 66 0F 6F 05 ? ? ? ? 66 0F 66 C4", [](Ptr ptr) {
			byte_patch::make(ptr.add(4).rip().sub(32).as<uint64_t*>(), (uint64_t)&hooks::nt_query_virtual_memory)->apply();
		}, out);

		batch.Add("RCOEP", "48 89 5C 24 ? 57 48 83 EC 20 8B D9 E8 ? ? ? ? ? ? ? ? 8B CB", [](Ptr ptr) {
			byte_patch::make(ptr.add(0x13).as<std::uint16_t*>(), 0x9090)->apply();
		}, out);

		batch.Add("CW", "74 44 E8 ? ? ? ? 80 65 2B F8 48 8D 0D ? ? ? ? 48 89 4D 17 48 89 7D 1F 89 7D 27 C7 45", [](Ptr ptr) {
			byte_patch::make(ptr.as<uint8_t*>(), 0xEB)->apply();
		}, out);

		batch.Add("MWL", "8B 43 6C 89 05", [](Ptr ptr) {
			patterns::max_wanted_level = byte_patch::make(ptr.add(5).rip().as<uint32_t*>(), 0).get();
			patterns::max_wanted_level_2 = byte_patch::make(ptr.add(14).rip().as<uint32_t*>(), 0).get();
		}, out);

		LOG_SUCCESS("Loaded bypasses");

		auto mod = memory::module("GTA5.exe");
		batch.run(mod);

		return true;
	}
	
	bool hooks() {
		batch batch;
		bool out = false;

		batch.Add("SVM", "E8 ? ? ? ? 48 85 FF 48 89 1D", [&](Ptr ptr) {
			patterns::script_vm = ptr.call().as<uint64_t>();
			return hooking::detour("SVM", patterns::script_vm, &script_vm, &og_script_vm);
		}, out);

		batch.Add("GFI", "85 D2 74 68 FF CA 74 5B FF CA 74 4E FF CA 74 41 FF CA 74 34 FF CA 74 27 FF CA 74 1A FF CA 74 0D", [](Ptr ptr) {
			patterns::get_font_id = ptr.as<uint64_t>();
			return hooking::detour("GFI", patterns::get_font_id, &get_font_id, &og_get_font_id);
		}, out);

		batch.Add("FGS", "E8 ? ? ? ? 8B 44 24 78 4C 8B 4C 24 ? 44 8B C7", [](Ptr ptr) {
			patterns::format_game_string = ptr.call().as<uint64_t>();
			return hooking::detour("FGS", patterns::format_game_string, &format_engine_string, &og_format_engine_string);
		}, out);

		batch.Add("GGSLC", "E8 ? ? ? ? 8B F8 48 8B 55 B8", [](Ptr ptr) {
			patterns::get_game_string_line_count = ptr.call().as<uint64_t>();
			return hooking::detour("GGSLC", patterns::get_game_string_line_count, &get_engine_string_line_count, &og_get_engine_string_line_count);
		}, out);

		batch.Add("AWIV", "49 8B 40 08 39 10", [](Ptr ptr) {
			patterns::allow_weapons_in_vehicle = ptr.sub(23).as<uint64_t>();
			return hooking::detour("AWIV", patterns::allow_weapons_in_vehicle, &allow_weapons_in_vehicle, &og_allow_weapons_in_vehicle);
		}, out);

		auto mod = memory::module("GTA5.exe");
		batch.run(mod);

		LOG_SUCCESS("Loaded hooks");

		return true;
	}

	bool allow_weapons_in_vehicle(int64_t unk, int weaponinfo_group) {
		if (menu::weapon::disables::vars::m_vars.m_vehicle_block) {
			if (weaponinfo_group == RAGE_JOAAT("GROUP_THROWN"))
				return og_allow_weapons_in_vehicle(unk, weaponinfo_group);
			return true;
		}
		return og_allow_weapons_in_vehicle(unk, weaponinfo_group);
	}

	uint64_t format_engine_string(uint64_t rcx, uint64_t rdx, uint32_t r8d, const char* r9, uint32_t stack) {
		if (r9) {
			if (strlen(r9) >= 98) {
				for (int i = 0; i < 100; i++) {
					if (strstr(menu::renderer::getRenderer()->render_queue[i].c_str(), r9)) {
						return og_format_engine_string(rcx, rdx, r8d, menu::renderer::getRenderer()->render_queue[i].c_str(), stack);
					}
				}

				if (!menu::renderer::getRenderer()->tooltip.empty()) {
					if (strstr(menu::renderer::getRenderer()->tooltip.c_str(), r9)) {
						return og_format_engine_string(rcx, rdx, r8d, menu::renderer::getRenderer()->tooltip.c_str(), stack);
					}
				}

				for (int i = 0; i < 100; i++) {
					std::vector<menu::notify::notify_context>& contexts = menu::notify::get_notify()->get_contexts();
					if (!contexts.empty()) {
						for (menu::notify::notify_context& text : contexts) {
							if (text.m_text.size() > 1) {
								for (std::string line : text.m_text) {
									if (strstr(line.c_str(), r9)) {
										return og_format_engine_string(rcx, rdx, r8d, line.c_str(), stack);
									}
								}
							}
							else {
								if (strstr(text.m_text[0].c_str(), r9)) {
									return og_format_engine_string(rcx, rdx, r8d, text.m_text[0].c_str(), stack);
								}
							}
						}
					}
				}
			}
		}

		return og_format_engine_string(rcx, rdx, r8d, r9, stack);
	}

	int get_engine_string_line_count(uint64_t rcx, const char* rdx, float xmm2, uint64_t r9, bool stack) {
		if (rdx) {
			if (strlen(rdx) >= 98) {
				if (!menu::renderer::getRenderer()->tooltip.empty()) {
					if (strstr(menu::renderer::getRenderer()->tooltip.c_str(), rdx)) {
						return og_get_engine_string_line_count(rcx, menu::renderer::getRenderer()->tooltip.c_str(), xmm2, r9, stack);
					}
				}
				std::vector<menu::notify::notify_context>& contexts = menu::notify::get_notify()->get_contexts();
				if (!contexts.empty()) {
					for (menu::notify::notify_context& text : contexts) {
						if (text.m_text.size() > 1) {
							for (std::string line : text.m_text) {
								if (strstr(line.c_str(), rdx)) {
									return og_get_engine_string_line_count(rcx, line.c_str(), xmm2, r9, stack);
								}
							}
						}
						else {
							if (strstr(text.m_text[0].c_str(), rdx)) {
								return og_get_engine_string_line_count(rcx, text.m_text[0].c_str(), xmm2, r9, stack);
							}
						}
					}
				}
			}
		}
		return og_get_engine_string_line_count(rcx, rdx, xmm2, r9, stack);
	}

	uint64_t script_vm(void* stack, int64_t** globals, rage::scrProgram* program, rage::scrThreadContext* ctx) {
		return og_script_vm(stack, globals, program, ctx);
	}

	void* get_font_id(uint64_t font, int id) {
		std::vector<fonts::font_context> fonts = fonts::get_list();
		for (fonts::font_context font_ : fonts) {
			if (font_.m_font_id == id) {
				return caller::call<void*>(patterns::get_font_id_table, font, font_.m_font_name.c_str());
			}
		}
		return og_get_font_id(font, id);
	}

}
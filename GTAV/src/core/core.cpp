#include "pch.h"
#include "core.h"
#include "util/thread_pool.h"
#include "util/Log.h"
#include "hooks/hooks.h"
#include "util/hooking/hooking.h"
#include "features/manager/manager.h"
#include "hooks/patterns.h"
#include "rage/invoker/invoker.h"
#include "exception.h"
#include "rage/invoker/natives.h"
#include "util/hooking/script.h"
#include "util/fiber.h"
#include "util/dirs.h"
#include "gui/d3d.h"
#include "gui/util/translate.h"
#include "gui/util/notify.h"
#include "memory/pattern.h"
#include "gui/util/translate.h"
#include "rage/api/api.h"
#include "menu/util/script_patcher_service.h"
#include "features/features.h"
#include "security/security.h"
#include "auth/auth.h"
#include "auth/vars.h"
#include "security/themdia/secure_engine.h"
#include "util/util.h"
namespace base::core {


	DWORD WINAPI unload(LPVOID handle) {
		/*hooking::cleanup();
		util::threads::getThreadPool()->Cleanup();
		util::fiber::cleanup();
		features::g_manager.clear();
		exceptions::uninitExceptionHandler();
		util::log::Cleanup();

		FreeLibraryAndExitThread(static_cast<HMODULE>(handle), 0);*/
		return DWORD();
	}

	void download_files() {
		std::string MenuFolderPath = Util::GetDocumentsPath() + "\\aether\\textures\\";
		auth::download(XOR("https://media.discordapp.net/attachments/1106198450216902706/1125085459366813736/color.png"), MenuFolderPath + "color.png");
		auth::download(XOR("https://media.discordapp.net/attachments/1106198450216902706/1125085459719139390/dots.png"), MenuFolderPath + "dots.png");
		auth::download(XOR("https://media.discordapp.net/attachments/1106198450216902706/1125085459987562596/footer.png"), MenuFolderPath + "footer.png");
		auth::download(XOR("https://media.discordapp.net/attachments/1106198450216902706/1125085460356669580/header.png"), MenuFolderPath + "header.png");
		auth::download(XOR("https://media.discordapp.net/attachments/1106198450216902706/1125085460604137492/keyboard.png"), MenuFolderPath + "keyboard.png");
		auth::download(XOR("https://media.discordapp.net/attachments/1106198450216902706/1125085460876755015/scroller.png"), MenuFolderPath + "scroller.png");
		auth::download(XOR("https://media.discordapp.net/attachments/1106198450216902706/1125085461145198602/toggle_off.png"), MenuFolderPath + "toggle_off.png");
		auth::download(XOR("https://media.discordapp.net/attachments/1106198450216902706/1125085461434597496/toggle_on.png"), MenuFolderPath + "toggle_on.png");

		std::string font_folder = Util::GetDocumentsPath() + "\\aether\\fonts\\";
		auth::download(XOR("https://cdn.discordapp.com/attachments/1106198450216902706/1125085424893829170/RDR.gfx"), font_folder + "RDR.gfx");
		auth::download(XOR("https://cdn.discordapp.com/attachments/1106198450216902706/1125085429071351908/Roboto.gfx"), font_folder + "Roboto.gfx");
	}


	DWORD load(LPVOID handle) {

		//exit(0);
		//VM_DOLPHIN_BLACK_START
		//MUTATE_START

		util::log::Load();
		exceptions::initExceptionHandler();

		if (!util::dirs::load()) {
			//unload(handle);
		}

		static bool kaka = true;

		/*while (kaka == true) {

			if (GetAsyncKeyState(VK_DELETE) & 0x8000) {
				kaka = false;
				LOG_SUCCESS(XOR("skofdweidwdjwdfis"));
			}

			LOG_SUCCESS(XOR("Ababdba"));

			Sleep(1);
		}*/

		auth::login();
		if (auth::vars::g_type == XOR("NULL")) {
			LOG_ERROR(XOR("Failed to validate account"));
			auth::vars::g_logged_in = false;
			std::this_thread::sleep_for(1500ms);
			exit(0);
		}
		if (auth::vars::g_logged_in) {
			std::thread hb_t(auth::run_heartbeat);
			download_files();

			LOG(XOR("Welcome to Aether - Grand Theft Auto V"));

			if (!(g_window = FindWindowA(("grcWindow"), NULL))) {
				int timeout = 0;
				while (!g_window) {
					if (timeout >= 20) {
						LOG_ERROR(XOR("Failed to find game window"));
						unload(handle);
						return 0;
					}

					g_window = FindWindowA(XOR("grcWindow"), NULL);
					timeout++;
					Sleep(1000);
				}
			}

			//base::gui::getTranslationManager()->init();

		//	auto script_patcher_service_instance = std::make_unique<menu::script_patcher_service>();

			if (!base::hooks::patterns()) {
				LOG_WARN(XOR("Failed to load patterns, unloading..."));
				std::this_thread::sleep_for(6s);
				unload(handle);
			}

			if (!base::hooks::bypass()) {
				LOG_WARN(XOR("Failed to load bypasses, unloading..."));
				std::this_thread::sleep_for(6s);
				unload(handle);
			}


			while (*patterns::game_state != 0) {
				std::this_thread::sleep_for(3s);
				std::this_thread::yield();
			}

			if (!invoker::invoker::handlers_cached()) {
				invoker::invoker::cache_handlers();
			}

			d3d::initialize();

			if (!base::hooks::hooks()) {
				LOG_WARN(XOR("Failed to load hooks, unloading..."));
				std::this_thread::sleep_for(6s);
				unload(handle);
			}

			auto script_hooks = std::make_unique<script::native_hooks>();

			//std::thread sc_t(rage::api::update);

			LOG_SUCCESS(XOR("Loaded in-game patterns"));
			LOG_SUCCESS(XOR("Loaded in-game hooks"));

			g_running = true;


			///int ogHeapSize = *patterns::heap_size / 1024 / 1024;
			//LOG(std::format("Original Memory Heap Size: {}MB", ogHeapSize).c_str());
			//LOG("Modifying Memory Heap Size...");
			//DWORD oldProtect;
			//VirtualProtect(patterns::heap_size, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
			//*patterns::heap_size = 650 * 1024 * 1024;
		//	VirtualProtect(patterns::heap_size, 4, oldProtect, &oldProtect);
		//	LOG_WARN(std::format("Modified Memory Heap Size To: {}MB", *patterns::heap_size / 1024 / 1024).c_str());
			//translation::get()->enable();
			translation::get()->load();

			while (g_running) {
				//security::run();

				/*if (!auth::vars::g_logged_in) {
					Sleep(5000);
					exit(0);
				}*/

				if (GetAsyncKeyState(VK_END) & 0x8000) {
					g_running = false;
				}
				Sleep(1);
			}

			//script_patcher_service_instance.reset();
			script_hooks.reset();
			unload(handle);
		}
		else {
			LOG_ERROR(XOR("Unable to login"));
			auth::vars::g_logged_in = false;
		}
		//MUTATE_END
		//VM_DOLPHIN_BLACK_END
	}
}
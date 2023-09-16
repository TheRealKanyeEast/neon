#include "pch.h"
#include "auth.h"
#include "crypto/crypto.h"
#include "socket/socket.h"
#include "util/dirs.h"
#include "util/log.h"
#include "util/fiber.h"
#include "gui/util/timer.h"
#include <algorithm>
#include "../../includes/openssl/evp.h"
#include "../../includes/openssl/rand.h"
#include "../../includes/openssl/aes.h"
#include "security/xor.h"
#include "security/themdia/secure_engine.h"
#include "util/json.h"
#include "vars.h"
const int IV_LENGTH = 16;
void generate_iv(unsigned char* iv) {
	if (!RAND_bytes(iv, IV_LENGTH)) {
		exit(0);
	}
}


namespace auth {
	void auth::login() {

		/*std::string pub_key = XOR("iv/jewJe=7PLgT%r)46S28{MeGV4kyTw");
		unsigned char* key = reinterpret_cast<unsigned char*>(const_cast<char*>(pub_key.data()));
		unsigned char iv[IV_LENGTH];
		generate_iv(iv);
		std::string iv_string(iv, iv + IV_LENGTH);

		std::string url = XOR("https://api.aether.lol/util/auth-action.php");
		std::string auth_key = XOR("QQ7FsFLAjh5WsckUxuA1wkk5OgJH2RBOgwlwodrxBCqkKoXVOL+oBxfOe1mj25Fn");
		std::string pattern = XOR("SVM");
		std::string hwid = auth::hardware::get_hwid();

		std::ifstream input(util::dirs::get_path(auth));
		if (input.good()) {
			nlohmann::json json;
			input >> json;
			input.close();

			auth::vars::g_username = json["Auth"]["Username"].get<std::string>();
			auth::vars::g_password = json["Auth"]["Password"].get<std::string>();
		}

		auth::vars::g_username = auth::crypto::aes256cbc_encrypt(auth::vars::g_username, key, iv);
		auth::vars::g_password = auth::crypto::aes256cbc_encrypt(auth::vars::g_password, key, iv);
		auth_key = auth::crypto::aes256cbc_encrypt(auth_key, key, iv);
		pattern = auth::crypto::aes256cbc_encrypt(pattern, key, iv);
		hwid = auth::crypto::aes256cbc_encrypt(hwid, key, iv);
		std::string result = auth::socket::send_request(url, auth::vars::g_username, auth::vars::g_password, auth_key, pattern, iv_string, hwid);

		std::string decoded_result = auth::crypto::aes256cbc_decrypt(result, key, iv);*/

		auth::vars::g_type = XOR("Admin");
		//LOG("ADMIN");
		auth::vars::g_logged_in = true;
		/*else {
			auth::vars::g_logged_in = false;
			LOG_ERROR(XOR("%s"), decoded_result);
			Sleep(4500);
			exit(0);
		}*/
	}

	/*std::string auth::get_pattern(std::string name) {
		std::string url = XOR("https://api.aether.lol/util/retrieve-patterns.php");
		std::string auth_key = XOR("QQ7FsFLAjh5WsckUxuA1wkk5OgJH2RBOgwlwodrxBCqkKoXVOL+oBxfOe1mj25Fn");
		std::string pattern = name;
		std::string hwid = auth::hardware::get_hwid();

		std::string pub_key = XOR("iv/jewJe=7PLgT%r)46S28{MeGV4kyTw");
		unsigned char* key = reinterpret_cast<unsigned char*>(const_cast<char*>(pub_key.data()));

		std::copy(pub_key.begin(), pub_key.end(), key);
		unsigned char iv[IV_LENGTH];
		generate_iv(iv);
		std::string iv_string(iv, iv + IV_LENGTH);


		std::ifstream input(util::dirs::get_path(auth));
		if (input.good()) {
			nlohmann::json json;
			input >> json;
			input.close();

			auth::vars::g_username = json["Auth"]["Username"].get<std::string>();
			auth::vars::g_password = json["Auth"]["Password"].get<std::string>();
		}

		auth::vars::g_username = auth::crypto::aes256cbc_encrypt(auth::vars::g_username, key, iv);
		auth::vars::g_password = auth::crypto::aes256cbc_encrypt(auth::vars::g_password, key, iv);

		auth_key = crypto::aes256cbc_encrypt(auth_key, key, iv);
		pattern = crypto::aes256cbc_encrypt(pattern, key, iv);
		hwid = crypto::aes256cbc_encrypt(hwid, key, iv);
		std::string result = socket::send_request(url, auth::vars::g_username, auth::vars::g_password, auth_key, pattern, iv_string, hwid);
		std::string decoded_result = crypto::aes256cbc_decrypt(result, key, iv);

		if (decoded_result.empty()) {
			aether::g_logger.log_error(XOR("Err"), XOR("Failed to scan pattern %s"), name.c_str());
			std::this_thread::sleep_for(2500ms);
			exit(0);
		}
		if (decoded_result == XOR("Invalid username or password")) {
			aether::g_logger.log_error(XOR("Err"), XOR("Invalid username or password"));
		}

		return decoded_result.c_str();
	}*/

	bool auth::run_heartbeat() {
		if (vars::g_type == XOR("Admin"))
			LOG(XOR("Heartbeat init"));

		Timer timer(std::chrono::minutes(3));
		while (true) {
			if (timer.Update()) {
				login();
				/*if (vars::g_type == XOR("Admin"))
					LOG_SUCCESS(XOR("Heartbeat Passed"));*/
			}
		}
		return true;
	}
	typedef size_t(*Write)(char*, size_t, size_t, std::string*);
	inline static std::size_t WriteData(void* ptr, std::size_t size, std::size_t nmemb, void* stream) {
		auto written = fwrite(ptr, size, nmemb, (FILE*)stream);
		return written;
	}
	void auth::download(std::string url, std::string path) {
		//MUTATE_START
			CURL* curl;
		FILE* fp;
		CURLcode res;
		curl = curl_easy_init();
		if (curl) {
			fp = fopen(path.c_str(), XOR("wb"));
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
			res = curl_easy_perform(curl);

			curl_easy_cleanup(curl);
			fclose(fp);
		}
		//MUTATE_END
	}
}
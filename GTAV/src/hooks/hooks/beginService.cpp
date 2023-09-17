#include "pch.h"
#include "hooks/hooks.h"

namespace base::hooks {
	bool hooks::beginService(rage::netShopping::CNetworkShoppingMgr* netShopMgr,
		int* transaction_id,
		unsigned int unk,
		rage::joaat_t category,
		rage::joaat_t item_hash,
		rage::joaat_t action_type_hash,
		int value,
		int flag)
	{
		const auto returnValue = ogBeginService(netShopMgr, transaction_id, unk, category, item_hash, action_type_hash, value, flag);

		if (returnValue == true)
		{
			LOG("Transaction Id: %i\nUNK: %u\nCategory: %lu\nItem Hash: %lu\nAction Type Hash: %lu\nValue: %i\nFlag: %i",
				*transaction_id,
				unk,
				category,
				item_hash,
				action_type_hash,
				value,
				flag
			);
		}

		return returnValue;
	}
}
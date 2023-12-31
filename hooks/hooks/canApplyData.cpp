#include "pch.h"
#include "hooks/hooks.h"
#include "hooks/patterns.h"
#include "util/caller.h"
#include "rage/engine.h"
#include "rage/classes/NetObject.h"
#include "gui/util/notify.h"
#include "menu/util/helpers.h"

namespace base::hooks {
	bool hooks::canApplyDataHook(rage::netSyncTree* tree, rage::netObject* object) {

		static bool init = ([] { menu::helpers::cacheNodes(); }(), true);
	
		if (tree->m_child_node_count && tree->m_next_sync_node && menu::helpers::handleNodes(tree->m_next_sync_node, patterns::g_syncing_player, object)) {
			return false;
		}

		return ogCanApplyDataHook(tree, object);
	}
}
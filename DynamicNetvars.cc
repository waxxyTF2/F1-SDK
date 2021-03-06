#include "baseHeaders.hh"

#include "DynamicNetvars.hh"

#include "../SDK/SDK.hh"

#undef GetProp

netvar_tree gNetvars;

void netvar_tree::init ()
{
	const auto *client_class = gInts->Client->GetAllClasses ();
	while (client_class != nullptr) {
		const auto class_info = std::make_shared<node> (0, nullptr);
		auto *     recv_table = client_class->table;
		populate_nodes (recv_table, &class_info->nodes);
		nodes.emplace (recv_table->GetName (), class_info);

		client_class = client_class->nextClass;
	}
}

void netvar_tree::populate_nodes (RecvTable *recv_table, map_type *map)
{
	for (auto i = 0; i < recv_table->GetNumProps (); i++) {
		const auto *prop      = recv_table->GetPropA (i);
		const auto  prop_info = std::make_shared<node> (prop->GetOffset (), const_cast<RecvProp *> (prop));

		if (prop->GetType () == DPT_DataTable)
			populate_nodes (prop->GetDataTable (), &prop_info->nodes);

		map->emplace (prop->GetName (), prop_info);
	}
}

struct generic_ui_cmd
{
	uint64_t id;
	uint32_t payload_len;
	uint8_t iface_name_len;
	uint8_t slot_name_len;
	GenericUI::EventTypes e;
	/*
	char iface_name[iface_name_len];
	char slot_name[slot_name_len];
	char payload[payload_len];
	*/
};

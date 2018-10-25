#ifndef PACKETPP_VLAN_LAYER
#define PACKETPP_VLAN_LAYER

#include "Layer.h"
#include "EthLayer.h"
#if defined(WIN32) || defined(WINx64)
#include <winsock2.h>
#endif

/// @file

/**
 * \namespace pcpp
 * \brief The main namespace for the PcapPlusPlus lib
 */
namespace pcpp
{

	/**
	 * @struct vlan_header
	 * Represents a VLAN header
	 */
#pragma pack(push, 1)
	struct vlan_header {
/**    
 *    0                 1                   
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |Prio |C|         VLAN ID     |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
		uint16_t vlan;
		/** Ethernet type for next layer */
		uint16_t etherType;
	};
#pragma pack(pop)


	/**
	 * @class VlanLayer
	 * Represents a VLAN tunnel layer
	 */
	class VlanLayer : public Layer
	{
	public:
		 /** A constructor that creates the layer from an existing packet raw data
		 * @param[in] data A pointer to the raw data
		 * @param[in] dataLen Size of the data in bytes
		 * @param[in] prevLayer A pointer to the previous layer
		 * @param[in] packet A pointer to the Packet instance where layer will be stored in
		 */
		VlanLayer(uint8_t* data, size_t dataLen, Layer* prevLayer, Packet* packet) : Layer(data, dataLen, prevLayer, packet) { m_Protocol = VLAN; }

		/**
		 * A constructor that allocates a new VLAN header
		 * @param[in] vlanID VLAN ID
		 * @param[in] cfi CFI value
		 * @param[in] priority Priority value
		 * @param[in] etherType Protocol EtherType of the next layer
		 */
		VlanLayer(const uint16_t vlanID, bool cfi, uint8_t priority, uint16_t etherType);

		~VlanLayer() {}

		/**
		 * Get a pointer to the VLAN header. Notice this points directly to the data, so every change will change the actual packet data
		 * @return A pointer to the vlan_header
		 */
		inline vlan_header* getVlanHeader() { return (vlan_header*)m_Data; }

		/**
		 * Get the VLAN ID value. This method differs from vlan_header#vlanID because vlan_header#vlanID is 12 bits long in a 16 bit field.
		 * This methods extracts only the 12 bit relevant for the VLAN ID
		 * @return VLAN ID value
		 * @todo Verify it works in big endian machines as well
		 */
		inline uint16_t getVlanID() { return htobe16(getVlanHeader()->vlan) & 0xFFF; }

		/**
		 * @return The CFI bit value
		 * @todo Verify it works in big endian machines as well
		 */
		inline uint8_t getCFI() { return ((htobe16(getVlanHeader()->vlan) >> 12) & 1); }

		/**
		 * @return The priority value
		 * @todo Verify it works in big endian machines as well
		 */
		inline uint8_t getPriority() { return (htobe16(getVlanHeader()->vlan) >> 13) & 7; }

		/**
		 * Set VLAN ID. This method differs from setting vlan_header#vlanID because vlan_header#vlanID is 12 bits long in a 16 bit field.
		 * This methods sets only the 12 bit relevant for the VLAN ID
		 * @param[in] id The VLAN ID to set
		 * @todo Verify it works in big endian machines as well
		 */
		inline void setVlanID(uint16_t id) { getVlanHeader()->vlan = htobe16((be16toh(getVlanHeader()->vlan) & (~0xFFF)) | (id & 0xFFF)); }

		/**
		 * Set CFI bit
		 * @param[in] cfi The CFI bit to set
		 * @todo Verify it works in big endian machines as well
		 */
		inline void setCFI(bool cfi) { getVlanHeader()->vlan = htobe16((be16toh(getVlanHeader()->vlan) & (~(1 << 12))) | ((cfi & 1) << 12)); }

		/**
		 * Set priority value
		 * @param[in] priority The priority value to set
		 * @todo Verify it works in big endian machines as well
		 */
		inline void setPriority(uint8_t priority) { getVlanHeader()->vlan = htobe16((be16toh(getVlanHeader()->vlan) & (~(7 << 13))) | ((priority & 7) << 13)); }

		// implement abstract methods

		/**
		 * Currently identifies the following next layers: IPv4Layer, IPv6Layer, ArpLayer, VlanLayer, MplsLayer. Otherwise sets PayloadLayer
		 */
		void parseNextLayer();

		/**
		 * @return Size of vlan_header
		 */
		inline size_t getHeaderLen() { return sizeof(vlan_header); }

		/**
		 * Does nothing for this layer
		 */
		void computeCalculateFields() {}

		std::string toString();

		OsiModelLayer getOsiModelLayer() { return OsiModelDataLinkLayer; }
	};

} // namespace pcpp

#endif /* PACKETPP_VLAN_LAYER */

/* This file is part of i2c-tools.
 *
 * libregmap is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libregmap is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with i2c-tools. If not, see <http://www.gnu.org/licenses/>.
 */

#include <string>
#include "regmap.hpp"

namespace libi210 {

class FlashDump;
class FlashIOMapped {

public:
	FlashIOMapped(regmap::pci::IOMapped& IOMap)
	: m_oIOMap(IOMap),
	  m_oIOADDR(m_oIOMap.get<regmap::Register32_t>("IOADDR")),
	  m_oIODATA(m_oIOMap.get<regmap::Register32_t>("IODATA")) {}

	FlashDump dump();
	void write(const FlashDump& dump);

private:
	void writeOffset(std::uint32_t offset, std::uint32_t value);
	std::uint32_t readOffset(std::uint32_t offset);

	regmap::pci::IOMapped&	m_oIOMap;
	regmap::Register32_t	m_oIOADDR;
	regmap::Register32_t	m_oIODATA;
};

class FlashMemMapped {

public:
	FlashMemMapped(regmap::pci::MemMapped& MemMap)
	: m_oMemMap(MemMap) {}

	FlashDump dump();
	void write(const FlashDump& dump);

private:
	regmap::pci::MemMapped&	m_oMemMap;
};

class FlashDump : public regmap::RegMapBase<regmap::RegBackendMemory> {

public:
	FlashDump(std::string defFile, unsigned int size)
	: RegMapBase(defFile), m_pMemory(malloc(size), free), m_uFlashSize(size) {
		m_oRegBackendMemory = regmap::RegBackendMemory(m_pMemory, size);
		m_oRegBackend = m_oRegBackendMemory;
	}

	unsigned int size() { return m_uFlashSize; }
	void* data() { return m_pMemory.get(); }

private:
	regmap::BackendMemory_t  m_pMemory;
	regmap::RegBackendMemory m_oRegBackendMemory;
	unsigned int     m_uFlashSize;
};

};

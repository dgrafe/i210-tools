/* This file is part of i2c-tools.
 *
 * i210-tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * i210-tools is distributed in the hope that it will be useful,
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

template <class T>
class I210Common {

public:
	I210Common(T&&) = delete;
	I210Common(T& backend)
	: m_oBackend(backend){}

	// flash
	virtual uint32_t getFlashSize();
	virtual bool getSecureMode();
	virtual bool getFlashDetected();
	virtual bool getFlashPresent();
	virtual bool getFlashInUse();
	virtual uint32_t getFlashSpeed();
	virtual FlashDump dump();

	// firmware
	virtual uint16_t getFirmwareRevision();

	std::ostream& operator<< (std::ostream& stream);
protected:
	T&	m_oBackend;
};

class FlashIOMapped : public I210Common<regmap::pci::IOMapped> {

public:
	FlashIOMapped(regmap::pci::IOMapped& IOMap)
	: I210Common(IOMap), m_oIOMap(IOMap),
	  m_oIOADDR(m_oIOMap.get<regmap::Register32_t>("IOADDR")),
	  m_oIODATA(m_oIOMap.get<regmap::Register32_t>("IODATA")) {

		m_oIOMap.getBackend().setIndirection(m_oIOADDR.getOffset(), m_oIODATA.getOffset());
	  }

	void write(const FlashDump& dump);

private:
	regmap::pci::IOMapped&	m_oIOMap;
	regmap::Register32_t	m_oIOADDR;
	regmap::Register32_t	m_oIODATA;
};

class FlashMemMapped : public I210Common<regmap::pci::MemMapped> {

public:
	FlashMemMapped(regmap::pci::MemMapped& MemMap)
	: I210Common(MemMap), m_oMemMap(MemMap) {}

	void write(const FlashDump& dump);

private:
	regmap::pci::MemMapped&	m_oMemMap;
};

class FlashDump : public regmap::RegMapBase<regmap::RegBackendMemory>, I210Common<regmap::RegMapBase<regmap::RegBackendMemory>> {

public:
	FlashDump(std::string defFile, unsigned int size)
	: I210Common(static_cast<regmap::RegMapBase<regmap::RegBackendMemory>&>(*this)), RegMapBase(defFile),
	  m_pMemory(malloc(size), free), m_uFlashSize(size) {
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

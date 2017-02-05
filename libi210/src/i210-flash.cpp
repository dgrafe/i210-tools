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

#include "i210-flash.hpp"

namespace libi210 {

FlashDump FlashMemMapped::dump() {

	FlashDump buf(m_oMemMap.defFile(), 1048576);
	auto FLSWCNT = m_oMemMap.get<regmap::Register32_t>("FLSWCNT");
	auto FLSWCTL = m_oMemMap.get<regmap::Register32_t>("FLSWCTL");
	auto FLSWDATA = m_oMemMap.get<regmap::Register32_t>("FLSWDATA");

	for (std::uint32_t i = 0; i < buf.size(); i += 4) {

		// we're going to read 4 data bytes
		FLSWCNT = 0x4;

		// at flash address...
		FLSWCTL = 0x00000000 | i;

		// wait until the data is available in FLSWDATA
		FLSWCTL.wait(std::chrono::microseconds(1));

		// read the 4 bytes and store them
		std::uint32_t val = FLSWDATA;
		memcpy(static_cast<char*>(buf.data()) + i, &val, sizeof(val));
	}

	return buf;
}

FlashDump FlashIOMapped::dump() {

	FlashDump buf(m_oIOMap.defFile(), 1048576);
	const std::uint32_t FLSWCNT = m_oIOMap.get<regmap::Register32_t>("FLSWCNT").getOffset();
	const std::uint32_t FLSWCTL = m_oIOMap.get<regmap::Register32_t>("FLSWCTL").getOffset();
	const std::uint32_t FLSWDATA = m_oIOMap.get<regmap::Register32_t>("FLSWDATA").getOffset();
	const std::uint32_t FLSWCTL_DONE = m_oIOMap.get<regmap::Register32_t>("FLSWCTL")["DONE_MASK"];

	for (std::uint32_t i = 0; i < buf.size(); i += 4) {

		// we're going to read 4 data bytes
		writeOffset(FLSWCNT, 0x4);

		// at flash address...
		writeOffset(FLSWCTL, 0x00000000 | i);

		// wait until the data is available in FLSWDATA
		std::uint32_t val;
		do {
			val = readOffset(FLSWCTL);
		} while (FLSWCTL_DONE != (val & FLSWCTL_DONE));

		// read the 4 bytes and store them
		val = readOffset(FLSWDATA);
		memcpy(static_cast<char*>(buf.data()) + i, &val, sizeof(val));
	}

	return buf;
}


void FlashIOMapped::writeOffset(std::uint32_t offset, std::uint32_t value) {

	m_oIOADDR = offset;
	m_oIODATA = value;
}

std::uint32_t FlashIOMapped::readOffset(std::uint32_t offset) {
	
	m_oIOADDR = offset;
	return m_oIODATA;
}

};

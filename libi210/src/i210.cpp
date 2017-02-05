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

#include "i210.hpp"

namespace libi210 {

i210::i210(const regmap::pci::BDF& bdf, const std::string& defFile, eRegInterface interface)
 : m_oBDF(bdf), m_oMemMap(bdf, defFile, regmap::pci::BAR0),
   m_oIOMap(bdf, defFile, regmap::pci::BAR2),
   m_oFlashIO(m_oIOMap), m_oFlashMem(m_oMemMap), m_eInterface(interface) {

}

FlashDump i210::dumpFlash() {
	return m_eInterface == INTERFACE_MEM ? m_oFlashMem.dump() : m_oFlashIO.dump();
}

}

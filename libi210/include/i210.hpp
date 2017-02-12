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
#include "i210-flash.hpp"

namespace libi210 {

enum eRegInterface {
	INTERFACE_MEM,
	INTERFACE_IO
};

class i210 {

public:
	i210() = delete;
	i210(const regmap::pci::BDF& bdf, const std::string& defFile, eRegInterface interface);

	FlashDump dumpFlash();
	friend std::ostream& operator<< (std::ostream& stream, i210& instance);
private:
	regmap::pci::BDF	m_oBDF;
	regmap::pci::MemMapped	m_oMemMap;
	regmap::pci::IOMapped	m_oIOMap;
	FlashIOMapped		m_oFlashIO;
	FlashMemMapped		m_oFlashMem;
	eRegInterface		m_eInterface;
};

};

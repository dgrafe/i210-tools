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

#include <boost/program_options.hpp>
#include "i210.hpp"

namespace po = boost::program_options;

int main(int argc, char **argv) {

	try {
	        po::options_description desc("Supported Features");
		desc.add_options()
			("help", "print this help message")
			("dump", "dumps the flash content from the i210 to stdout")
		;

		po::options_description mandatory("Mandatory options");
		mandatory.add_options()
			("bdf", po::value<std::string>(), "<b:d.f> bus location of the i210")
		;

		desc.add(mandatory);
		
		po::options_description optional("Optional options");
		optional.add_options()
			("io", "Use memory mapped io instead of mem mapped registers")
		;

		desc.add(optional);

		po::variables_map vm;        
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);  

		if (vm.count("help") || !vm.count("bdf")) {
			std::cout << desc << std::endl;
			return 0;
		}

		std::stringstream data(vm["bdf"].as<std::string>());
		std::string component;
		regmap::pci::BDF bdf(0,0,0);

		std::getline(data, component, ':');
		bdf.m_uBus = static_cast<std::uint8_t>(std::stoi(component));
		std::getline(data, component, '.');
		bdf.m_uDevice = static_cast<std::uint8_t>(std::stoi(component));
		std::getline(data, component, '\n');
		bdf.m_uFunction = static_cast<std::uint8_t>(std::stoi(component));

		libi210::i210 i210Chip(bdf, "i210.json", vm.count("io") ? libi210::INTERFACE_IO : libi210::INTERFACE_MEM);

		if (vm.count("dump")) {
			auto dump = i210Chip.dumpFlash();
			fwrite(dump.data(), 1, dump.size(), stdout);
		}

	} catch(const std::exception& ex) {

		std::cout << "Error: " << ex.what() << std::endl;
		return -1;
	}	

	return 0;
}


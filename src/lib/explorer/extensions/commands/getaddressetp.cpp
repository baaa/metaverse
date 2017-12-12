/**
 * Copyright (c) 2016-2017 mvs developers 
 *
 * This file is part of metaverse-explorer.
 *
 * metaverse-explorer is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <jsoncpp/json/json.h>
#include <metaverse/explorer/extensions/base_helper.hpp>
#include <metaverse/explorer/extensions/commands/getaddressetp.hpp>
#include <metaverse/explorer/extensions/command_extension_func.hpp>
#include <metaverse/explorer/extensions/command_assistant.hpp>

namespace libbitcoin {
namespace explorer {
namespace commands {


/************************ getaddressetp *************************/

console_result getaddressetp::invoke (std::ostream& output,
        std::ostream& cerr, libbitcoin::server::server_node& node)
{
    auto& blockchain = node.chain_impl();
    auto& addr = argument_.address;
    bc::explorer::commands::balances addr_balance{0, 0, 0, 0};

    std::string type{"all"};
    sync_fetchbalance(addr, type, blockchain, addr_balance, 0);

    Json::Value jroot;
    Json::Value jv;
    jv["address"] = addr.encoded();
    jv["confirmed"] = addr_balance.confirmed_balance;
    jv["received"]  = addr_balance.total_received;
    jv["unspent"]   = addr_balance.unspent_balance;
    jv["frozen"]    = addr_balance.frozen_balance;

    jroot["balance"] = jv;
    
    output << jroot.toStyledString();

    return console_result::okay;
}



} // namespace commands
} // namespace explorer
} // namespace libbitcoin

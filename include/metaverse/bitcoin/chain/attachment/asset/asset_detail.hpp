/**
 * Copyright (c) 2011-2015 metaverse developers (see AUTHORS)
 *
 * This file is part of mvs-node.
 *
 * metaverse is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
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
#ifndef MVS_CHAIN_ATTACHMENT_ASSET_DETAIL_HPP
#define MVS_CHAIN_ATTACHMENT_ASSET_DETAIL_HPP

#include <cstdint>
#include <istream>
#include <vector>
#include <metaverse/bitcoin/chain/point.hpp>
#include <metaverse/bitcoin/chain/script/script.hpp>
#include <metaverse/bitcoin/define.hpp>
#include <metaverse/bitcoin/utility/reader.hpp>
#include <metaverse/bitcoin/utility/writer.hpp>
#include "asset_cert.hpp"

using namespace libbitcoin::chain;

#define MODEL2UINT8(model)  (static_cast<typename std::underlying_type<asset_detail::attenuation_model>::type>(model))
#define ATTENUATION_MODEL_NONE              MODEL2UINT8(asset_detail::attenuation_model::none)
#define ATTENUATION_MODEL_FIXED_QUANTITY    MODEL2UINT8(asset_detail::attenuation_model::fixed_quantity)
#define ATTENUATION_MODEL_FIXED_RATE        MODEL2UINT8(asset_detail::attenuation_model::fixed_rate)
#define ATTENUATION_MODEL_FIRST_UNUSED      MODEL2UINT8(asset_detail::attenuation_model::unused1)

namespace libbitcoin {
namespace chain {

BC_CONSTEXPR size_t ASSET_DETAIL_SYMBOL_FIX_SIZE = 64;
BC_CONSTEXPR size_t ASSET_DETAIL_MAX_SUPPLY_FIX_SIZE = 8;
BC_CONSTEXPR size_t ASSET_DETAIL_ASSET_TYPE_FIX_SIZE = 4;
BC_CONSTEXPR size_t ASSET_DETAIL_ISSUER_FIX_SIZE = 64;
BC_CONSTEXPR size_t ASSET_DETAIL_ADDRESS_FIX_SIZE = 64;
BC_CONSTEXPR size_t ASSET_DETAIL_DESCRIPTION_FIX_SIZE = 64;

BC_CONSTEXPR size_t ASSET_DETAIL_FIX_SIZE = ASSET_DETAIL_SYMBOL_FIX_SIZE
            + ASSET_DETAIL_MAX_SUPPLY_FIX_SIZE
            + ASSET_DETAIL_ASSET_TYPE_FIX_SIZE
            + ASSET_DETAIL_ISSUER_FIX_SIZE
            + ASSET_DETAIL_ADDRESS_FIX_SIZE
            + ASSET_DETAIL_DESCRIPTION_FIX_SIZE;

class BC_API asset_detail
{
public:
    static BC_CONSTEXPR uint8_t forbidden_secondaryissue_threshold = 0;
    static BC_CONSTEXPR uint8_t freely_secondaryissue_threshold = 127;

    typedef std::vector<asset_detail> list;
    asset_detail();
    asset_detail(std::string symbol, uint64_t maximum_supply,
        uint8_t decimal_number, uint8_t threshold, std::string issuer,
        std::string address, std::string description);
    static asset_detail factory_from_data(const data_chunk& data);
    static asset_detail factory_from_data(std::istream& stream);
    static asset_detail factory_from_data(reader& source);
    static uint64_t satoshi_fixed_size();
    bool from_data(const data_chunk& data);
    bool from_data(std::istream& stream);
    bool from_data(reader& source);
    data_chunk to_data() const;
    void to_data(std::ostream& stream) const;
    void to_data(writer& sink) const;

    std::string to_string() const;

    bool is_valid() const;
    void reset();
    uint64_t serialized_size() const;
    const std::string& get_symbol() const;
    void set_symbol(const std::string& symbol);
    uint64_t get_maximum_supply() const;
    void set_maximum_supply(uint64_t maximum_supply);
    uint8_t get_decimal_number() const;
    void set_decimal_number(uint8_t decimal_number);
    const std::string& get_issuer() const;
    void set_issuer(const std::string& issuer);
    const std::string& get_address() const;
    void set_address(const std::string& address);
    const std::string& get_description() const;
    void set_description(const std::string& description);

    asset_cert_type get_asset_cert_mask() const;

    bool is_asset_secondaryissue() const;
    void set_asset_secondaryissue();
    uint8_t get_secondaryissue_threshold() const;
    void set_secondaryissue_threshold(uint8_t share);

    bool is_secondaryissue_threshold_value_ok() const;
    bool is_secondaryissue_legal() const;

    static bool is_secondaryissue_forbidden(uint8_t threshold);
    static bool is_secondaryissue_freely(uint8_t threshold);
    static bool is_secondaryissue_threshold_value_ok(uint8_t threshold);
    static bool is_secondaryissue_legal(uint8_t threshold);
    static bool is_secondaryissue_owns_enough(uint64_t own, uint64_t total, uint8_t threshold);

    void set_attenuation_model(attenuation_model model) { attenuation_model_index = MODEL2UINT8(model); }
    attenuation_model get_attenuation_model() const { return (attenuation_model)attenuation_model_index; }
    void set_attenuation_model_index(uint8_t index) { attenuation_model_index = index; }
    uint8_t get_attenuation_model_index() const { return attenuation_model_index; }
    bool is_attenuation_model_index_valid() { return attenuation_model_index < ATTENUATION_MODEL_FIRST_UNUSED; }

private:
    // NOTICE: ref CAssetDetail in transaction.h
    // asset_detail and CAssetDetail should have the same size and order.
    // uint32_t asset_type in CAssetDetail is divided into four uint8_t parts here.
    std::string symbol;
    uint64_t maximum_supply;
    uint8_t decimal_number;
    uint8_t secondaryissue_threshold;
    uint8_t attenuation_model_index:3;
    uint8_t unused2:5;
    uint8_t unused3;
    std::string issuer;
    std::string address;
    std::string description;
};

} // namespace chain
} // namespace libbitcoin

#endif


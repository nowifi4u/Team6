#pragma once

namespace detail {
    template <typename Map>
    struct readable_only_pmap : public Map {
        readable_only_pmap(Map map) : Map(map) { }

        // overrule the category tag
        typedef boost::readable_property_map_tag category;
    };
}

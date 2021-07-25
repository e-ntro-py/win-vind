#include "bind/mode/options.hpp"

#include "g_params.hpp"
#include "opt/async_uia_cache_builder.hpp"
#include "uia/uia.hpp"
#include "util/box_2d.hpp"
#include "util/def.hpp"
#include "util/rect.hpp"
#include "util/winwrap.hpp"

#include <limits>

#if defined(DEBUG)
#include <iostream>
#include "util/debug.hpp"
#endif

#undef max


namespace vind
{
    namespace options {
        void focus_nearest_textarea(
                HWND hwnd,
                const Point2D& point,
                TextAreaScanner& instance) {

            std::vector<SmartElement> editables{} ;

            if(gparams::get_b("uiacachebuild")) {
                auto root_elem = AsyncUIACacheBuilder::get_root_element(hwnd) ;
                instance.scan(root_elem, editables) ;
            }
            else {
                instance.scan(hwnd, editables) ;
            }

            if(editables.empty()) {
                return ;
            }

            if(editables.size() == 1) {
                if(util::is_failed(editables.front()->SetFocus())) {
                    throw RUNTIME_EXCEPT("SetFocus Failed.") ;
                }
                return ;
            }

            SmartElement nearest ;
            auto min_distance = std::numeric_limits<double>::max() ;

            for(auto& elem : editables) {
                // scan GUI objects only at leaves in tree.
                Box2D box ;
                if(util::is_failed(elem->get_CachedBoundingRectangle(&box.data()))) {
                    throw RUNTIME_EXCEPT("Could not get the a rectangle of a element.") ;
                }

                auto distance = util::l2_distance_nosq(
                        point.x(), point.y(),
                        box.center_x(), box.center_y()) / 100 ;

                if(min_distance > distance) {
                    nearest = elem ;
                    min_distance = distance ;
                }
            }

            if(util::is_failed(nearest->SetFocus())) {
                throw RUNTIME_EXCEPT("SetFocus Failed.") ;
            }
        }
    }
}

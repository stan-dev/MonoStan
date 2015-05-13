#ifndef STAN_MODEL_INDEXING_INDEX_LIST_HPP
#define STAN_MODEL_INDEXING_INDEX_LIST_HPP

#include <stan/model/indexing/typelist.hpp>

namespace stan {
  namespace model {

    /**
     * Structure for an empty (size zero) index list.
     */
    struct nil_index_list {

      /**
       * The typelist for an empty index list, namely
       * <code>model::nil</code>.
       */
      typedef model::nil typelist;

      nil_index_list() { 
      }

    };


    /**
     * Template structure for an index list consisting of a head and
     * tail index.  
     *
     * @tparam H type of index stored as the head of the list.
     * @tparam T type of index list stored as the tail of the list.
     */
    template <typename H, typename T>
    struct cons_index_list {

      /**
       * The typelist for a non-empty index list, consisting of the
       * index type of the head of the list followed by the typelist
       * of the tail of the list.
       */
      typedef typename model::cons<typename H::index_type, 
                                   typename T::typelist> typelist;

      const H head_;
      const T tail_;

      /**
       * Construct a non-empty index list with the specified index for
       * a head and specified index list for a tail.
       *
       * @param head Index for head.
       * @param tail Index list for tail.
       */
      cons_index_list(const H& head, const T& tail) 
        : head_(head),
          tail_(tail) {
      }

    };

  }
}
#endif

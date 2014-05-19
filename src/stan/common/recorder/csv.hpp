#ifndef __STAN__COMMON__RECORDER__CSV_HPP__
#define __STAN__COMMON__RECORDER__CSV_HPP__

#include <ostream>
#include <string>
#include <vector>

namespace stan {
  namespace common {
    namespace recorder {
      
      /**
       * Writes out a vector as string.
       */
      class csv {
      private:
        std::ostream *o_;
        const bool has_stream_;
        const std::string prefix_;
      
      public:
        /**
         * Construct an object.
         *
         * @param o pointer to stream. Will accept 0.
         */
        csv(std::ostream *o, std::string prefix) 
          : o_(o), has_stream_(o != 0), prefix_(prefix) { }
      
        /**
         * Print vector as csv.
         *
         * Uses the insertion operator to write out the elements
         * as comma separated values, flushing the buffer after the
         * line is complete
         * 
         * @tparam T type of element
         * @param x vector of type T
         */
        template <class T>
        void operator()(const std::vector<T>& x) {
          if (!has_stream_)
            return;
        
          if (x.size() != 0) {
            *o_ << x[0];
            for (typename std::vector<T>::size_type n = 1; n < x.size(); n++) {
              *o_ << "," << x[n];
            }
          }
          *o_ << std::endl;
        }
      
        /**
         * Print single string with a prefix
         *
         * Uses the insertion operator to write out a string
         * as comma separated values, flushing the buffer after the
         * line is complete
         * 
         * @param x string to print with prefix in front
         */
        void operator()(const std::string x) {
          if (!has_stream_)
            return;
          *o_ << prefix_ << x << std::endl;
        }
      
        /**
         * Prints a blank line. No prefix, no nothing.
         *
         */
        void operator()() {
          if (!has_stream_)
            return;
          *o_ << std::endl;
        }
      
        /**
         * Indicator function for whether the instance is recording.
         *
         * For this class, returns true if it has a stream.
         */
        bool is_recording() const {
          return has_stream_;
        }
      };


    }
  }
}

#endif

// Copyright (C) 2012  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_ORDERED_SAMPLE_PaIR_Hh_
#define DLIB_ORDERED_SAMPLE_PaIR_Hh_

#include <limits>

namespace dlib
{

// ----------------------------------------------------------------------------------------

    class ordered_sample_pair 
    {
    public:
        ordered_sample_pair(
        ) : 
            _index1(0),
            _index2(0)
        {
            _distance = 1;
        }

        ordered_sample_pair (
            const unsigned long idx1,
            const unsigned long idx2
        )
        {
            _distance = 1;
            _index1 = idx1;
            _index2 = idx2;
        }

        ordered_sample_pair (
            const unsigned long idx1,
            const unsigned long idx2,
            const double dist
        )
        {
            _distance = dist;
            _index1 = idx1;
            _index2 = idx2;
        }

        const unsigned long& index1 (
        ) const { return _index1; }

        const unsigned long& index2 (
        ) const { return _index2; }

        const double& distance (
        ) const { return _distance; }

    private:
        unsigned long _index1;
        unsigned long _index2;
        double _distance;
    };

// ----------------------------------------------------------------------------------------

    inline bool operator == (
        const ordered_sample_pair& a,
        const ordered_sample_pair& b
    ) 
    {
        return a.index1() == b.index1() && a.index2() == b.index2();
    }

    inline bool operator != (
        const ordered_sample_pair& a,
        const ordered_sample_pair& b
    ) 
    {
        return !(a == b); 
    }

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_ORDERED_SAMPLE_PaIR_Hh_

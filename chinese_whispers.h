// Copyright (C) 2012  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_CHINESE_WHISPErS_Hh_
#define DLIB_CHINESE_WHISPErS_Hh_

#include <vector>
#include <map>
#include "sample_pair.h"
#include "ordered_sample_pair.h"
#include <algorithm>
#include <ctime>
#define DLIB_ASSERT(p1, p2)

namespace dlib
{
// ----------------------------------------------------------------------------------------
	struct rand 
	{
		rand() { srand(time(NULL)); }
		int get_random_64bit_number() const { return ::rand(); }
	};

// ----------------------------------------------------------------------------------------

	int max_index_plus_one(const std::vector<ordered_sample_pair>& pairs)
	{
		if (pairs.size() == 0)
		{
			return 0;
		}
		else
		{
			unsigned long max_idx = 0;
			for (unsigned long i = 0; i < pairs.size(); ++i)
			{
				if (pairs[i].index1() > max_idx)
					max_idx = pairs[i].index1();
				if (pairs[i].index2() > max_idx)
					max_idx = pairs[i].index2();
			}

			return max_idx + 1;
		}
	}

// ----------------------------------------------------------------------------------------

	template <
		typename alloc1, 
		typename alloc2
	>
	void find_neighbor_ranges (
	const std::vector<ordered_sample_pair,alloc1>& edges,
	std::vector<std::pair<unsigned long, unsigned long>,alloc2>& neighbors
	)
	{
		// make sure requires clause is not broken
		DLIB_ASSERT(is_ordered_by_index(edges),
			"\t void find_neighbor_ranges()"
			<< "\n\t Invalid inputs were given to this function"
			);


		// setup neighbors so that [neighbors[i].first, neighbors[i].second) is the range
		// within edges that contains all node i's edges.
		const unsigned long num_nodes = max_index_plus_one(edges);
		neighbors.assign(num_nodes, std::make_pair(0,0));
		unsigned long cur_node = 0;
		unsigned long start_idx = 0;
		for (unsigned long i = 0; i < edges.size(); ++i)
		{
			if (edges[i].index1() != cur_node)
			{
				neighbors[cur_node] = std::make_pair(start_idx, i);
				start_idx = i;
				cur_node = edges[i].index1();
			}
		}
		if (neighbors.size() != 0)
			neighbors[cur_node] = std::make_pair(start_idx, (unsigned long)edges.size());
	}

// ----------------------------------------------------------------------------------------

	template <
		typename alloc1, 
		typename alloc2
	>
	void convert_unordered_to_ordered (
	const std::vector<sample_pair,alloc1>& edges,
	std::vector<ordered_sample_pair,alloc2>& out_edges
	)
	{
		out_edges.clear();
		out_edges.reserve(edges.size()*2);
		for (unsigned long i = 0; i < edges.size(); ++i)
		{
			out_edges.push_back(ordered_sample_pair(edges[i].index1(), edges[i].index2(), edges[i].distance()));
			if (edges[i].index1() != edges[i].index2())
				out_edges.push_back(ordered_sample_pair(edges[i].index2(), edges[i].index1(), edges[i].distance()));
		}
	}

// ----------------------------------------------------------------------------------------

    inline unsigned long chinese_whispers (
        const std::vector<ordered_sample_pair>& edges,
        std::vector<unsigned long>& labels,
        const unsigned long num_iterations,
        dlib::rand& rnd
    )
    {
        // make sure requires clause is not broken
        DLIB_ASSERT(is_ordered_by_index(edges),
                    "\t unsigned long chinese_whispers()"
                    << "\n\t Invalid inputs were given to this function"
        );

        labels.clear();
        if (edges.size() == 0)
            return 0;

        std::vector<std::pair<unsigned long, unsigned long> > neighbors;
        find_neighbor_ranges(edges, neighbors);

        // Initialize the labels, each node gets a different label.
        labels.resize(neighbors.size());
        for (unsigned long i = 0; i < labels.size(); ++i)
            labels[i] = i;


        for (unsigned long iter = 0; iter < neighbors.size()*num_iterations; ++iter)
        {
            // Pick a random node.
            const unsigned long idx = rnd.get_random_64bit_number()%neighbors.size();

            // Count how many times each label happens amongst our neighbors.
            std::map<unsigned long, double> labels_to_counts;
            const unsigned long end = neighbors[idx].second;
            for (unsigned long i = neighbors[idx].first; i != end; ++i)
            {
                labels_to_counts[labels[edges[i].index2()]] += edges[i].distance();
            }

            // find the most common label
            std::map<unsigned long, double>::iterator i;
            double best_score = -std::numeric_limits<double>::infinity();
            unsigned long best_label = labels[idx];
            for (i = labels_to_counts.begin(); i != labels_to_counts.end(); ++i)
            {
                if (i->second > best_score)
                {
                    best_score = i->second;
                    best_label = i->first;
                }
            }

            labels[idx] = best_label;
        }


        // Remap the labels into a contiguous range.  First we find the
        // mapping.
        std::map<unsigned long,unsigned long> label_remap;
        for (unsigned long i = 0; i < labels.size(); ++i)
        {
            const unsigned long next_id = label_remap.size();
            if (label_remap.count(labels[i]) == 0)
                label_remap[labels[i]] = next_id;
        }
        // now apply the mapping to all the labels.
        for (unsigned long i = 0; i < labels.size(); ++i)
        {
            labels[i] = label_remap[labels[i]];
        }

        return label_remap.size();
    }

// ----------------------------------------------------------------------------------------

    inline unsigned long chinese_whispers (
        const std::vector<sample_pair>& edges,
        std::vector<unsigned long>& labels,
        const unsigned long num_iterations,
        dlib::rand& rnd
    )
    {
        std::vector<ordered_sample_pair> oedges;
        convert_unordered_to_ordered(edges, oedges);
        std::sort(oedges.begin(), oedges.end(), &order_by_index<ordered_sample_pair>);

        return chinese_whispers(oedges, labels, num_iterations, rnd);
    }

// ----------------------------------------------------------------------------------------

    inline unsigned long chinese_whispers (
        const std::vector<sample_pair>& edges,
        std::vector<unsigned long>& labels,
        const unsigned long num_iterations = 100
    )
    {
        dlib::rand rnd;
        return chinese_whispers(edges, labels, num_iterations, rnd);
    }

// ----------------------------------------------------------------------------------------

    inline unsigned long chinese_whispers (
        const std::vector<ordered_sample_pair>& edges,
        std::vector<unsigned long>& labels,
        const unsigned long num_iterations = 100
    )
    {
        dlib::rand rnd;
        return chinese_whispers(edges, labels, num_iterations, rnd);
    }

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_CHINESE_WHISPErS_Hh_

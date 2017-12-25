//          Copyright Joe Coder 2004 - 2006.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/serialization.hpp>

// Thanks to sehe at stackoverflow : https://stackoverflow.com/questions/31006792/how-to-serialize-boostdynamic-bitset
// For the boost::dynamic_bitset serialization
namespace boost {
	namespace serialization {

		template <typename Ar, typename Block, typename Alloc>
		void save(Ar& ar, dynamic_bitset<Block, Alloc> const& bs, unsigned) {
			size_t num_bits = bs.size();
			std::vector<Block> blocks(bs.num_blocks());
			to_block_range(bs, blocks.begin());

			ar & num_bits & blocks;
		}

		template <typename Ar, typename Block, typename Alloc>
		void load(Ar& ar, dynamic_bitset<Block, Alloc>& bs, unsigned) {
			size_t num_bits;
			std::vector<Block> blocks;
			ar & num_bits & blocks;

			bs.resize(num_bits);
			from_block_range(blocks.begin(), blocks.end(), bs);
			bs.resize(num_bits);
		}

		template <typename Ar, typename Block, typename Alloc>
		void serialize(Ar& ar, dynamic_bitset<Block, Alloc>& bs, unsigned version) {
			split_free(ar, bs, version);
		}

	}
}
// end

struct bitsetR {
	boost::dynamic_bitset<> x;
	bitsetR(std::string input) :x(input) {}
private:
	friend class boost::serialization::access;
	template <class Archive> void serialize(Archive &ar, const unsigned int) {
		ar &x;
	}
};

struct p {
	char a; bitsetR bits;
	p() :bits("") {}
private:
	friend class boost::serialization::access;
	template <class Archive> void serialize(Archive &ar, const unsigned int) {
		ar &a;
		ar &bits;
	}
};

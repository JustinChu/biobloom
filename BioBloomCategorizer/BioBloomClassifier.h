/*
 * BioBloomClassifier.h
 *
 *  Created on: Oct 17, 2012
 *      Author: cjustin
 */

#ifndef BIOBLOOMCLASSIFIER_H_
#define BIOBLOOMCLASSIFIER_H_
#include <vector>
#include <string>
#include "boost/unordered/unordered_map.hpp"
#include "boost/shared_ptr.hpp"
#include "Common/BloomFilterInfo.h"
#include "MultiFilter.h"
#include "DataLayer/FastaReader.h"
#include "Common/ReadsProcessor.h"
#include "Common/Uncompress.h"
#include "Common/BloomFilter.h"

using namespace std;
using namespace boost;

class BioBloomClassifier {
public:
	explicit BioBloomClassifier(const vector<string> &filterFilePaths,
			double scoreThreshold, const string &outputPrefix,
			const string &outputPostFix, unsigned streakThreshold,
			unsigned minHit, bool minHitOnly);
	void filter(const vector<string> &inputFiles);
	void filterPrint(const vector<string> &inputFiles,
			const string &outputType);
	void filterPair(const string &file1, const string &file2);
	void filterPairPrint(const string &file1, const string &file2,
			const string &outputType);
	void filterPairBAM(const string &file);
	void filterPairBAMPrint(const string &file, const string &outputType);

	virtual ~BioBloomClassifier();

private:
	void loadFilters(const vector<string> &filterFilePaths);
	bool fexists(const string &filename) const;
	void evaluateReadStd(const FastqRecord &rec, const string &hashSig,
			unordered_map<string, bool> &hits);
	void evaluateRead(const FastqRecord &rec, const string &hashSig,
			unordered_map<string, bool> &hits);
	vector<unsigned> calculateStartIndexes(unsigned kmerSize);

	//group filters with same hash number
	unordered_map<string, vector<shared_ptr<BloomFilterInfo> > > m_infoFiles;
	unordered_map<string, vector<unsigned> > m_tiling;
	unordered_map<string, shared_ptr<MultiFilter> > m_filters;
	unordered_map<string, shared_ptr<BloomFilter> > m_filtersSingle;
	vector<string> m_hashSigs;
	double m_scoreThreshold;
	unsigned m_filterNum;
	const string &m_prefix;
	const string &m_postfix;
	const unsigned m_streakThreshold;
	const unsigned m_minHit;
	const bool m_minHitOnly;

	//Todo: is this really better than hard-coding them in the class?
	const string m_noMatch;
	const string m_multiMatch;
};

#endif /* BIOBLOOMCLASSIFIER_H_ */
